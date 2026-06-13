/* ============================================================
 * inventory.h — Inventory & crafting for PROJECT NANO
 * ============================================================ */

#ifndef INVENTORY_H
#define INVENTORY_H

#include "game.h"

/* Add an item string to player inventory; returns 1 on success */
int  addItem(Player *p, const char *itemName);

/* Check whether player carries a specific item; returns 1 if yes */
int  hasItem(const Player *p, const char *itemName);

/* Remove one copy of an item; returns 1 on success */
int  removeItem(Player *p, const char *itemName);

/* Print all inventory slots to the terminal */
void showInventory(const Player *p);

/* Attempt crafting: Paperclip + Rubber Band -> Grappling Hook */
void tryCraft(Player *p);

#endif /* INVENTORY_H */
