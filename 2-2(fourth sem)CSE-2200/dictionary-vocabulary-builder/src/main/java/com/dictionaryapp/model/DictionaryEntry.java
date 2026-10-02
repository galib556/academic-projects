package com.dictionaryapp.model;

import java.util.ArrayList;
import java.util.List;

/**
 * Main domain object representing a dictionary entry returned from the Dictionary API.
 * Demonstrates Object-Oriented Programming (OOP) concepts: encapsulation, composite objects, and Collections.
 */
public class DictionaryEntry {
    private String word;
    private String phonetic;
    private List<Phonetic> phonetics;
    private List<Meaning> meanings;
    private List<String> sourceUrls;

    public DictionaryEntry() {
        this.phonetics = new ArrayList<>();
        this.meanings = new ArrayList<>();
        this.sourceUrls = new ArrayList<>();
    }

    public String getWord() {
        return word;
    }

    public void setWord(String word) {
        this.word = word;
    }

    public String getPhonetic() {
        if (phonetic != null && !phonetic.isEmpty()) {
            return phonetic;
        }
        if (phonetics != null) {
            for (Phonetic p : phonetics) {
                if (p.getText() != null && !p.getText().isEmpty()) {
                    return p.getText();
                }
            }
        }
        return "N/A";
    }

    public void setPhonetic(String phonetic) {
        this.phonetic = phonetic;
    }

    public List<Phonetic> getPhonetics() {
        return phonetics;
    }

    public void setPhonetics(List<Phonetic> phonetics) {
        this.phonetics = phonetics;
    }

    public List<Meaning> getMeanings() {
        return meanings;
    }

    public void setMeanings(List<Meaning> meanings) {
        this.meanings = meanings;
    }

    public List<String> getSourceUrls() {
        return sourceUrls;
    }

    public void setSourceUrls(List<String> sourceUrls) {
        this.sourceUrls = sourceUrls;
    }

    /**
     * Helper method to extract primary part of speech.
     */
    public String getPrimaryPartOfSpeech() {
        if (meanings != null && !meanings.isEmpty()) {
            return meanings.get(0).getPartOfSpeech();
        }
        return "N/A";
    }

    /**
     * Helper method to extract primary definition text.
     */
    public String getPrimaryDefinition() {
        if (meanings != null) {
            for (Meaning meaning : meanings) {
                if (meaning.getDefinitions() != null && !meaning.getDefinitions().isEmpty()) {
                    return meaning.getDefinitions().get(0).getDefinition();
                }
            }
        }
        return "No definition available.";
    }

    /**
     * Helper method to extract primary example sentence if available.
     */
    public String getPrimaryExample() {
        if (meanings != null) {
            for (Meaning meaning : meanings) {
                if (meaning.getDefinitions() != null) {
                    for (Definition def : meaning.getDefinitions()) {
                        if (def.getExample() != null && !def.getExample().trim().isEmpty()) {
                            return def.getExample();
                        }
                    }
                }
            }
        }
        return "No example available.";
    }

    /**
     * Helper method to aggregate all unique synonyms from meanings and definitions.
     */
    public String getAggregatedSynonyms() {
        List<String> synList = new ArrayList<>();
        if (meanings != null) {
            for (Meaning meaning : meanings) {
                if (meaning.getSynonyms() != null) {
                    for (String s : meaning.getSynonyms()) {
                        if (!synList.contains(s)) synList.add(s);
                    }
                }
                if (meaning.getDefinitions() != null) {
                    for (Definition def : meaning.getDefinitions()) {
                        if (def.getSynonyms() != null) {
                            for (String s : def.getSynonyms()) {
                                if (!synList.contains(s)) synList.add(s);
                            }
                        }
                    }
                }
            }
        }
        if (synList.isEmpty()) {
            return "None";
        }
        return String.join(", ", synList);
    }
}
