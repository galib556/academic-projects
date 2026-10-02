package com.dictionaryapp.util;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Unit tests for ValidationUtil.
 */
public class ValidationUtilTest {

    @Test
    @DisplayName("Test sanitizeWord normalizes and trims text correctly")
    public void testSanitizeWord() {
        assertEquals("hello", ValidationUtil.sanitizeWord("   Hello   "));
        assertEquals("algorithm", ValidationUtil.sanitizeWord("ALGORITHM"));
        assertEquals("", ValidationUtil.sanitizeWord(null));
        assertEquals("", ValidationUtil.sanitizeWord("   "));
    }

    @Test
    @DisplayName("Test isValidSearchTerm checks for empty or null strings")
    public void testIsValidSearchTerm() {
        assertTrue(ValidationUtil.isValidSearchTerm("word"));
        assertTrue(ValidationUtil.isValidSearchTerm("  computer  "));
        assertFalse(ValidationUtil.isValidSearchTerm(null));
        assertFalse(ValidationUtil.isValidSearchTerm("   "));
        assertFalse(ValidationUtil.isValidSearchTerm(""));
    }

    @Test
    @DisplayName("Test defaultIfEmpty fallback logic")
    public void testDefaultIfEmpty() {
        assertEquals("Default", ValidationUtil.defaultIfEmpty("", "Default"));
        assertEquals("Default", ValidationUtil.defaultIfEmpty(null, "Default"));
        assertEquals("Value", ValidationUtil.defaultIfEmpty("Value", "Default"));
    }
}
