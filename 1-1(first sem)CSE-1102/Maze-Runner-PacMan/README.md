# The Maze-Runner: Pac-Man

A console-based Pac-Man game built in C for the CSE-1102 (Structured Programming Laboratory) course at KUET.

The player moves through a maze collecting pellets while 4 ghosts chase them down. Scores are saved across sessions and there's a login system so multiple players can track their high scores separately.

---

## How to Run

**Requirements:** Windows, Code::Blocks with GCC (MinGW)

1. Open `TheMazeRunner_PacMan.cbp` in Code::Blocks
2. Hit **Build & Run** (F9)

Or compile manually with GCC:
```bash
gcc main.c functions.c -o pacman.exe -lm
pacman.exe
```

> Only works on Windows — uses `<conio.h>` and `<windows.h>` for real-time input and console rendering.

---

## Controls

| Key | Action |
|-----|--------|
| W   | Move Up |
| A   | Move Left |
| S   | Move Down |
| D   | Move Right |
| Q   | Quit |

---

## Features

- **Login / Sign Up** — accounts stored in `users.txt`, passwords masked with `*` on input
- **Maze** — 60×30 playfield, all open spaces filled with pellets on game start
- **Ghosts (×4)** — randomly placed, chase the player using distance-based movement
- **Lives** — 3 lives; losing all ends the game
- **Scoreboard** — saved to `scoreboard.txt`, sorted by score (highest first)
- **High Score** — persisted in `highscore.txt` across sessions

---

## File Structure

```
TheMazeRunner_PacMan/
├── main.c                      # Entry point, memory allocation
├── functions.h                 # Macros, extern declarations, prototypes
├── functions.c                 # All game logic
├── TheMazeRunner_PacMan.cbp    # Code::Blocks project file
├── users.txt                   # Stored user credentials
├── scoreboard.txt              # Player scores
└── highscore.txt               # All-time high score
```

---

## Implementation Notes

The maze is a hardcoded 2D char array. At game start, every non-wall cell gets filled with `.` (pellets). Pac-Man spawns at the center (`W/2`, `H/2`), ghosts are randomly placed on valid pellet tiles.

Ghost movement runs every 3 game ticks to keep them slower than the player. Each ghost picks the axis (horizontal or vertical) where the distance to the player is greater, moves one step that way, and falls back to the other axis if blocked by a wall.

Dynamic memory allocation is used for all major game variables (`px`, `py`, `gx`, `gy`, `score`, `h_score`, `final_lives`, `flag`) as required by the course.

---

## Team

| Name | Roll |
|------|------|
| Farhan Shariar | 2307113 |
| Sadat Ul-Islam Britto | 2307107 |
| Sk. Jisan Rahman | 2307114 |

**Course Teachers:**  
Md. Nazirulhasan Shawon — Lecturer, CSE, KUET  
Md. Tajmilur Rahman — Lecturer, CSE, KUET

---

**Course:** CSE-1102 — Structured Programming Laboratory  
**Semester:** 1-1 | **KUET CSE, Batch 2023**
