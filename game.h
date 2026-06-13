/* ============================================================
 * game.h — Core game definitions for PROJECT NANO: BioNano Escape
 * ============================================================ */

#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* ── Map dimensions ── */
#define MAP_ROWS    8
#define MAP_COLS    22

/* ── Game limits ── */
#define MAX_ITEMS   10
#define MAX_NAME    32
#define START_TIME  60    /* seconds of in-game time */
#define START_HP    5
#define MAX_HP      5

/* ── Tile characters ── */
#define TILE_WALL   '#'
#define TILE_FLOOR  '.'
#define TILE_PLAYER '@'
#define TILE_ITEM   'K'
#define TILE_EXIT   'D'
#define TILE_BLOCK  'X'   /* impassable object (e.g. locked door) */

/* ── Room IDs ── */
#define ROOM_LAB_BENCH   0
#define ROOM_KEY_CANYON  1
#define ROOM_VENT_TUNNEL 2
#define ROOM_EXIT_AIRLOCK 3
#define TOTAL_ROOMS      4

/* ── Item names ── */
#define ITEM_NONE         "Empty"
#define ITEM_PAPERCLIP    "Paperclip"
#define ITEM_RUBBER_BAND  "Rubber Band"
#define ITEM_GRAPPLING    "Grappling Hook"
#define ITEM_ACCESS_CHIP  "Access Chip"

/* ── Ending codes ── */
#define END_NONE       0
#define END_HERO       1   /* escaped fast, low noise */
#define END_SURVIVOR   2   /* escaped normally */
#define END_CAUGHT     3   /* detected by researcher */
#define END_STERILISED 4   /* time ran out or health = 0 */

/* ── Player structure ── */
typedef struct {
    int x;          /* column position on map */
    int y;          /* row    position on map  */
    int health;     /* 0–5 hit points          */
    int noise;      /* 0–100 percent           */
    int timeLeft;   /* in-game seconds left    */
    int room;       /* current room ID         */
    char inventory[MAX_ITEMS][MAX_NAME];
    int  itemCount;
    int  ending;    /* set when game over      */
} Player;

/* ── Room structure ── */
typedef struct {
    char  name[MAX_NAME];
    char  grid[MAP_ROWS][MAP_COLS];   /* ASCII map  */
    int   itemRow;                     /* where item spawns */
    int   itemCol;
    char  itemName[MAX_NAME];         /* item on floor      */
    int   itemPickedUp;               /* 1 = already taken  */
    int   locked;                     /* 1 = needs grappling hook */
} Room;

/* ── Function prototypes (game.c) ── */
void  initGame(Player *p, Room rooms[]);
void  runGame(Player *p, Room rooms[]);
void  drawHUD(const Player *p);
void  handleInput(Player *p, Room rooms[], char key);
void  tryMove(Player *p, Room rooms[], int dy, int dx);
void  tryInteract(Player *p, Room rooms[]);
void  randomEvent(Player *p);
void  updateNoise(Player *p, int delta);
void  takeDamage(Player *p, int amount);
void  showEnding(const Player *p);
int   checkWin(const Player *p);

#endif /* GAME_H */
