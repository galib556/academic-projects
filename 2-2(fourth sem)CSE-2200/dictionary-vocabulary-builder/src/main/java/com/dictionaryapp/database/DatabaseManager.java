package com.dictionaryapp.database;

import java.io.File;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.concurrent.locks.ReentrantLock;

/**
 * DatabaseManager handles SQLite database connection initialization and table creation.
 * Demonstrates Week 4 (Synchronization & Shared Resources) and Week 6 (SQLite Database Integration).
 * Uses a ReentrantLock to ensure thread-safe access to the shared SQLite database file.
 */
public class DatabaseManager {

    private static final String DEFAULT_DB_DIR = "data";
    private static final String DEFAULT_DB_PATH = "data/dictionary.db";
    private static String dbUrl = "jdbc:sqlite:" + DEFAULT_DB_PATH;

    // Shared resource lock for thread safety (Week 4: Synchronization)
    private static final ReentrantLock dbLock = new ReentrantLock();

    private DatabaseManager() {
        // Utility class
    }

    /**
     * Sets custom database path (useful for unit testing with temporary databases).
     */
    public static void setCustomDbPath(String customPath) {
        dbLock.lock();
        try {
            dbUrl = "jdbc:sqlite:" + customPath;
        } finally {
            dbLock.unlock();
        }
    }

    /**
     * Returns a thread-safe connection to the SQLite database.
     */
    public static Connection getConnection() throws SQLException {
        ensureDirectoryExists();
        return DriverManager.getConnection(dbUrl);
    }

    /**
     * Obtains the shared database execution lock.
     */
    public static ReentrantLock getDbLock() {
        return dbLock;
    }

    /**
     * Ensures the database folder exists.
     */
    private static void ensureDirectoryExists() {
        File dir = new File(DEFAULT_DB_DIR);
        if (!dir.exists()) {
            dir.mkdirs();
        }
    }

    /**
     * Initializes the SQLite database and creates the saved_words table if it does not exist.
     * Demonstrates Week 6 DDL query (CREATE TABLE).
     */
    public static void initializeDatabase() {
        dbLock.lock();
        try {
            ensureDirectoryExists();
            String createTableSQL = """
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
                """;

            try (Connection conn = getConnection();
                 Statement stmt = conn.createStatement()) {
                stmt.execute(createTableSQL);
            } catch (SQLException e) {
                System.err.println("[DatabaseManager] Database initialization failed: " + e.getMessage());
                e.printStackTrace();
            }
        } finally {
            dbLock.unlock();
        }
    }
}
