package com.dictionaryapp.model;

/**
 * Represents a word record saved in the SQLite database.
 * Encapsulates record identity, vocabulary content, and metadata.
 */
public class SavedWord {
    private int id;
    private String word;
    private String phonetic;
    private String partOfSpeech;
    private String definition;
    private String example;
    private String synonyms;
    private String createdAt;

    public SavedWord() {
    }

    public SavedWord(String word, String phonetic, String partOfSpeech, String definition, String example, String synonyms) {
        this.word = word;
        this.phonetic = phonetic;
        this.partOfSpeech = partOfSpeech;
        this.definition = definition;
        this.example = example;
        this.synonyms = synonyms;
    }

    public SavedWord(int id, String word, String phonetic, String partOfSpeech, String definition, String example, String synonyms, String createdAt) {
        this.id = id;
        this.word = word;
        this.phonetic = phonetic;
        this.partOfSpeech = partOfSpeech;
        this.definition = definition;
        this.example = example;
        this.synonyms = synonyms;
        this.createdAt = createdAt;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getWord() {
        return word;
    }

    public void setWord(String word) {
        this.word = word;
    }

    public String getPhonetic() {
        return phonetic;
    }

    public void setPhonetic(String phonetic) {
        this.phonetic = phonetic;
    }

    public String getPartOfSpeech() {
        return partOfSpeech;
    }

    public void setPartOfSpeech(String partOfSpeech) {
        this.partOfSpeech = partOfSpeech;
    }

    public String getDefinition() {
        return definition;
    }

    public void setDefinition(String definition) {
        this.definition = definition;
    }

    public String getExample() {
        return example;
    }

    public void setExample(String example) {
        this.example = example;
    }

    public String getSynonyms() {
        return synonyms;
    }

    public void setSynonyms(String synonyms) {
        this.synonyms = synonyms;
    }

    public String getCreatedAt() {
        return createdAt;
    }

    public void setCreatedAt(String createdAt) {
        this.createdAt = createdAt;
    }

    @Override
    public String toString() {
        return word;
    }
}
