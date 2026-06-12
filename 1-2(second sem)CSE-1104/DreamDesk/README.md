<div align="center">

# DreamDesk — Object-Oriented Desktop Item Manager

**A fully functional virtual desktop manager built in C++ to demonstrate all core OOP concepts — encapsulation, inheritance, polymorphism, abstraction, and more.**

[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue?logo=cplusplus)](https://isocpp.org/)
[![Paradigm](https://img.shields.io/badge/Paradigm-Object--Oriented-purple)](https://en.wikipedia.org/wiki/Object-oriented_programming)
[![Course](https://img.shields.io/badge/Course-CSE--1104-orange)]()
[![Status](https://img.shields.io/badge/Status-Complete-brightgreen)]()
[![License](https://img.shields.io/badge/License-MIT-lightgrey)](../../LICENSE)

</div>

---

## Overview

DreamDesk is a command-line application that simulates a desktop file management system — the kind you interact with every day in Windows or macOS — reimplemented from scratch in pure C++17 to demonstrate every core Object-Oriented Programming concept covered in CSE-1206.

The program lets you create **files**, **folders**, and **shortcuts**, nest items inside folders, rename and delete them, and persist the entire desktop state to disk with save/load support.

---

## OOP Concepts Demonstrated

| Concept | Where Applied |
|---------|---------------|
| **Abstraction** | `DesktopItem` — abstract base class with 4 pure virtual methods |
| **Encapsulation** | `name` is `protected`; accessed only through `getName()` / `renameTo()` |
| **Inheritance** | `FileItem`, `ShortcutItem`, `FolderItem` all inherit from `DesktopItem` |
| **Polymorphism** | `open()`, `print()`, `clone()`, `save()` — overridden in each derived class |
| **Operator Overloading** | `operator<<` (stream output), `operator==` (equality by name) |
| **Templates** | `heapClone<T>()` — generic clone utility |
| **Smart Pointers** | `unique_ptr<DesktopItem>` throughout — no manual `delete` |
| **Composition** | `FolderItem` owns a `vector<unique_ptr<DesktopItem>>` |
| **STL Containers** | `vector`, `unordered_map` for storage and O(1) name lookup |
| **Lambda / `std::function`** | Recursive tree traversal inside `rebuildIndex()` |
| **RTTI / `dynamic_cast`** | Safe downcasting to `FolderItem*` |
| **File I/O** | `ofstream` / `ifstream` for save and load |
| **Namespaces** | All code lives inside `namespace DreamDesk` |
| **Move Semantics** | `unique_ptr` ownership transfer via `std::move()` |

---

## Architecture

```
DesktopItem  (abstract base)
│   + getName()   + renameTo()
│   # name
│   ~ open()      [pure virtual]
│   ~ print()     [pure virtual]
│   ~ clone()     [pure virtual]
│   ~ save()      [pure virtual]
│
├── FileItem
│       content: string
│       open()  → prints file content
│
├── ShortcutItem
│       targetName: string
│       open()  → prints redirect arrow
│
└── FolderItem
        children: vector<unique_ptr<DesktopItem>>
        open()  → lists children with index
        ─── Composite pattern: can nest any DesktopItem ───

Desktop (manager class)
    topItems:   vector<unique_ptr<DesktopItem>>   ← ownership
    nameIndex:  unordered_map<string, DesktopItem*>  ← O(1) lookup
    ─── save / load via a simple custom text format ───
```

---

## Save / Load Format

DreamDesk persists the desktop to a human-readable text file:

```
DREAMDESK_V1
2
FILE
NAME notes.txt
CONTENT Meeting at 3pm
END
FOLDER
NAME Work
COUNT 1
  FILE
  NAME report.txt
  CONTENT Q3 complete
  END
END
EOF
```

The parser is hand-written (no external libraries) to demonstrate structured text I/O.

---

## How to Build and Run

### Requirements

- Any C++17-compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- No external dependencies — standard library only

### Build

```bash
# Linux / macOS
g++ -std=c++17 DreamDesk.cpp -O2 -o DreamDesk
./DreamDesk

# Windows (MinGW)
g++ -std=c++17 DreamDesk.cpp -O2 -o DreamDesk.exe
DreamDesk.exe
```

### Sample Session

```
╔═══════════════════════════════════════════╗
║          D R E A M D E S K               ║
║    Object-Oriented Desktop Item Manager  ║
║    CSE-1206 · OOP Lab · KUET CSE         ║
╚═══════════════════════════════════════════╝

┌─ Menu ──────────────────────────────┐
│  1. List desktop                    │
│  2. Create file (top-level)         │
│  3. Create folder (top-level)       │
│  4. Add file inside a folder        │
│  5. Create shortcut (top-level)     │
│  6. Open / view an item             │
│  7. Rename item                     │
│  8. Delete item                     │
│  9. Save desktop to file            │
│  0. Load desktop from file          │
│  q. Quit                            │
└─────────────────────────────────────┘
Choice: 3
Folder name  : Projects
[+] Folder created.

Choice: 2
File name    : readme.txt
Content      : Hello, DreamDesk!
[+] File created.

Choice: 1

╔══ Desktop (2 top-level items) ══╗
  [0] + [FOLDER]   Projects
  [1] - [FILE]     readme.txt
╚══════════════════════════════════════╝
```

---

## Features

- Create, rename, and delete files, folders, and shortcuts
- Nest files inside folders (one level of nesting)
- Open / view any item — files show content, shortcuts show target, folders list children
- Save the entire desktop state to a text file
- Load a previously saved desktop from disk
- Duplicate-name detection with informative error messages
- O(1) name lookup via hash map index
- Recursive tree display with indentation

---

## Project Info

| Field | Detail |
|-------|--------|
| Course | CSE-1206 — Object Oriented Programming Lab |
| Institution | Khulna University of Engineering & Technology (KUET) |
| Department | Computer Science and Engineering |
| Semester | 2nd Semester (Year 1, Semester 2) |
| Academic Year | 2023–2024 |

---

## Author

**Farhan Shariar** — Roll: 2307113  
GitHub: [@galib556](https://github.com/galib556)

---

## License

MIT — see [LICENSE](../../LICENSE)

---

<div align="center">
Farhan Shariar · KUET CSE · <a href="mailto:f.s.galib91@gmail.com">f.s.galib91@gmail.com</a>
</div>
