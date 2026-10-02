package com.dictionaryapp.service;

import com.dictionaryapp.model.DictionaryEntry;
import javafx.concurrent.Task;

import java.util.List;

/**
 * Background Task for searching word definitions asynchronously.
 * Demonstrates Week 4 (Multithreading, Task, Concurrency API) & JavaFX background execution.
 * Inheriting from javafx.concurrent.Task ensures thread-safe callbacks to the JavaFX Application Thread.
 */
public class SearchTask extends Task<List<DictionaryEntry>> {

    private final DictionaryService dictionaryService;
    private final String word;

    public SearchTask(DictionaryService dictionaryService, String word) {
        this.dictionaryService = dictionaryService;
        this.word = word;
    }

    @Override
    protected List<DictionaryEntry> call() throws Exception {
        updateMessage("Searching dictionary API for '" + word + "'...");
        updateProgress(-1, 1); // Indeterminate progress
        return dictionaryService.fetchWordDefinition(word);
    }
}
