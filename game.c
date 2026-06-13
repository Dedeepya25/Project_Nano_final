/* ============================================================
 * game.c — Core game logic for PROJECT NANO: BioNano Escape
 * ============================================================ */

#include "game.h"
#include "map.h"
#include "inventory.h"

/* ── Forward declaration of static helpers ── */
static void clearScreen(void);
static void printBanner(void);
static void printHelp(void);
static int  isWall(const Room *room, int row, int col);

/* ── Clear the terminal (cross-platform friendly) ── */
static void clearScreen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* ── Splash screen banner ── */
static void printBanner(void)
{
    clearScreen();
    printf("\n");
    printf("  +============================================+\n");
    printf("  | PROJECT NANO: BioNano Escape              |\n");
    printf("  | You are 2 microns tall. Escape the lab.    |\n");
    printf("  +============================================+\n");
    printf("  | W/A/S/D  Move      E  Interact            |\n");
    printf("  | I        Inventory  C  Craft               |\n");
    printf("  | Q        Quit                             |\n");
    printf("  +============================================+\n");
    printf("\n  Press ENTER to begin...\n");
    getchar();   /* wait for player */
}

/* ── In-game key reference ── */
static void printHelp(void)
{
    printf("\n  Controls: W=Up  A=Left  S=Down  D=Right\n");
    printf("            E=Interact (items/doors)  I=Inventory  C=Craft  Q=Quit\n\n");
}

/* ── Initialise player and rooms ── */
void initGame(Player *p, Room rooms[])
{
    int i;

    /* Seed random number generator */
    srand((unsigned int)time(NULL));

    /* Set up player */
    p->x         = 2;
    p->y         = 4;
    p->health    = START_HP;
    p->noise     = 0;
    p->timeLeft  = START_TIME;
    p->room      = ROOM_LAB_BENCH;
    p->itemCount = 0;
    p->ending    = END_NONE;

    /* Clear inventory slots */
    for (i = 0; i < MAX_ITEMS; i++)
        strcpy(p->inventory[i], ITEM_NONE);

    /* Build room maps */
    initMaps(rooms);
}

/* ── Draw the HUD (stats bar) ── */
void drawHUD(const Player *p)
{
    int i;

    printf("  TIME: %-3d   HEALTH: [", p->timeLeft);

    /* Draw filled/empty hearts */
    for (i = 0; i < MAX_HP; i++) {
        if (i < p->health)
            printf("*");   /* asterisk for health */
        else
            printf(".");
    }

    printf("]   NOISE: ");
    for (i = 0; i < 10; i++) {
        if (i < (p->noise / 10))
            printf("#");
        else
            printf("-");
    }
    printf(" %3d%%\n", p->noise);
    printf("  -----------------------------------------\n");
}

/* ── Check if a tile is solid (wall or out-of-bounds) ── */
static int isWall(const Room *room, int row, int col)
{
    if (row < 0 || row >= MAP_ROWS || col < 0 || col >= MAP_COLS - 1)
        return 1;
    char tile = room->grid[row][col];
    return (tile == TILE_WALL || tile == TILE_BLOCK);
}

/* ── Attempt to move the player ── */
void tryMove(Player *p, Room rooms[], int dy, int dx)
{
    int newY = p->y + dy;
    int newX = p->x + dx;
    Room *room = &rooms[p->room];

    if (isWall(room, newY, newX)) {
        printf("  [!] Blocked!\n");
        return;
    }

    /* Check for exit door tile */
    char tile = room->grid[newY][newX];
    if (tile == TILE_EXIT) {
        /* Try to advance to next room */
        int nextRoom = p->room + 1;

        if (nextRoom >= TOTAL_ROOMS) {
            /* Already in last room — handled by tryInteract */
            printf("  [!] Use E near the door to activate the airlock.\n");
            return;
        }

        /* Room 2 (Vent Tunnel) requires Grappling Hook */
        if (nextRoom == ROOM_VENT_TUNNEL && rooms[nextRoom].locked) {
            if (!hasItem(p, ITEM_GRAPPLING)) {
                printf("  [!] The vent is too high! You need a Grappling Hook.\n");
                return;
            }
            rooms[nextRoom].locked = 0;
            printf("  [*] You use the Grappling Hook to climb into the vent!\n");
        }

        /* Move to next room */
        p->room = nextRoom;
        p->x    = 2;
        p->y    = 4;

        printf("  --> Entered: %s\n", rooms[p->room].name);

        /* Room transition makes a little noise */
        updateNoise(p, 2);
        p->timeLeft -= 2;
        return;
    }

    /* Normal move */
    p->x = newX;
    p->y = newY;
    updateNoise(p, 1);
    p->timeLeft--;
}

/* ── Interact with adjacent items or the exit door ── */
void tryInteract(Player *p, Room rooms[])
{
    Room *room = &rooms[p->room];

    /* Check if player is adjacent to the item */
    int dr = abs(p->y - room->itemRow);
    int dc = abs(p->x - room->itemCol);

    if (strcmp(room->itemName, ITEM_NONE) != 0 && dr <= 1 && dc <= 1) {
        if (!room->itemPickedUp) {
            /* Pick up the item */
            if (addItem(p, room->itemName))
                room->itemPickedUp = 1;
            return;
        }

        printf("  [!] You already picked up the %s here.\n", room->itemName);
        return;
    }

    /* Check if player is adjacent to the exit door (column 18, row 6) */
    if (p->room == ROOM_EXIT_AIRLOCK) {
        if (abs(p->y - 6) <= 1 && abs(p->x - 18) <= 1) {
            if (hasItem(p, ITEM_ACCESS_CHIP)) {
                printf("\n  [*] You insert the Access Chip...\n");
                printf("  [*] AIRLOCK OPENING! You crawl through to freedom!\n\n");
                /* Determine ending based on performance */
                if (p->timeLeft > 30 && p->noise < 40)
                    p->ending = END_HERO;
                else
                    p->ending = END_SURVIVOR;
            } else {
                printf("  [!] The airlock needs an Access Chip.\n");
            }
        } else {
            printf("  [!] Nothing to interact with here.\n");
        }
        return;
    }

    printf("  [!] Nothing nearby to interact with.\n");
}

/* ── Adjust noise level, clamped to 0–100 ── */
void updateNoise(Player *p, int delta)
{
    p->noise += delta;
    if (p->noise < 0)   p->noise = 0;
    if (p->noise > 100) p->noise = 100;

    if (p->noise >= 100 && p->ending == END_NONE)
        p->ending = END_CAUGHT;
}

/* ── Deal damage to player ── */
void takeDamage(Player *p, int amount)
{
    p->health -= amount;
    if (p->health < 0) p->health = 0;
}

/* ── Random events fired periodically ── */
void randomEvent(Player *p)
{
    int roll = rand() % 100;

    /* ~20% chance of a random event each turn */
    if (roll >= 20) return;

    int event = rand() % 4;

    printf("\n  == RANDOM EVENT ==\n");

    switch (event) {
        case 0:
            printf("  Footsteps nearby! (+5 Noise)\n");
            updateNoise(p, 5);
            break;

        case 1:
            printf("  A water droplet splashes on you! (-1 Health)\n");
            takeDamage(p, 1);
            break;

        case 2: {
            /* Researcher event — player chooses hide or run */
            int choice;
            printf("  A researcher walks past!\n");
            printf("  1. Hide quietly  (Noise stays same)\n");
            printf("  2. Run!          (Noise +15, Time -3)\n");
            printf("  Choice: ");
            if (scanf(" %d", &choice) != 1) choice = 1;
            /* flush leftover newline */
            while (getchar() != '\n') {}

            if (choice == 2) {
                updateNoise(p, 8);
                p->timeLeft -= 3;
                printf("  You sprint away!\n");
            } else {
                printf("  You crouch behind a dust particle.\n");
            }
            break;
        }

        case 3:
            printf("  Air current slows you down! (Time -2)\n");
            p->timeLeft -= 2;
            break;
    }

    printf("  ==================\n\n");
}

/* ── Check if the game has ended ── */
int checkWin(const Player *p)
{
    return (p->ending != END_NONE ||
            p->health <= 0        ||
            p->timeLeft <= 0 ||
            p->noise >= 100);
}

/* ── Display the appropriate ending screen ── */
void showEnding(const Player *p)
{
    clearScreen();

    if (p->health <= 0) {
        printf("\n  +========================================+\n");
        printf("  | YOU HAVE BEEN SQUISHED                 |\n");
        printf("  |                                        |\n");
        printf("  | ENDING: STERILISED                     |\n");
        printf("  | A researcher's finger ended your       |\n");
        printf("  | microscopic adventure.                 |\n");
        printf("  +========================================+\n");
    } else if (p->timeLeft <= 0) {
        printf("\n  +========================================+\n");
        printf("  | STERILISATION CYCLE COMPLETE          |\n");
        printf("  |                                        |\n");
        printf("  | ENDING: STERILISED                     |\n");
        printf("  | UV light reduced you to nothing.       |\n");
        printf("  +========================================+\n");
    } else if (p->ending == END_CAUGHT) {
        printf("\n  +========================================+\n");
        printf("  | RESEARCHER ALERT                      |\n");
        printf("  |                                        |\n");
        printf("  | ENDING: CAUGHT                        |\n");
        printf("  | Your movements attracted attention    |\n");
        printf("  | and a researcher found you.           |\n");
        printf("  +========================================+\n");
    } else if (p->ending == END_HERO) {
        printf("\n  +========================================+\n");
        printf("  | ESCAPE SUCCESSFUL!                     |\n");
        printf("  |                                        |\n");
        printf("  | ENDING: HERO                           |\n");
        printf("  | Fast, silent, unstoppable.             |\n");
        printf("  | You are the greatest nano-agent        |\n");
        printf("  | the world has never seen.              |\n");
        printf("  +========================================+\n");
    } else {
        printf("\n  +========================================+\n");
        printf("  | ESCAPE SUCCESSFUL!                     |\n");
        printf("  |                                        |\n");
        printf("  | ENDING: SURVIVOR                       |\n");
        printf("  | Battered but free. Good enough.        |\n");
        printf("  +========================================+\n");
    }

    printf("\n  Final Stats - Time left: %d   Noise: %d%%   Health: %d\n\n",
           p->timeLeft, p->noise, p->health);
}

/* ── Process one keypress ── */
void handleInput(Player *p, Room rooms[], char key)
{
    key = (char)toupper((unsigned char)key);

    switch (key) {
        case 'W': tryMove(p, rooms, -1,  0); break;
        case 'S': tryMove(p, rooms, +1,  0); break;
        case 'A': tryMove(p, rooms,  0, -1); break;
        case 'D': tryMove(p, rooms,  0, +1); break;
        case 'E': tryInteract(p, rooms);     break;
        case 'I': showInventory(p);          break;
        case 'C': tryCraft(p);               break;
        case 'H': printHelp();               break;
        case 'Q':
            printf("  [!] You gave up. The lab claims another victim.\n");
            p->ending = END_STERILISED;
            break;
        default:
            printf("  [?] Unknown key. Press H for help.\n");
            break;
    }
}

/* ── Main game loop ── */
void runGame(Player *p, Room rooms[])
{
    char input[4];

    printBanner();

    while (!checkWin(p)) {
        clearScreen();

        /* Draw current room map */
        drawMap(p, rooms);

        /* Draw HUD below the map */
        printf("\n");
        drawHUD(p);
        printf("\n");

        if (p->noise >= 75 && p->noise < 100) {
            printf("  [!] WARNING: Noise is critical. Move carefully or you'll attract a researcher.\n\n");
        }

        /* Show a hint for the current room */
        switch (p->room) {
            case ROOM_LAB_BENCH:
                printf("  Hint: Pick up the Paperclip (K) with E, then move to the door (D) to leave this bench.\n");
                break;
            case ROOM_KEY_CANYON:
                printf("  Hint: Pick up the Rubber Band (K), craft the Grappling Hook with C, then go through door D.\n");
                break;
            case ROOM_VENT_TUNNEL:
                printf("  Hint: Use the Grappling Hook to enter the vent and move into the exit door (D) to reach the airlock.\n");
                break;
            case ROOM_EXIT_AIRLOCK:
                printf("  Hint: Pick up the Access Chip (K), then press E next to the door (D) to escape.\n");
                break;
        }

        printf("\n  > Command: ");
        fflush(stdout);

        /* Read one line, use first character */
        if (fgets(input, sizeof(input), stdin) == NULL)
            break;
        if (input[0] == '\n') continue;   /* empty enter */

        handleInput(p, rooms, input[0]);

        /* Trigger a possible random event after each action */
        randomEvent(p);

        /* Brief pause so player can read messages */
        printf("\n  [Press ENTER to continue]");
        while (getchar() != '\n') {}
    }

    showEnding(p);
}
