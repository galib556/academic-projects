package com.dictionaryapp.model;

import java.util.ArrayList;
import java.util.List;

/**
 * Represents a group of definitions categorized by a part of speech (noun, verb, adjective, etc.).
 */
public class Meaning {
    private String partOfSpeech;
    private List<Definition> definitions;
    private List<String> synonyms;
    private List<String> antonyms;

    public Meaning() {
        this.definitions = new ArrayList<>();
        this.synonyms = new ArrayList<>();
        this.antonyms = new ArrayList<>();
    }

    public Meaning(String partOfSpeech, List<Definition> definitions, List<String> synonyms, List<String> antonyms) {
        this.partOfSpeech = partOfSpeech;
        this.definitions = definitions != null ? definitions : new ArrayList<>();
        this.synonyms = synonyms != null ? synonyms : new ArrayList<>();
        this.antonyms = antonyms != null ? antonyms : new ArrayList<>();
    }

    public String getPartOfSpeech() {
        return partOfSpeech;
    }

    public void setPartOfSpeech(String partOfSpeech) {
        this.partOfSpeech = partOfSpeech;
    }

    public List<Definition> getDefinitions() {
        return definitions;
    }

    public void setDefinitions(List<Definition> definitions) {
        this.definitions = definitions;
    }

    public List<String> getSynonyms() {
        return synonyms;
    }

    public void setSynonyms(List<String> synonyms) {
        this.synonyms = synonyms;
    }

    public List<String> getAntonyms() {
        return antonyms;
    }

    public void setAntonyms(List<String> antonyms) {
        this.antonyms = antonyms;
    }
}
