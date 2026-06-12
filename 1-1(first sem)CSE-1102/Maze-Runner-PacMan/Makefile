# ─────────────────────────────────────────────────────────────────
# Makefile — The Maze-Runner: Pac-Man
# CSE-1102 Structured Programming Laboratory, KUET
# Requires: MinGW/GCC on Windows
# ─────────────────────────────────────────────────────────────────

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2
LIBS    = -lm
TARGET  = pacman.exe
SRC     = main.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)
	@echo "Build successful -> $(TARGET)"

run: $(TARGET)
	./$(TARGET)

clean:
	del /Q $(TARGET) 2>nul || rm -f $(TARGET)
