# PROJECT NANO — BioNano Escape

> *You are 2 microns tall, trapped inside a research laboratory.  
> Sterilisation begins soon. Escape before the UV light finds you.*

---

## Quick Start

```bash
# Compile
gcc *.c -o project_nano

# Or use Make
make

# Run
./project_nano
```

---

## Controls

| Key | Action          |
|-----|-----------------|
| W   | Move Up         |
| A   | Move Left       |
| S   | Move Down       |
| D   | Move Right      |
| E   | Interact        |
| I   | Show Inventory  |
| C   | Craft           |
| H   | Help            |
| Q   | Quit            |

---

## Rooms

| Room | Name            | Goal                                  |
|------|-----------------|---------------------------------------|
| 1    | Lab Bench       | Collect the **Paperclip**             |
| 2    | Keyboard Canyon | Collect the **Rubber Band**           |
| 3    | Vent Tunnel     | Craft a **Grappling Hook** first (C)  |
| 4    | Exit Airlock    | Grab the **Access Chip**, use E at door |

### Crafting Recipe

```
Paperclip + Rubber Band  →  Grappling Hook
```

Press **C** at any time once you have both ingredients.

---

## Map Legend

```
@  = You (the player)
K  = Key item on the floor
D  = Exit door to next room
#  = Wall (impassable)
.  = Open floor
```

---

## Endings

| Ending      | Condition                                    |
|-------------|----------------------------------------------|
| **Hero**    | Escaped with > 30 s left and < 40% noise     |
| **Survivor**| Escaped normally                             |
| **Sterilised** | Health = 0 OR time ran out               |

---

## C Concepts Demonstrated

| Concept    | Where                                          |
|------------|------------------------------------------------|
| `struct`   | `Player` and `Room` typedefs in `game.h`      |
| Arrays     | `inventory[][]`, `grid[][]`, `rooms[]`        |
| Functions  | Separate `.c` files, single responsibility    |
| Pointers   | `Player *p` passed to every update function   |
| Strings    | `strncpy`, `strcmp`, `strcpy` on item names   |

---

## File Structure

```
project_nano/
├── main.c          Entry point
├── game.h          Shared types, constants, prototypes
├── game.c          Game loop, movement, events, endings
├── map.h / map.c   Room layouts and ASCII rendering
├── inventory.h / inventory.c   Inventory and crafting
├── Makefile
└── README.md
```

---

*Good luck, nano-agent.*
