package com.dictionaryapp;

/**
 * Entry point for the Dictionary & Vocabulary Builder application.
 * 
 * NOTE FOR INTELLIJ IDEA USERS:
 * This separate launcher class ensures that IntelliJ IDEA can execute the JavaFX application
 * directly without throwing runtime JavaFX module layer errors.
 */
public class Main {
    public static void main(String[] args) {
        DictionaryApplication.launchApp(args);
    }
}
