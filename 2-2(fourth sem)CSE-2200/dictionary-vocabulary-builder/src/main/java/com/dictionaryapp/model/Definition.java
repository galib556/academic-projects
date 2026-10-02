package com.dictionaryapp.model;

import java.util.ArrayList;
import java.util.List;

/**
 * Represents an individual word definition, example, and associated synonyms/antonyms.
 */
public class Definition {
    private String definition;
    private String example;
    private List<String> synonyms;
    private List<String> antonyms;

    public Definition() {
        this.synonyms = new ArrayList<>();
        this.antonyms = new ArrayList<>();
    }

    public Definition(String definition, String example, List<String> synonyms, List<String> antonyms) {
        this.definition = definition;
        this.example = example;
        this.synonyms = synonyms != null ? synonyms : new ArrayList<>();
        this.antonyms = antonyms != null ? antonyms : new ArrayList<>();
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

    @Override
    public String toString() {
        return definition != null ? definition : "";
    }
}
