package com.dictionaryapp.service;

import com.dictionaryapp.model.DictionaryEntry;
import com.dictionaryapp.util.JsonParser;
import com.dictionaryapp.util.ValidationUtil;

import java.io.IOException;
import java.net.URI;
import java.net.URLEncoder;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.charset.StandardCharsets;
import java.time.Duration;
import java.util.List;

/**
 * Service to communicate with free public Dictionary APIs.
 * Features automatic multi-source failover (Primary API -> Datamuse API) to guarantee high availability
 * even when primary endpoint services experience network timeouts or regional ISP blocks.
 * Demonstrates Week 7 syllabus: API URL construction, java.net.http.HttpClient, 
 * HTTP response handling, status code processing, and JSON mapping.
 */
public class DictionaryService {

    private static final String PRIMARY_API_URL = "https://api.dictionaryapi.dev/api/v2/entries/en/";
    private static final String DATAMUSE_API_URL = "https://api.datamuse.com/words?sp=";
    private static final String DATAMUSE_SYN_URL = "https://api.datamuse.com/words?rel_syn=";

    private static final String USER_AGENT = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) DictionaryApp/1.0";

    private final HttpClient httpClient;

    public DictionaryService() {
        this.httpClient = HttpClient.newBuilder()
                .connectTimeout(Duration.ofSeconds(6))
                .followRedirects(HttpClient.Redirect.NORMAL)
                .build();
    }

    /**
     * Fetches dictionary entries for a specified word with automatic failover fallback.
     *
     * @param rawWord Input word string from user
     * @return List of parsed DictionaryEntry objects
     * @throws IOException if network or I/O error occurs across all APIs
     * @throws InterruptedException if thread is interrupted during request
     * @throws IllegalArgumentException if word is invalid or not found
     */
    public List<DictionaryEntry> fetchWordDefinition(String rawWord) throws IOException, InterruptedException, IllegalArgumentException {
        if (!ValidationUtil.isValidSearchTerm(rawWord)) {
            throw new IllegalArgumentException("Please enter a valid English word.");
        }

        String word = ValidationUtil.sanitizeWord(rawWord);
        String encodedWord = URLEncoder.encode(word, StandardCharsets.UTF_8);

        // Strategy 1: Attempt Primary API (api.dictionaryapi.dev)
        try {
            List<DictionaryEntry> primaryResult = fetchFromPrimaryApi(encodedWord);
            if (primaryResult != null && !primaryResult.isEmpty()) {
                System.out.println("[DictionaryService] Successfully retrieved data from Primary Dictionary API.");
                return primaryResult;
            }
        } catch (Exception e) {
            System.err.println("[DictionaryService] Primary API unavailable (" + e.getMessage() + "). Initiating automatic failover to Datamuse API...");
        }

        // Strategy 2: Attempt Fallback API (api.datamuse.com)
        try {
            List<DictionaryEntry> fallbackResult = fetchFromDatamuseApi(word, encodedWord);
            if (fallbackResult != null && !fallbackResult.isEmpty()) {
                System.out.println("[DictionaryService] Successfully retrieved data from Fallback Datamuse API.");
                return fallbackResult;
            }
        } catch (Exception e) {
            System.err.println("[DictionaryService] Fallback API error: " + e.getMessage());
        }

        throw new IllegalArgumentException("Word '" + word + "' was not found or dictionary services are unreachable.");
    }

    private List<DictionaryEntry> fetchFromPrimaryApi(String encodedWord) throws IOException, InterruptedException {
        String targetUrl = PRIMARY_API_URL + encodedWord;

        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(targetUrl))
                .timeout(Duration.ofSeconds(5))
                .header("User-Agent", USER_AGENT)
                .header("Accept", "application/json")
                .GET()
                .build();

        HttpResponse<String> response = httpClient.send(request, HttpResponse.BodyHandlers.ofString());

        int statusCode = response.statusCode();
        if (statusCode == 200) {
            return JsonParser.parseDictionaryEntries(response.body());
        } else if (statusCode == 404) {
            return null; // Not found on primary
        } else {
            throw new IOException("Primary API status: " + statusCode);
        }
    }

    private List<DictionaryEntry> fetchFromDatamuseApi(String rawWord, String encodedWord) throws IOException, InterruptedException {
        String defsUrl = DATAMUSE_API_URL + encodedWord + "&md=d";
        String synsUrl = DATAMUSE_SYN_URL + encodedWord;

        HttpRequest defsReq = HttpRequest.newBuilder()
                .uri(URI.create(defsUrl))
                .timeout(Duration.ofSeconds(6))
                .header("User-Agent", USER_AGENT)
                .header("Accept", "application/json")
                .GET()
                .build();

        HttpResponse<String> defsResp = httpClient.send(defsReq, HttpResponse.BodyHandlers.ofString());
        if (defsResp.statusCode() != 200) {
            return null;
        }

        // Fetch synonyms in parallel/sequential fallback
        String synsBody = "";
        try {
            HttpRequest synsReq = HttpRequest.newBuilder()
                    .uri(URI.create(synsUrl))
                    .timeout(Duration.ofSeconds(4))
                    .header("User-Agent", USER_AGENT)
                    .header("Accept", "application/json")
                    .GET()
                    .build();
            HttpResponse<String> synsResp = httpClient.send(synsReq, HttpResponse.BodyHandlers.ofString());
            if (synsResp.statusCode() == 200) {
                synsBody = synsResp.body();
            }
        } catch (Exception ignored) {
        }

        return JsonParser.parseDatamuseResponse(rawWord, defsResp.body(), synsBody);
    }
}
