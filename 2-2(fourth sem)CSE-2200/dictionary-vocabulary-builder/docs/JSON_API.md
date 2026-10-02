# JSON & Public API Integration Guide

## Overview

Week 7 of the syllabus covers consuming public APIs, processing HTTP responses, and converting raw JSON strings into structured Java objects.

---

## Dictionary API Overview

- **Service URL**: `https://api.dictionaryapi.dev/api/v2/entries/en/{word}`
- **HTTP Method**: `GET`
- **Authentication**: Free public API (no API key required)

---

## JSON Response Structure

Example API JSON payload for `hello`:

```json
[
  {
    "word": "hello",
    "phonetic": "/həˈloʊ/",
    "phonetics": [
      {
        "text": "/həˈloʊ/",
        "audio": "https://api.dictionaryapi.dev/media/pronunciations/en/hello-us.mp3"
      }
    ],
    "meanings": [
      {
        "partOfSpeech": "exclamation",
        "definitions": [
          {
            "definition": "Used as a greeting or to begin a phone conversation.",
            "example": "hello there!",
            "synonyms": ["greeting"],
            "antonyms": []
          }
        ]
      }
    ]
  }
]
```

---

## API Request Flow

```
User Input ("   Hello   ")
       ↓  (ValidationUtil.sanitizeWord)
Sanitized Endpoint ("hello")
       ↓
java.net.http.HttpClient GET Request
       ↓
HttpResponse Status Code (200 OK / 404 Not Found)
       ↓
JsonParser (Google Gson)
       ↓
List<DictionaryEntry> Java Objects
       ↓
JavaFX UI Card Display
```

---

## Error & Edge Case Handling

1. **HTTP 404 (Word Not Found)**: Displays friendly alert: `"Word 'xyz' was not found in the dictionary."`
2. **Empty or Whitespace Input**: Validation prevents network request and highlights input field.
3. **Network Connection Timeout / Offline**: Catch `IOException` and display connection error alert without crashing application.
4. **Malformed JSON**: Gson safely handles missing fields and defaults to `"N/A"` or empty collections.
