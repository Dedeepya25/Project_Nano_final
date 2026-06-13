/* ============================================================
 * main.c — Entry point for PROJECT NANO: BioNano Escape
 *
 * Build:   gcc *.c -o project_nano
 * Run:     ./project_nano
 *
 * Concepts demonstrated:
 *   - Structures   (Player, Room)
 *   - Arrays       (inventory, map grid, rooms array)
 *   - Functions    (split across game.c / map.c / inventory.c)
 *   - Pointers     (Player * passed to all update functions)
 *   - Strings      (item names, room names, map rows)
 * ============================================================ */

#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "game.h"
#include "map.h"
#include "inventory.h"

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    setlocale(LC_ALL, "");

    /* ── Declare game state on the stack (no malloc needed) ── */
    Player player;
    Room   rooms[TOTAL_ROOMS];

    /* ── Initialise everything ── */
    initGame(&player, rooms);

    /* ── Run the game loop (blocks until game over) ── */
    runGame(&player, rooms);

    return 0;
}
