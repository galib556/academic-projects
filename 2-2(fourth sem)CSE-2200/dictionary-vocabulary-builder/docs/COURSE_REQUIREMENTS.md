# Course Requirements Mapping Matrix

This document maps every requirement from Weeks 1–7 of the university laboratory syllabus directly to the code components and documentation in this project.

---

## Detailed Syllabus Topic Mapping

| Week | Topic / Concept | Project Location / Implementation | Explanation / Purpose |
|---|---|---|---|
| **Week 1** | Compilation Process | `docs/COURSE_REQUIREMENTS.md` | Explains C native compilation vs Java bytecode JVM execution |
| **Week 1** | JVM, JDK, JRE | `docs/COURSE_REQUIREMENTS.md` | Describes the Java platform components |
| **Week 1** | Primitive & Reference Data Types | All Model Classes (`com.dictionaryapp.model`) | Uses `int`, `boolean`, `String`, `List<T>` |
| **Week 1** | Control Flow & Methods | `ValidationUtil`, `JsonParser`, `MainController` | Uses `if/else`, `try/catch`, loops, static & instance methods |
| **Week 1** | Core OOP Principles | Domain Models & Services | Encapsulation, constructors, getters/setters, methods |
| **Week 2** | Git Repositories & Commits | Git History (`.git/`) | Sequential feature commits representing real development |
| **Week 2** | Branches & Git Workflow | `docs/GIT_WORKFLOW.md` | Explains local working tree, staging, commit, branching |
| **Week 3** | JavaFX Application Structure | `DictionaryApplication.java` | Extends `Application`, configures `Stage` & `Scene` |
| **Week 3** | JavaFX Controls & Layouts | `MainController.java` | Uses `BorderPane`, `SplitPane`, `VBox`, `HBox`, `ListView`, `TextField`, `Button`, `TextArea`, `ProgressIndicator` |
| **Week 3** | Event Handling | `MainController.java` | Button action events (`setOnAction`), list selection listeners |
| **Week 3** | GUI Design & CSS | `src/main/resources/style.css` | Custom stylesheet for clean typography, cards, badges |
| **Week 4** | Thread & Multithreading | `SearchTask.java`, `MainController.java` | Offloads API search and DB calls off the JavaFX thread |
| **Week 4** | Runnable Interface | `MainController.java` (`loadSavedVocabularyAsync`) | Uses explicit `Runnable` instance for background DB load |
| **Week 4** | Thread Lifecycle | `docs/ARCHITECTURE.md` | Documents `NEW` -> `RUNNABLE` -> `RUNNING` -> `BLOCKED/WAITING` -> `TERMINATED` |
| **Week 4** | Executors / ExecutorService | `MainController.java` | Manages fixed thread pool (`Executors.newFixedThreadPool(4)`) |
| **Week 4** | Synchronization & Shared Resources| `DatabaseManager.java`, `DatabaseService.java` | Uses `ReentrantLock` to protect shared SQLite DB file |
| **Week 5** | Quiz 1 Practical Evidence | Entire Project | Consolidates all Weeks 1–4 foundational topics |
| **Week 6** | Relational Database & SQLite | `DatabaseManager.java`, `data/dictionary.db` | Local persistent relational database |
| **Week 6** | JDBC Connection & PreparedStatement| `DatabaseService.java` | Uses JDBC `Connection`, `PreparedStatement`, `ResultSet` |
| **Week 6** | Table Creation (DDL) | `DatabaseManager.java` | Executes `CREATE TABLE IF NOT EXISTS saved_words` |
| **Week 6** | Database INSERT | `DatabaseService.java` (`saveWord`) | Saves vocabulary record into SQLite |
| **Week 6** | Database SELECT | `DatabaseService.java` (`getAllSavedWords`, `searchSavedWords`) | Queries records with `ORDER BY` and `WHERE LIKE` |
| **Week 6** | Database UPDATE | `DatabaseService.java` (`updateSavedWord`) | Updates existing word record details |
| **Week 6** | Database DELETE | `DatabaseService.java` (`deleteSavedWord`) | Deletes word record by ID |
| **Week 7** | JSON Structure & Parsing | `JsonParser.java` | Uses Google Gson to parse JSON array into domain Java objects |
| **Week 7** | Public API Consumption | `DictionaryService.java` | Communicates with `https://api.dictionaryapi.dev/api/v2/entries/en/{word}` |
| **Week 7** | API Response Handling | `DictionaryService.java` | Handles HTTP 200, 404, 500, network timeouts, invalid inputs |
| **Week 7** | API to Application Format | `MainController.java` | Formats parsed Java objects into visual UI cards and labels |

---

## Theory: C vs. Java Compilation

### C Compilation Process
```
C Source Code (.c)
       ↓  (Preprocessor & Compiler)
Assembly Code (.s)
       ↓  (Assembler)
Machine Object Code (.o / .obj)
       ↓  (Linker)
Native Binary Executable (.exe / a.out)
```
- **Platform Dependent**: C compiles directly into machine code tailored to a specific CPU architecture (x86_64, ARM) and Operating System.

### Java Compilation Process
```
Java Source Code (.java)
       ↓  (javac / Maven)
Java Bytecode (.class)
       ↓
Java Virtual Machine (JVM)
       ↓  (Just-In-Time JIT Compiler)
Native Machine Code Executed by OS / Hardware
```
- **Platform Independent ("Write Once, Run Anywhere")**: Java compiles into intermediate bytecode (`.class`). The platform-specific JVM interprets and JIT-compiles this bytecode into machine code at runtime.

---

## Theory: JDK vs. JRE vs. JVM

1. **JDK (Java Development Kit)**:
   - Contains tools required to **develop** Java software.
   - Includes compiler (`javac`), archiver (`jar`), documentation generator (`javadoc`), and the JRE.
2. **JRE (Java Runtime Environment)**:
   - Contains libraries (`rt.jar`, core modules) and the JVM required to **run** compiled Java applications.
3. **JVM (Java Virtual Machine)**:
   - The abstract runtime execution engine that loads `.class` bytecode, verifies code safety, performs memory allocation/garbage collection, and executes native CPU instructions.
