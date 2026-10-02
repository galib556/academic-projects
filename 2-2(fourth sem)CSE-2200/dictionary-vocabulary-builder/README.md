# Dictionary & Vocabulary Builder

A desktop English vocabulary learning application built with **Java**, **JavaFX**, **SQLite**, **JSON API**, and **Java Multithreading**. Created as a practical laboratory project fulfilling Weeks 1–7 of the university Java software engineering syllabus.

---

## Features

- **Asynchronous Word Search**: Query English definitions, phonetics, parts of speech, examples, and synonyms from the free public Dictionary API.
- **Non-Blocking GUI**: Multithreaded execution using `ExecutorService` and JavaFX `Task` prevents the GUI from freezing during network requests.
- **SQLite Vocabulary Storage**: Save words to a local relational database (`data/dictionary.db`) using JDBC.
- **Full Database CRUD**:
  - **Create / Insert**: Save new vocabulary words with duplicate checks.
  - **Read / Select**: Display saved words alphabetically or search by keyword.
  - **Update**: Edit saved definitions, example sentences, and synonyms.
  - **Delete**: Remove vocabulary words with confirmation prompts.
- **IntelliJ IDEA & Maven Compatible**: Pre-configured Maven project buildable directly in IntelliJ IDEA or command line.

---

## Technologies Used

- **Java**: Java 21 LTS
- **GUI Framework**: JavaFX 21 (`javafx-controls`, `javafx-graphics`)
- **Database**: SQLite & SQLite JDBC Driver (`3.45.1.0`)
- **JSON Parser**: Google Gson (`2.10.1`)
- **HTTP Client**: `java.net.http.HttpClient`
- **Concurrency**: `java.util.concurrent.ExecutorService`, JavaFX `Task`, `ReentrantLock`
- **Build Tool**: Apache Maven (`3.9.9`)
- **Testing**: JUnit 5 (`5.10.2`)
- **Version Control**: Git & GitHub

---

## Project Structure

```
dictionary-vocabulary-builder/
├── pom.xml                               # Maven project dependencies & plugins
├── README.md                             # Project overview & documentation
├── .gitignore                            # Git ignore rules
├── LICENSE                               # MIT License
├── docs/                                 # Detailed course topic documentation
│   ├── COURSE_REQUIREMENTS.md            # Mapping matrix for Weeks 1–7 syllabus
│   ├── ARCHITECTURE.md                   # Application architecture & design patterns
│   ├── GIT_WORKFLOW.md                   # Git history & version control principles
│   ├── DATABASE.md                       # SQLite schema, queries & JDBC details
│   ├── JSON_API.md                       # API consumption, HttpClient & Gson parsing
│   └── FINAL_VERIFICATION.md             # Verification test results checklist
├── src/
│   ├── main/
│   │   ├── java/com/dictionaryapp/
│   │   │   ├── Main.java                 # Entry point launcher (IntelliJ compatible)
│   │   │   ├── DictionaryApplication.java# JavaFX Application class
│   │   │   ├── model/                    # Domain model classes
│   │   │   ├── service/                  # API, Database CRUD, and SearchTask services
│   │   │   ├── database/                 # SQLite DatabaseManager with thread lock
│   │   │   ├── controller/               # MainController for JavaFX UI & events
│   │   │   └── util/                     # JsonParser and ValidationUtil
│   │   └── resources/
│   │       └── style.css                 # Custom CSS styling
│   └── test/                             # Automated JUnit 5 unit tests
└── data/                                 # SQLite database folder (data/dictionary.db)
```

---

## How to Run in IntelliJ IDEA

1. Open **IntelliJ IDEA**.
2. Select **File > Open** and choose the `dictionary-vocabulary-builder` directory.
3. IntelliJ will automatically detect `pom.xml` and import dependencies.
4. Navigate to `src/main/java/com/dictionaryapp/Main.java`.
5. Right-click `Main.java` and select **Run 'Main.main()'**.

---

## Running via Maven Command Line

### 1. Execute Unit Tests
```bash
mvn clean test
```

### 2. Package Executable JAR
```bash
mvn clean package
```
Generates shaded executable JAR at `target/dictionary-vocabulary-builder-1.0.0.jar`.

### 3. Launch Application via Maven
```bash
mvn javafx:run
```

---

## Course Topic Mapping (Weeks 1–7)

| Week | Laboratory Topic | Application Implementation |
|---|---|---|
| **Week 1** | Java Syntax & OOP | Primitive/reference types, encapsulation, classes, inheritance, collections |
| **Week 2** | Git Version Control | Git workflow, commits, local branches, .gitignore |
| **Week 3** | Desktop GUI (JavaFX) | Stage, Scene, BorderPane, SplitPane, TabPane, ListView, Controls, CSS |
| **Week 4** | Multithreading & Concurrency | `ExecutorService`, JavaFX `Task`, `Runnable`, `ReentrantLock` synchronization |
| **Week 5** | Revision & Knowledge Check | Comprehensive application integration |
| **Week 6** | SQLite & Relational DB | JDBC, SQLite connection, CREATE TABLE, INSERT, SELECT, UPDATE, DELETE |
| **Week 7** | JSON & Public API | `java.net.http.HttpClient`, Gson JSON parsing into domain objects |

---

## Author

**Farhan Shariar**  
Department of Computer Science and Engineering, Khulna University of Engineering & Technology (KUET)

**Course:** CSE-2200 — Advanced Programming Laboratory Project (2-2, 4th Semester), 2026
