# System Architecture & Design Specification

## Overview

The **Dictionary & Vocabulary Builder** is designed using a multi-tiered architecture that separates presentation logic (JavaFX UI & Controller), business services (API search & database CRUD), domain models, and data storage (SQLite).

---

## Component Architecture

```
                       ┌───────────────────────────────┐
                       │    JavaFX User Interface      │
                       │ (DictionaryApplication & CSS) │
                       └───────────────┬───────────────┘
                                       │
                                       ▼
                       ┌───────────────────────────────┐
                       │        MainController         │
                       └───────┬───────────────┬───────┘
                               │               │
            (Asynchronous Task)│               │(Asynchronous Task)
                               ▼               ▼
                 ┌──────────────────┐    ┌──────────────────┐
                 │DictionaryService │    │ DatabaseService  │
                 └────────┬─────────┘    └────────┬─────────┘
                          │                       │
                          ▼                       ▼
                 ┌──────────────────┐    ┌──────────────────┐
                 │ HttpClient API   │    │  SQLite Database │
                 └────────┬─────────┘    └──────────────────┘
                          │
                          ▼
                 ┌──────────────────┐
                 │    JsonParser    │
                 └────────┬─────────┘
                          │
                          ▼
                 ┌──────────────────┐
                 │   Java Objects   │
                 └──────────────────┘
```

---

## Multithreading & Thread Lifecycle Design

To keep the JavaFX user interface smooth and responsive (preventing "not responding" application freezes), network HTTP requests and SQLite disk operations are executed off the JavaFX Application Thread.

### Thread Lifecycle Workflow
```
   NEW  ───>  RUNNABLE  ───>  RUNNING  ───>  WAITING / BLOCKED  ───>  TERMINATED
(Task Init) (In Executor Pool) (Executing)   (I/O Network / DB)      (Task Complete)
```

1. **JavaFX Application Thread**:
   - Manages UI controls, scene graph rendering, user clicks, and keypresses.
   - UI controls are modified **exclusively** on this thread using `Platform.runLater()` or Task `setOnSucceeded` callbacks.

2. **ExecutorService Thread Pool**:
   - A `FixedThreadPool` with 4 worker threads manages background execution.
   - Offloads blocking operations (`DictionaryService.fetchWordDefinition`, `DatabaseService.saveWord`, `getAllSavedWords`).

3. **ReentrantLock Database Synchronization**:
   - `DatabaseManager.getDbLock()` guards all SQLite database transactions.
   - Prevents race conditions or database file lock errors when multiple background threads access SQLite concurrently.
