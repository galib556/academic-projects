package com.dictionaryapp.service;

import com.dictionaryapp.database.DatabaseManager;
import com.dictionaryapp.model.SavedWord;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Service providing database CRUD operations for saved vocabulary words.
 * Demonstrates Week 6 syllabus (SQLite, INSERT, SELECT, UPDATE, DELETE, PreparedStatement, ResultSet)
 * and Week 4 thread-safety / synchronization using ReentrantLock.
 */
public class DatabaseService {

    private final ReentrantLock lock = DatabaseManager.getDbLock();

    /**
     * Saves a new word into SQLite database (INSERT operation).
     *
     * @param word SavedWord model instance
     * @return true if successfully saved, false if failed or duplicate
     * @throws SQLException if a database error occurs (e.g. UNIQUE constraint violation)
     */
    public boolean saveWord(SavedWord word) throws SQLException {
        lock.lock();
        try {
            String sql = """
                INSERT INTO saved_words (word, phonetic, part_of_speech, definition, example, synonyms)
                VALUES (?, ?, ?, ?, ?, ?);
                """;

            try (Connection conn = DatabaseManager.getConnection();
                 PreparedStatement pstmt = conn.prepareStatement(sql)) {

                pstmt.setString(1, word.getWord().trim().toLowerCase());
                pstmt.setString(2, word.getPhonetic());
                pstmt.setString(3, word.getPartOfSpeech());
                pstmt.setString(4, word.getDefinition());
                pstmt.setString(5, word.getExample());
                pstmt.setString(6, word.getSynonyms());

                int rowsAffected = pstmt.executeUpdate();
                return rowsAffected > 0;
            }
        } finally {
            lock.unlock();
        }
    }

    /**
     * Retrieves all saved words ordered alphabetically (SELECT operation).
     */
    public List<SavedWord> getAllSavedWords() throws SQLException {
        lock.lock();
        try {
            List<SavedWord> list = new ArrayList<>();
            String sql = "SELECT id, word, phonetic, part_of_speech, definition, example, synonyms, created_at FROM saved_words ORDER BY word ASC;";

            try (Connection conn = DatabaseManager.getConnection();
                 PreparedStatement pstmt = conn.prepareStatement(sql);
                 ResultSet rs = pstmt.executeQuery()) {

                while (rs.next()) {
                    SavedWord word = new SavedWord(
                        rs.getInt("id"),
                        rs.getString("word"),
                        rs.getString("phonetic"),
                        rs.getString("part_of_speech"),
                        rs.getString("definition"),
                        rs.getString("example"),
                        rs.getString("synonyms"),
                        rs.getString("created_at")
                    );
                    list.add(word);
                }
            }
            return list;
        } finally {
            lock.unlock();
        }
    }

    /**
     * Searches saved words by keyword matching word or definition (SELECT operation with WHERE clause).
     */
    public List<SavedWord> searchSavedWords(String keyword) throws SQLException {
        lock.lock();
        try {
            List<SavedWord> list = new ArrayList<>();
            if (keyword == null || keyword.trim().isEmpty()) {
                return getAllSavedWords();
            }

            String sql = """
                SELECT id, word, phonetic, part_of_speech, definition, example, synonyms, created_at 
                FROM saved_words 
                WHERE word LIKE ? OR definition LIKE ? OR synonyms LIKE ? 
                ORDER BY word ASC;
                """;

            String searchTerm = "%" + keyword.trim().toLowerCase() + "%";

            try (Connection conn = DatabaseManager.getConnection();
                 PreparedStatement pstmt = conn.prepareStatement(sql)) {

                pstmt.setString(1, searchTerm);
                pstmt.setString(2, searchTerm);
                pstmt.setString(3, searchTerm);

                try (ResultSet rs = pstmt.executeQuery()) {
                    while (rs.next()) {
                        SavedWord word = new SavedWord(
                            rs.getInt("id"),
                            rs.getString("word"),
                            rs.getString("phonetic"),
                            rs.getString("part_of_speech"),
                            rs.getString("definition"),
                            rs.getString("example"),
                            rs.getString("synonyms"),
                            rs.getString("created_at")
                        );
                        list.add(word);
                    }
                }
            }
            return list;
        } finally {
            lock.unlock();
        }
    }

    /**
     * Updates an existing saved word record (UPDATE operation).
     */
    public boolean updateSavedWord(SavedWord word) throws SQLException {
        lock.lock();
        try {
            String sql = """
                UPDATE saved_words
                SET phonetic = ?, part_of_speech = ?, definition = ?, example = ?, synonyms = ?
                WHERE id = ?;
                """;

            try (Connection conn = DatabaseManager.getConnection();
                 PreparedStatement pstmt = conn.prepareStatement(sql)) {

                pstmt.setString(1, word.getPhonetic());
                pstmt.setString(2, word.getPartOfSpeech());
                pstmt.setString(3, word.getDefinition());
                pstmt.setString(4, word.getExample());
                pstmt.setString(5, word.getSynonyms());
                pstmt.setInt(6, word.getId());

                int rows = pstmt.executeUpdate();
                return rows > 0;
            }
        } finally {
            lock.unlock();
        }
    }

    /**
     * Deletes a saved word by ID (DELETE operation).
     */
    public boolean deleteSavedWord(int id) throws SQLException {
        lock.lock();
        try {
            String sql = "DELETE FROM saved_words WHERE id = ?;";

            try (Connection conn = DatabaseManager.getConnection();
                 PreparedStatement pstmt = conn.prepareStatement(sql)) {

                pstmt.setInt(1, id);
                int rows = pstmt.executeUpdate();
                return rows > 0;
            }
        } finally {
            lock.unlock();
        }
    }

    /**
     * Checks if a word is already saved in the database.
     */
    public boolean isWordSaved(String word) throws SQLException {
        lock.lock();
        try {
            String sql = "SELECT COUNT(*) FROM saved_words WHERE LOWER(word) = ?;";
            try (Connection conn = DatabaseManager.getConnection();
                 PreparedStatement pstmt = conn.prepareStatement(sql)) {

                pstmt.setString(1, word.trim().toLowerCase());
                try (ResultSet rs = pstmt.executeQuery()) {
                    if (rs.next()) {
                        return rs.getInt(1) > 0;
                    }
                }
            }
            return false;
        } finally {
            lock.unlock();
        }
    }
}
