package com.dictionaryapp.service;

import com.dictionaryapp.database.DatabaseManager;
import com.dictionaryapp.model.SavedWord;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.sql.SQLException;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Unit tests for DatabaseService testing SQLite CRUD operations (Week 6).
 */
public class DatabaseServiceTest {

    private DatabaseService databaseService;
    private File tempDbFile;

    @BeforeEach
    public void setUp() throws Exception {
        tempDbFile = File.createTempFile("test_dictionary_", ".db");
        DatabaseManager.setCustomDbPath(tempDbFile.getAbsolutePath());
        DatabaseManager.initializeDatabase();
        databaseService = new DatabaseService();
    }

    @AfterEach
    public void tearDown() {
        if (tempDbFile != null && tempDbFile.exists()) {
            tempDbFile.delete();
        }
    }

    @Test
    @DisplayName("Test CREATE/INSERT and READ/SELECT database operations")
    public void testSaveAndGetWords() throws SQLException {
        SavedWord word = new SavedWord("algorithm", "/ˈælɡəˌrɪðəm/", "noun", 
            "A process or set of rules to be followed in calculations.", 
            "Sorting algorithms are fundamental.", "procedure, method");

        boolean saved = databaseService.saveWord(word);
        assertTrue(saved, "Word should be successfully inserted");

        assertTrue(databaseService.isWordSaved("algorithm"), "isWordSaved should return true");

        List<SavedWord> list = databaseService.getAllSavedWords();
        assertEquals(1, list.size());
        assertEquals("algorithm", list.get(0).getWord());
        assertEquals("noun", list.get(0).getPartOfSpeech());
    }

    @Test
    @DisplayName("Test UPDATE database operation")
    public void testUpdateWord() throws SQLException {
        SavedWord word = new SavedWord("computer", "/kəmˈpjuːtər/", "noun", 
            "An electronic device.", "I work on a computer.", "machine");
        databaseService.saveWord(word);

        List<SavedWord> list = databaseService.getAllSavedWords();
        assertFalse(list.isEmpty());
        SavedWord saved = list.get(0);

        saved.setDefinition("An electronic device for storing and processing data.");
        boolean updated = databaseService.updateSavedWord(saved);
        assertTrue(updated, "Update operation should succeed");

        List<SavedWord> updatedList = databaseService.getAllSavedWords();
        assertEquals("An electronic device for storing and processing data.", updatedList.get(0).getDefinition());
    }

    @Test
    @DisplayName("Test DELETE database operation")
    public void testDeleteWord() throws SQLException {
        SavedWord word = new SavedWord("syntax", "/ˈsɪntæks/", "noun", 
            "The arrangement of words and phrases to create well-formed sentences.", 
            "Java syntax is strict.", "structure");
        databaseService.saveWord(word);

        List<SavedWord> list = databaseService.getAllSavedWords();
        assertEquals(1, list.size());

        boolean deleted = databaseService.deleteSavedWord(list.get(0).getId());
        assertTrue(deleted, "Delete operation should succeed");

        List<SavedWord> remaining = databaseService.getAllSavedWords();
        assertTrue(remaining.isEmpty(), "Database should be empty after deletion");
    }
}
