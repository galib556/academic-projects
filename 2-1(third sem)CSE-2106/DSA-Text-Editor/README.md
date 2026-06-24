# 📝 DSA Text Editor

A fully functional **console-based text editor** built from scratch in **C++11**, implementing six core data structures and multiple algorithms — with zero STL containers. Every structure (Stack, Queue, Linked List, BST, Graph) is hand-written.

> **Course:** CSE-2106 — Data Structures & Algorithms  
> **Semester:** 2-1 (3rd Semester)  
> **Team:** Md. Wasif Rahman (2307101) · Md. Rashed Shariar (2307109) · Farhan Shahriar Galib (2307113)

---

## 🏗️ Architecture & Data Structures

| Module | Data Structure | Purpose |
|---|---|---|
| `LineBuffer` | Doubly Linked List | Core text buffer — O(1) insert/delete at known node |
| `UndoRedoManager` | Custom Stack (dynamic array) | Undo/Redo command history |
| `HistNode` / Graph | Adjacency List Graph | Edit history as a traversable graph |
| `BST` | Binary Search Tree | Live word frequency index |
| `Queue<T>` | Linked-node Queue | BFS traversal of history graph |
| `ExpressionEvaluator` | Stack (Shunting-Yard) | Infix → Postfix/Prefix conversion & evaluation |

---

## ⚙️ Algorithms Implemented

- **6 Sorting algorithms** — Bubble, Selection, Insertion, Merge, Quick, Heap Sort (on line arrays)
- **Shunting-Yard Algorithm** — Infix to Postfix & Prefix conversion
- **Binary Search** — Fast word lookup in sort-then-search find-and-replace
- **BFS & DFS** — Graph traversal for edit history visualization
- **BST Inorder Traversal** — Alphabetical word frequency listing

---

## 🚀 Getting Started

### Prerequisites
- A C++11 compatible compiler (`g++`, `clang++`, MSVC)

### Compile & Run

```bash
# Linux / macOS
g++ -std=c++11 -O2 -o text_editor text_editor.cpp
./text_editor

# Windows (MinGW)
g++ -std=c++11 -O2 -o text_editor.exe text_editor.cpp
text_editor.exe
```

---

## 🖥️ Commands

```
============================================================
             DSA TEXT EDITOR — COMMANDS
============================================================
  print                       Show buffer with line numbers
  stats                       Buffer statistics
  insert <line> <text>        Insert line at position (1-based)
  delete <line>               Delete line
  replace <line> <text>       Replace line content
  searchreplace <old> <new>   Replace word across buffer (Binary Search)
  undo / redo                 Undo / redo last operation
  history                     Visualize edit history via BFS & DFS
  find <word>                 Search word frequency (BST index)
  topwords <n>                Top N most frequent words
  sort <algo>                 Sort lines lexicographically
                              (bubble/selection/insertion/merge/quick/heap)
  expr <expression>           Evaluate arithmetic expression
  save [filename]             Save buffer to file
  load <filename>             Load .txt file into buffer
  quit                        Exit editor
============================================================
```

### Example Session

```
> insert 1 Hello World
  Inserted line 1

> insert 2 Data Structures are fun
  Inserted line 2

> expr 3 * (2 + 4) ^ 2
  Prefix  : * 3 ^ + 2 4 2
  Postfix : 3 2 4 + 2 ^ *
  Result  : 108

> sort merge
  Sorted lines using merge sort.

> searchreplace fun amazing
  Replaced 'fun' with 'amazing' across 1 lines.

> history
--- Edit History (BFS Traversal) ---
  State 0: INIT (Empty Buffer)
  State 1: insert at line 1  <-- CURRENT STATE
  ...

> undo
  Undone: replace at line 2

> save myfile.txt
  Saved to 'myfile.txt'
```

---

## 📁 Project Structure

```
dsa-text-editor/
├── text_editor.cpp     # Complete single-file source (all modules)
└── README.md
```

The project is intentionally a single self-contained file to reflect the academic scope — all sections are clearly delimited with comments:

```
SECTION 0 — Custom Utilities    (Swap, Pair, Stack<T>, Queue<T>)
SECTION 1 — LineBuffer          (Doubly Linked List text buffer)
SECTION 2 — Undo/Redo + Graph   (Stack-based commands, Adjacency List history)
SECTION 3 — Expression Evaluator(Shunting-Yard, Postfix evaluator)
SECTION 4 — Sorting Algorithms  (6 sorting methods)
SECTION 5 — BST Word Index      (Insert, Search, Inorder, TopN)
SECTION 6 — Binary Search F&R   (MergeSort + BinarySearch find-and-replace)
TEXT EDITOR SHELL               (TextEditor class + main())
```

---

## 🧠 Key Design Decisions

- **No STL containers** — `std::vector`, `std::stack`, `std::map` etc. are intentionally avoided. All structures are built from scratch to demonstrate manual memory management.
- **Single-file architecture** — All modules in one `.cpp` for simplicity in an academic context.
- **Template generics** — `Stack<T>` and `Queue<T>` are fully templated and reused across the codebase (holds `char`, `int`, `double`, `Command`).
- **Command pattern** — Every edit is encapsulated in a `Command` struct, enabling clean undo/redo and history graph recording.

---

## 📚 References

- *Data Structures and Algorithms with C* — Seymour Lipschutz
- [GeeksForGeeks — Shunting-Yard Algorithm](https://www.geeksforgeeks.org/expression-evaluation/)
- [GeeksForGeeks — Binary Search Tree](https://www.geeksforgeeks.org/binary-search-tree-data-structure/)
- [GeeksForGeeks — BFS and DFS Graph Traversal](https://www.geeksforgeeks.org/graph-data-structure-and-algorithms/)
- [VisuAlgo — Sorting Algorithms](https://visualgo.net/en/sorting)
- [Khan Academy — Binary Search](https://www.khanacademy.org/computing/computer-science/algorithms/binary-search/a/binary-search)
- [cppreference — `<fstream>`](https://en.cppreference.com/w/cpp/io/basic_fstream)
