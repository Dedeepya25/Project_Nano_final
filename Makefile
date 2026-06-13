# ============================================================
# Makefile — PROJECT NANO: BioNano Escape
# Usage:
#   make          → build the game
#   make run      → build and run
#   make clean    → remove binary
# ============================================================

CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -pedantic
TARGET  = project_nano
SRCS    = main.c game.c map.c inventory.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

run: all
	$(TARGET)

clean:
	del /Q $(TARGET).exe 2>nul || true
