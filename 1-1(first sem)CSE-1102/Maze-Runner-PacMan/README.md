<div align="center">

# 🟡 The Maze-Runner: Pac-Man

**A Console-Based Maze-Chase Game in C**

[![Course](https://img.shields.io/badge/Course-CSE--1102-blue)](https://www.kuet.ac.bd/)
[![Semester](https://img.shields.io/badge/Semester-1--1%20(First)-green)](https://www.kuet.ac.bd/)
[![Language](https://img.shields.io/badge/Language-C-lightgrey?logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?logo=windows)](https://www.microsoft.com/windows)
[![University](https://img.shields.io/badge/University-KUET-red)](https://www.kuet.ac.bd/)

</div>

---

## 📖 Overview

**The Maze-Runner: Pac-Man** is a fully functional, terminal-based recreation of the classic Pac-Man arcade game, built entirely in **C** as a first-semester academic project. The player navigates a maze, collects pellets to score points, and survives against AI-controlled ghosts — all rendered in the Windows console with real-time input handling.

> **Course:** Structured Programming Laboratory (CSE-1102)  
> **Institution:** Khulna University of Engineering & Technology (KUET)  
> **Semester:** Year 1, Semester 1 (1-1), 2023  

---

## ✨ Features

| Feature | Description |
|---|---|
| 🔐 **Login / Signup** | User authentication with password masking (`*`) stored in `users.txt` |
| 🗺️ **Maze Engine** | Hardcoded 21×15 maze with walls (`#`), open paths, and collectible pellets (`.`) |
| 👾 **Ghost AI** | 3 ghosts that chase the player using distance-based directional logic |
| 🏆 **Scoreboard** | Persistent leaderboard sorted in descending order (`scoreboard.txt`) |
| 💾 **High Score** | Persistent high score tracking across sessions (`highscore.txt`) |
| ❤️ **Lives System** | Player starts with 3 lives; collision deducts a life and resets the round |
| 🎮 **Real-time Input** | Non-blocking `_kbhit()` / `_getch()` for smooth WASD controls |
| 🖥️ **HUD** | Live score, high score, and remaining lives displayed each frame |

---

## 🎮 Controls

```
  W  — Move Up
  A  — Move Left
  S  — Move Down
  D  — Move Right
  Q  — Quit Game
```

---

## 🗂️ Project Structure

```
Maze-Runner-PacMan/
├── main.c              ← Full game source (single-file)
├── Makefile            ← Build instructions (MinGW / GCC)
├── users.txt           ← Auto-generated: stores credentials
├── scoreboard.txt      ← Auto-generated: leaderboard entries
├── highscore.txt       ← Auto-generated: all-time high score
└── README.md
```

---

## ⚙️ Build & Run

### Prerequisites
- Windows OS
- [MinGW / GCC](https://www.mingw-w64.org/) (for `gcc` in PATH)  
  *or* [Code::Blocks](https://www.codeblocks.org/) / any C IDE with `<windows.h>` support

### Compile & Run

```bash
# Using GCC / MinGW
gcc main.c -o pacman.exe -lm
pacman.exe
```

```bash
# Using the Makefile
make
make run
```

> ⚠️ This project uses Windows-specific headers (`<conio.h>`, `<windows.h>`) and is designed for the Windows console. It will **not** compile natively on Linux/macOS without adaptation.

---

## 🔄 Game Flow

```
Startup
  └─► Login / Signup
         └─► Main Menu
               ├─► Start Game
               │     └─► Maze renders → Player moves → Ghosts chase
               │              ├─► Pellet collected → score++
               │              ├─► Ghost collision → lives--
               │              │       └─► lives == 0 → Game Over
               │              └─► All pellets → YOU WIN!
               ├─► Scoreboard  (sorted high scores)
               ├─► Logout
               └─► Exit
```

---

## 🧠 Key Concepts Demonstrated

- **Dynamic Memory Allocation** — core game variables managed via `malloc` / `free`
- **File I/O** — persistent user auth, scoreboard, and high score with file handling
- **2D Arrays** — maze represented as `char playfield[H][W]`
- **Real-time Input** — non-blocking keyboard with `_kbhit()` + `_getch()`
- **Basic AI** — ghost pathfinding using Manhattan distance minimization
- **Modular Design** — clean separation of rendering, input, game logic, and I/O

---

## 📊 Function Reference

| Function | Purpose |
|---|---|
| `display()` | Main login/signup menu |
| `menu()` | In-game menu (start, scoreboard, logout, exit) |
| `signup()` | New user registration with password masking |
| `login()` | Credential check against `users.txt` |
| `game()` | Core game loop: init, render, input, update, collision |
| `render_map()` | Redraws the full maze + HUD each frame |
| `update_pacman_position()` | Moves Pac-Man, handles pellet collection |
| `update_ghost_position()` | Moves all ghosts toward the player |
| `allowed_move()` | Validates a tile as passable (not a wall) |
| `col_check()` | Detects Pac-Man ↔ ghost collision |
| `gameover_check()` | Returns true when lives reach zero |
| `input_handling()` | Reads WASD/Q keypress non-blocking |
| `scoreboard()` | Reads, sorts, and displays leaderboard |
| `save_score()` | Appends score to file, updates high score |

---

## 📈 Possible Enhancements

- [ ] Graphical UI using SDL2 or raylib
- [ ] Multiple maze levels with increasing difficulty
- [ ] Advanced ghost AI (Blinky, Pinky, Inky, Clyde patterns)
- [ ] Power pellets and ghost vulnerability mode
- [ ] Pause / resume functionality
- [ ] Animated Pac-Man character
- [ ] Linux/macOS cross-platform port (ncurses)

---

## 👥 Team

| Name | Roll | GitHub |
|---|---|---|
| Farhan Shariar | 2307113 | [@galib556](https://github.com/galib556) |
| Sadat Ul-Islam Britto | 2307107 | — |
| Sk. Jisan Rahman | 2307114 | — |

---

## 🎓 Supervised By

| Instructor | Role |
|---|---|
| Mr. Md. Nazirulhasan Shawon | Lecturer, Dept. of CSE, KUET |
| Md. Tajmilur Rahman | Lecturer, Dept. of CSE, KUET |

---

## 📚 References

1. *ANSI C* — E. Balagurusamy  
2. *Let Us C* — Yashavant Kanetkar  
3. [TutorialsPoint](https://www.tutorialspoint.com/)  
4. [GeeksforGeeks](https://www.geeksforgeeks.org/)  
5. [W3Schools](https://www.w3schools.com/)

---

<div align="center">

**KUET · CSE · Batch 2023**  
*Structured Programming Laboratory — CSE-1102*

</div>
