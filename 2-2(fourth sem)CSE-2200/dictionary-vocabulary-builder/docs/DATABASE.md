# Relational Database & SQLite Documentation

## Overview

The application utilizes **SQLite**, a self-contained, serverless relational database engine (Week 6 Syllabus). Database transactions are handled via Java Database Connectivity (JDBC).

---

## Relational Concepts

- **Database**: A structured collection of data stored in `data/dictionary.db`.
- **Table**: `saved_words` organizes vocabulary records in structured rows and columns.
- **Primary Key**: `id INTEGER PRIMARY KEY AUTOINCREMENT` uniquely identifies every saved row.
- **UNIQUE Constraint**: `word TEXT NOT NULL UNIQUE` prevents saving duplicate words.

---

## SQLite Database Schema

```sql
CREATE TABLE IF NOT EXISTS saved_words (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    word TEXT NOT NULL UNIQUE,
    phonetic TEXT,
    part_of_speech TEXT,
    definition TEXT,
    example TEXT,
    synonyms TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

---

## SQL CRUD Operations Used in Project

### 1. INSERT (Create)
Saves a new word entry into the database.
```sql
INSERT INTO saved_words (word, phonetic, part_of_speech, definition, example, synonyms)
VALUES (?, ?, ?, ?, ?, ?);
```

### 2. SELECT (Read)
Retrieves all saved vocabulary records ordered alphabetically.
```sql
SELECT id, word, phonetic, part_of_speech, definition, example, synonyms, created_at 
FROM saved_words 
ORDER BY word ASC;
```

Search/Filter query using wildcard pattern matching:
```sql
SELECT id, word, phonetic, part_of_speech, definition, example, synonyms, created_at 
FROM saved_words 
WHERE word LIKE ? OR definition LIKE ? OR synonyms LIKE ? 
ORDER BY word ASC;
```

### 3. UPDATE (Edit)
Modifies existing vocabulary record details.
```sql
UPDATE saved_words
SET phonetic = ?, part_of_speech = ?, definition = ?, example = ?, synonyms = ?
WHERE id = ?;
```

### 4. DELETE (Remove)
Removes a word record by primary key ID.
```sql
DELETE FROM saved_words WHERE id = ?;
```

---

## JDBC Security Best Practices

1. **PreparedStatement**: All queries use parameterized `PreparedStatement` instances rather than unsafe string concatenation to completely eliminate **SQL Injection** security risks.
2. **Try-With-Resources**: Automatically closes `Connection`, `PreparedStatement`, and `ResultSet` objects, eliminating memory and file handle leaks.
3. **Thread Synchronization**: Uses `ReentrantLock` in `DatabaseManager` to guarantee thread safety across multithreaded operations.
