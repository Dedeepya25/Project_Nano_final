/* ============================================================
 * map.c — Room layouts and map rendering
 * ============================================================ */

#include "map.h"

/* ── Helper: copy a string literal into a fixed char array ── */
static void setRow(char dst[MAP_COLS], const char *src)
{
    int i;
    for (i = 0; i < MAP_COLS - 1 && src[i] != '\0'; i++)
        dst[i] = src[i];
    /* pad with spaces if the literal is short */
    for (; i < MAP_COLS - 1; i++)
        dst[i] = ' ';
    dst[MAP_COLS - 1] = '\0';
}

/* ── Build all four room layouts ── */
void initMaps(Room rooms[])
{
    /* ── Room 0: Lab Bench ── */
    strcpy(rooms[0].name, "LAB BENCH");
    setRow(rooms[0].grid[0], "######################");
    setRow(rooms[0].grid[1], "#....................#");
    setRow(rooms[0].grid[2], "#..K.................#");
    setRow(rooms[0].grid[3], "#....................#");
    setRow(rooms[0].grid[4], "#....................#");
    setRow(rooms[0].grid[5], "#....................#");
    setRow(rooms[0].grid[6], "#.................D..#");
    setRow(rooms[0].grid[7], "######################");
    rooms[0].itemRow      = 2;
    rooms[0].itemCol      = 3;
    strcpy(rooms[0].itemName, ITEM_PAPERCLIP);
    rooms[0].itemPickedUp = 0;
    rooms[0].locked       = 0;

    /* ── Room 1: Keyboard Canyon ── */
    strcpy(rooms[1].name, "KEYBOARD CANYON");
    setRow(rooms[1].grid[0], "######################");
    setRow(rooms[1].grid[1], "#....................#");
    setRow(rooms[1].grid[2], "#....................#");
    setRow(rooms[1].grid[3], "#.......K............#");
    setRow(rooms[1].grid[4], "#....................#");
    setRow(rooms[1].grid[5], "#....................#");
    setRow(rooms[1].grid[6], "#.................D..#");
    setRow(rooms[1].grid[7], "######################");
    rooms[1].itemRow      = 3;
    rooms[1].itemCol      = 8;
    strcpy(rooms[1].itemName, ITEM_RUBBER_BAND);
    rooms[1].itemPickedUp = 0;
    rooms[1].locked       = 0;

    /* ── Room 2: Vent Tunnel (requires Grappling Hook) ── */
    strcpy(rooms[2].name, "VENT TUNNEL");
    setRow(rooms[2].grid[0], "######################");
    setRow(rooms[2].grid[1], "#....................#");
    setRow(rooms[2].grid[2], "#....................#");
    setRow(rooms[2].grid[3], "#....................#");
    setRow(rooms[2].grid[4], "#....................#");
    setRow(rooms[2].grid[5], "#....................#");
    setRow(rooms[2].grid[6], "#.................D..#");
    setRow(rooms[2].grid[7], "######################");
    rooms[2].itemRow      = 0;   /* no item in this room */
    rooms[2].itemCol      = 0;
    strcpy(rooms[2].itemName, ITEM_NONE);
    rooms[2].itemPickedUp = 1;   /* nothing to pick up */
    rooms[2].locked       = 1;   /* needs grappling hook */

    /* ── Room 3: Exit Airlock ── */
    strcpy(rooms[3].name, "EXIT AIRLOCK");
    setRow(rooms[3].grid[0], "######################");
    setRow(rooms[3].grid[1], "#....................#");
    setRow(rooms[3].grid[2], "#.......K............#");
    setRow(rooms[3].grid[3], "#....................#");
    setRow(rooms[3].grid[4], "#....................#");
    setRow(rooms[3].grid[5], "#....................#");
    setRow(rooms[3].grid[6], "#.................D..#");
    setRow(rooms[3].grid[7], "######################");
    rooms[3].itemRow      = 2;
    rooms[3].itemCol      = 8;
    strcpy(rooms[3].itemName, ITEM_ACCESS_CHIP);
    rooms[3].itemPickedUp = 0;
    rooms[3].locked       = 0;
}

/* ── Render the map, overlaying player + items ── */
void drawMap(const Player *p, const Room rooms[])
{
    int r, c;
    const Room *room = &rooms[p->room];

    printf("\n  [ %s ]\n", room->name);
    printf("  +");
    for (c = 0; c < MAP_COLS - 1; c++) printf("-");
    printf("+\n");

    for (r = 0; r < MAP_ROWS; r++) {
        printf("  |");
        for (c = 0; c < MAP_COLS - 1; c++) {
            char tile = room->grid[r][c];

            /* Draw player on top */
            if (r == p->y && c == p->x) {
                putchar(TILE_PLAYER);
                continue;
            }

            /* Draw item if still on the floor */
            if (!room->itemPickedUp &&
                r == room->itemRow && c == room->itemCol) {
                putchar(TILE_ITEM);
                continue;
            }

            switch (tile) {
                case TILE_WALL:  printf("#"); break;
                case TILE_FLOOR: putchar(' '); break;
                default:         putchar(tile); break;
            }
        }
        printf("|\n");
    }

    printf("  +");
    for (c = 0; c < MAP_COLS - 1; c++) printf("-");
    printf("+\n");
}
