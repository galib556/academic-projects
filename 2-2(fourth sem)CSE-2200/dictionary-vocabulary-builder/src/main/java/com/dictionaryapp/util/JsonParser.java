package com.dictionaryapp.util;

import com.dictionaryapp.model.Definition;
import com.dictionaryapp.model.DictionaryEntry;
import com.dictionaryapp.model.Meaning;
import com.dictionaryapp.model.Phonetic;
import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

import java.util.ArrayList;
import java.util.List;

/**
 * Utility class to parse JSON responses from the Primary and Fallback Dictionary APIs into Java domain objects.
 * Demonstrates Week 7 requirements: JSON parsing, error handling, and object conversion.
 */
public class JsonParser {

    private static final Gson gson = new Gson();

    private JsonParser() {
        // Private constructor for utility class
    }

    /**
     * Parses raw JSON response string from Primary Dictionary API into a list of DictionaryEntry objects.
     */
    public static List<DictionaryEntry> parseDictionaryEntries(String jsonResponse) throws IllegalArgumentException {
        if (jsonResponse == null || jsonResponse.trim().isEmpty()) {
            throw new IllegalArgumentException("JSON response content is empty.");
        }

        List<DictionaryEntry> entries = new ArrayList<>();
        try {
            JsonElement element = com.google.gson.JsonParser.parseString(jsonResponse);
            if (element.isJsonArray()) {
                JsonArray jsonArray = element.getAsJsonArray();
                for (JsonElement item : jsonArray) {
                    DictionaryEntry entry = gson.fromJson(item, DictionaryEntry.class);
                    if (entry != null && entry.getWord() != null) {
                        entries.add(entry);
                    }
                }
            } else if (element.isJsonObject()) {
                DictionaryEntry entry = gson.fromJson(element, DictionaryEntry.class);
                if (entry != null && entry.getWord() != null) {
                    entries.add(entry);
                }
            }
        } catch (Exception e) {
            throw new IllegalArgumentException("Failed to parse primary JSON response: " + e.getMessage(), e);
        }

        return entries;
    }

    /**
     * Parses Datamuse API fallback JSON into a List of DictionaryEntry objects.
     */
    public static List<DictionaryEntry> parseDatamuseResponse(String searchWord, String defsJson, String synsJson) {
        if (defsJson == null || defsJson.trim().isEmpty()) {
            return new ArrayList<>();
        }

        List<DictionaryEntry> entries = new ArrayList<>();
        try {
            JsonElement element = com.google.gson.JsonParser.parseString(defsJson);
            if (!element.isJsonArray()) {
                return entries;
            }

            JsonArray array = element.getAsJsonArray();
            if (array.isEmpty()) {
                return entries;
            }

            JsonObject mainObj = null;
            for (JsonElement item : array) {
                if (item.isJsonObject()) {
                    JsonObject obj = item.getAsJsonObject();
                    if (obj.has("word") && obj.get("word").getAsString().equalsIgnoreCase(searchWord)) {
                        mainObj = obj;
                        break;
                    }
                }
            }

            if (mainObj == null && !array.isEmpty() && array.get(0).isJsonObject()) {
                mainObj = array.get(0).getAsJsonObject();
            }

            if (mainObj == null || !mainObj.has("defs")) {
                return entries;
            }

            DictionaryEntry entry = new DictionaryEntry();
            entry.setWord(mainObj.has("word") ? mainObj.get("word").getAsString() : searchWord);
            entry.setPhonetic("/" + searchWord.toLowerCase() + "/");

            JsonArray defsArray = mainObj.getAsJsonArray("defs");
            List<Meaning> meanings = new ArrayList<>();
            
            for (JsonElement defElem : defsArray) {
                String defText = defElem.getAsString();
                String pos = "general";
                String cleanDef = defText;

                if (defText.contains("\t")) {
                    String[] parts = defText.split("\t", 2);
                    String rawPos = parts[0].trim();
                    cleanDef = parts[1].trim();

                    pos = switch (rawPos) {
                        case "n" -> "noun";
                        case "v" -> "verb";
                        case "adj" -> "adjective";
                        case "adv" -> "adverb";
                        default -> rawPos;
                    };
                }

                Definition definitionObj = new Definition(cleanDef, "The word '" + searchWord + "' is used in standard context.", new ArrayList<>(), new ArrayList<>());
                
                // Check if meaning for pos exists
                Meaning existingMeaning = null;
                for (Meaning m : meanings) {
                    if (m.getPartOfSpeech().equalsIgnoreCase(pos)) {
                        existingMeaning = m;
                        break;
                    }
                }

                if (existingMeaning == null) {
                    existingMeaning = new Meaning();
                    existingMeaning.setPartOfSpeech(pos);
                    existingMeaning.setDefinitions(new ArrayList<>());
                    meanings.add(existingMeaning);
                }

                existingMeaning.getDefinitions().add(definitionObj);
            }

            // Extract synonyms from synsJson if available
            if (synsJson != null && !synsJson.trim().isEmpty()) {
                try {
                    JsonElement synElem = com.google.gson.JsonParser.parseString(synsJson);
                    if (synElem.isJsonArray()) {
                        List<String> synonymsList = new ArrayList<>();
                        JsonArray synArray = synElem.getAsJsonArray();
                        for (int i = 0; i < Math.min(synArray.size(), 8); i++) {
                            JsonObject sObj = synArray.get(i).getAsJsonObject();
                            if (sObj.has("word")) {
                                synonymsList.add(sObj.get("word").getAsString());
                            }
                        }
                        if (!meanings.isEmpty()) {
                            meanings.get(0).setSynonyms(synonymsList);
                        }
                    }
                } catch (Exception ignored) {
                }
            }

            entry.setMeanings(meanings);
            entries.add(entry);

        } catch (Exception e) {
            System.err.println("[JsonParser] Datamuse fallback parsing warning: " + e.getMessage());
        }

        return entries;
    }
}
