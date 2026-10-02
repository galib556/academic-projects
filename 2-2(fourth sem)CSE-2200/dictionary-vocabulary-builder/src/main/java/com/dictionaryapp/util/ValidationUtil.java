package com.dictionaryapp.util;

/**
 * Utility class for validating and sanitizing user inputs.
 * Demonstrates static methods, string operations, and basic control flow.
 */
public class ValidationUtil {

    private ValidationUtil() {
        // Private constructor to prevent instantiation of utility class
    }

    /**
     * Sanitizes and normalizes input text.
     * E.g., "   Hello   " -> "hello"
     */
    public static String sanitizeWord(String input) {
        if (input == null) {
            return "";
        }
        return input.trim().toLowerCase();
    }

    /**
     * Checks if a search term is valid (non-null and non-empty after trimming).
     */
    public static boolean isValidSearchTerm(String input) {
        if (input == null) {
            return false;
        }
        String sanitized = input.trim();
        return !sanitized.isEmpty();
    }

    /**
     * Ensures non-null string fallback.
     */
    public static String defaultIfEmpty(String input, String defaultValue) {
        if (input == null || input.trim().isEmpty()) {
            return defaultValue;
        }
        return input.trim();
    }
}
