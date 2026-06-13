/* ============================================================
 * inventory.c — Inventory management and crafting
 * ============================================================ */

#include "inventory.h"

/* ── Add an item to inventory ── */
int addItem(Player *p, const char *itemName)
{
    if (p->itemCount >= MAX_ITEMS) {
        printf("  [!] Inventory full! Cannot carry more items.\n");
        return 0;
    }
    strncpy(p->inventory[p->itemCount], itemName, MAX_NAME - 1);
    p->inventory[p->itemCount][MAX_NAME - 1] = '\0';
    p->itemCount++;
    printf("  [+] Picked up: %s\n", itemName);
    return 1;
}

/* ── Check if player has a specific item ── */
int hasItem(const Player *p, const char *itemName)
{
    int i;
    for (i = 0; i < p->itemCount; i++) {
        if (strcmp(p->inventory[i], itemName) == 0)
            return 1;
    }
    return 0;
}

/* ── Remove one copy of an item by shifting the array ── */
int removeItem(Player *p, const char *itemName)
{
    int i, found = -1;
    for (i = 0; i < p->itemCount; i++) {
        if (strcmp(p->inventory[i], itemName) == 0) {
            found = i;
            break;
        }
    }
    if (found == -1) return 0;

    /* Shift remaining items left */
    for (i = found; i < p->itemCount - 1; i++) {
        strncpy(p->inventory[i], p->inventory[i + 1], MAX_NAME);
    }
    strcpy(p->inventory[p->itemCount - 1], ITEM_NONE);
    p->itemCount--;
    return 1;
}

/* ── Display the inventory ── */
void showInventory(const Player *p)
{
    int i;
    printf("\n  +====================+\n");
    printf("  |    INVENTORY       |\n");
    printf("  +====================+\n");

    if (p->itemCount == 0) {
        printf("  | (nothing yet)      |\n");
    } else {
        for (i = 0; i < p->itemCount; i++) {
            printf("  | %-18s |\n", p->inventory[i]);
        }
    }

    printf("  +====================+\n");
    printf("  Slots used: %d / %d\n\n", p->itemCount, MAX_ITEMS);
}

/* ── Crafting: Paperclip + Rubber Band → Grappling Hook ── */
void tryCraft(Player *p)
{
    printf("\n  +====================+\n");
    printf("  |      CRAFTING      |\n");
    printf("  +====================+\n");
    printf("  | Paperclip          |\n");
    printf("  |     +              |\n");
    printf("  | Rubber Band        |\n");
    printf("  |     =              |\n");
    printf("  | Grappling Hook     |\n");
    printf("  +====================+\n");

    if (hasItem(p, ITEM_PAPERCLIP) && hasItem(p, ITEM_RUBBER_BAND)) {
        removeItem(p, ITEM_PAPERCLIP);
        removeItem(p, ITEM_RUBBER_BAND);
        addItem(p, ITEM_GRAPPLING);
        printf("  [*] Grappling Hook crafted! Move into the exit door to enter the Vent Tunnel.\n\n");
    } else {
        printf("  [!] You need a Paperclip AND a Rubber Band.\n\n");
    }
}
