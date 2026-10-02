# Git & Version Control Workflow Guide

## Overview

Version control is an integral component of software engineering (Week 2 Syllabus). This project was created and maintained using **Git**.

---

## Core Git Concepts

1. **Working Directory**: The local folder containing project source files, `pom.xml`, documentation, and assets.
2. **Staging Area (`git add`)**: Index where changes are prepared before recording them into the repository history.
3. **Repository (`git commit`)**: Persistent commit graph storing incremental snapshots of project state.
4. **Branch (`git branch`)**: Independent line of development allowing features to be created safely.
5. **Remote Repository (`git push`)**: Remote server (e.g., GitHub) hosting the shared repository.

---

## Git Workflow Cycle

```
Working Directory  ───(git add)───>  Staging Area  ───(git commit)───>  Local Repository  ───(git push)───>  GitHub Remote
```

---

## Project Commit History

```bash
597a616 feat: Implement responsive JavaFX UI, tab layout, event handling, CSS styling, and ExecutorService thread pool
eb16ca8 test: Add automated unit tests for JsonParser, ValidationUtil, and SQLite DatabaseService
ef2f420 feat: Implement SQLite database manager, CRUD service, Dictionary API client, and background SearchTask
5e3b18e feat: Add JsonParser and ValidationUtil for API response mapping and input sanitization
7d003a8 feat: Add core domain model classes (DictionaryEntry, Meaning, Definition, Phonetic, SavedWord)
e1232a2 feat: Initial Maven project setup with Java 21, JavaFX, SQLite, and Gson dependencies
```

---

## How to Connect and Push to Your GitHub Account

Follow these steps to push this local project to your personal GitHub account:

### Step 1: Create a GitHub Repository
1. Log in to [GitHub](https://github.com).
2. Click **New Repository**.
3. Name the repository: `dictionary-vocabulary-builder`.
4. Choose **Public** or **Private**.
5. Do **NOT** initialize with a README (the project already contains a complete README.md).
6. Click **Create repository**.

### Step 2: Add Remote and Push Local Repository
Open terminal/PowerShell inside the project directory and run:

```bash
# Set your main branch
git branch -M main

# Add your GitHub repository URL (replace YOUR_USERNAME)
git remote add origin https://github.com/YOUR_USERNAME/dictionary-vocabulary-builder.git

# Push all commits to GitHub
git push -u origin main
```
