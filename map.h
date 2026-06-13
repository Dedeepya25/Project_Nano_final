/* ============================================================
 * map.h — Map rendering for PROJECT NANO: BioNano Escape
 * ============================================================ */

#ifndef MAP_H
#define MAP_H

#include "game.h"

/* Build the four room layouts into the rooms array */
void initMaps(Room rooms[]);

/* Print the current room with player and items overlaid */
void drawMap(const Player *p, const Room rooms[]);

#endif /* MAP_H */
