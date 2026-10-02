package com.dictionaryapp.util;

import com.dictionaryapp.model.DictionaryEntry;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Unit tests for JsonParser verifying Week 7 JSON parsing into Java domain objects.
 */
public class JsonParserTest {

    @Test
    @DisplayName("Test parsing valid Dictionary API JSON payload")
    public void testParseValidJson() {
        String mockJson = """
            [
              {
                "word": "hello",
                "phonetic": "/həˈloʊ/",
                "meanings": [
                  {
                    "partOfSpeech": "exclamation",
                    "definitions": [
                      {
                        "definition": "Used as a greeting or to begin a phone conversation.",
                        "example": "hello there!",
                        "synonyms": ["greeting"]
                      }
                    ]
                  }
                ]
              }
            ]
            """;

        List<DictionaryEntry> entries = JsonParser.parseDictionaryEntries(mockJson);
        assertNotNull(entries);
        assertEquals(1, entries.size());

        DictionaryEntry entry = entries.get(0);
        assertEquals("hello", entry.getWord());
        assertEquals("/həˈloʊ/", entry.getPhonetic());
        assertEquals("exclamation", entry.getPrimaryPartOfSpeech());
        assertEquals("Used as a greeting or to begin a phone conversation.", entry.getPrimaryDefinition());
        assertEquals("hello there!", entry.getPrimaryExample());
    }

    @Test
    @DisplayName("Test parsing throws IllegalArgumentException on empty response")
    public void testParseEmptyJsonThrowsException() {
        assertThrows(IllegalArgumentException.class, () -> JsonParser.parseDictionaryEntries(""));
        assertThrows(IllegalArgumentException.class, () -> JsonParser.parseDictionaryEntries(null));
    }
}
