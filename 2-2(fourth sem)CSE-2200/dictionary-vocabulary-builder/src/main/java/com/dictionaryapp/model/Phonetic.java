package com.dictionaryapp.model;

/**
 * Represents phonetic pronunciation data returned by the Dictionary API.
 * Demonstrates OOP encapsulation with private fields, constructors, and getters/setters.
 */
public class Phonetic {
    private String text;
    private String audio;

    public Phonetic() {
    }

    public Phonetic(String text, String audio) {
        this.text = text;
        this.audio = audio;
    }

    public String getText() {
        return text;
    }

    public void setText(String text) {
        this.text = text;
    }

    public String getAudio() {
        return audio;
    }

    public void setAudio(String audio) {
        this.audio = audio;
    }

    @Override
    public String toString() {
        return text != null ? text : "";
    }
}
