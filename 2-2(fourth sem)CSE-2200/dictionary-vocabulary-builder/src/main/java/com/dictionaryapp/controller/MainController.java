package com.dictionaryapp.controller;

import com.dictionaryapp.model.DictionaryEntry;
import com.dictionaryapp.model.SavedWord;
import com.dictionaryapp.service.DatabaseService;
import com.dictionaryapp.service.DictionaryService;
import com.dictionaryapp.service.SearchTask;
import com.dictionaryapp.util.ValidationUtil;
import javafx.application.Platform;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Parent;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import javafx.stage.Stage;

import java.sql.SQLException;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Controller for the Dictionary & Vocabulary Builder JavaFX Application.
 * Integrates JavaFX UI components, event handlers, multithreaded background tasks (ExecutorService),
 * SQLite database CRUD operations, and external API consumption.
 */
public class MainController {

    // Services
    private final DictionaryService dictionaryService;
    private final DatabaseService databaseService;

    // Multithreading ExecutorService (Week 4: Java Concurrency Package & Executors)
    private final ExecutorService executorService;

    // Root Container
    private BorderPane rootPane;

    // Search Tab Controls
    private TextField searchField;
    private Button searchButton;
    private Button clearButton;
    private ProgressIndicator progressIndicator;
    private Label wordLabel;
    private Label phoneticLabel;
    private Label posLabel;
    private TextArea definitionArea;
    private TextArea exampleArea;
    private Label synonymsLabel;
    private Button saveWordButton;
    private VBox resultCard;

    // Current API Search Result Cache
    private DictionaryEntry currentEntry;

    // Saved Vocabulary Tab Controls
    private TextField filterSavedField;
    private ListView<SavedWord> savedListView;
    private ObservableList<SavedWord> savedWordsList;
    private Label savedWordTitleLabel;
    private Label savedPhoneticLabel;
    private Label savedPosLabel;
    private TextArea savedDefinitionArea;
    private TextArea savedExampleArea;
    private Label savedSynonymsLabel;
    private Label savedDateLabel;
    private Button editButton;
    private Button deleteButton;
    private Button refreshButton;

    // Status Bar
    private Label statusLabel;

    public MainController() {
        this.dictionaryService = new DictionaryService();
        this.databaseService = new DatabaseService();
        // Fixed thread pool managing concurrent tasks (Week 4 topic)
        this.executorService = Executors.newFixedThreadPool(4);
        this.savedWordsList = FXCollections.observableArrayList();
    }

    /**
     * Builds and initializes the complete JavaFX UI layout.
     */
    public Parent buildUI(Stage primaryStage) {
        rootPane = new BorderPane();

        // 1. Header Box
        VBox headerBox = createHeaderBox();
        rootPane.setTop(headerBox);

        // 2. Tab Pane (Dictionary Search & Saved Vocabulary)
        TabPane tabPane = new TabPane();
        tabPane.setTabClosingPolicy(TabPane.TabClosingPolicy.UNAVAILABLE);

        Tab searchTab = new Tab("Dictionary Search", createSearchTabContent());
        Tab savedTab = new Tab("Saved Vocabulary", createSavedTabContent());

        tabPane.getTabs().addAll(searchTab, savedTab);
        rootPane.setCenter(tabPane);

        // 3. Status Bar
        HBox statusBar = createStatusBar();
        rootPane.setBottom(statusBar);

        // Handle Application Shutdown to clean up thread pool resources
        primaryStage.setOnCloseRequest(event -> shutdown());

        // Load saved vocabulary asynchronously on startup using a Runnable (Week 4 requirement)
        loadSavedVocabularyAsync();

        return rootPane;
    }

    private VBox createHeaderBox() {
        VBox header = new VBox(4);
        header.getStyleClass().add("header-box");
        Label title = new Label("DICTIONARY & VOCABULARY BUILDER");
        title.getStyleClass().add("title-label");
        Label subtitle = new Label("Search English definitions, explore phonetics, and manage personal vocabulary");
        subtitle.getStyleClass().add("subtitle-label");
        header.getChildren().addAll(title, subtitle);
        return header;
    }

    private VBox createSearchTabContent() {
        VBox container = new VBox(15);
        container.setPadding(new Insets(20));

        // Search Bar (HBox)
        HBox searchBar = new HBox(10);
        searchBar.setAlignment(Pos.CENTER_LEFT);

        searchField = new TextField();
        searchField.setPromptText("Enter an English word (e.g., algorithm, beautiful)...");
        searchField.setPrefWidth(400);
        HBox.setHgrow(searchField, Priority.ALWAYS);

        // Trigger search on Enter key press
        searchField.setOnAction(e -> handleSearch());

        searchButton = new Button("SEARCH");
        searchButton.setOnAction(e -> handleSearch());

        clearButton = new Button("CLEAR");
        clearButton.getStyleClass().add("button-secondary");
        clearButton.setOnAction(e -> handleClearSearch());

        progressIndicator = new ProgressIndicator();
        progressIndicator.setPrefSize(24, 24);
        progressIndicator.setVisible(false);

        searchBar.getChildren().addAll(searchField, searchButton, clearButton, progressIndicator);

        // Result Card (VBox)
        resultCard = new VBox(12);
        resultCard.getStyleClass().add("card-panel");
        resultCard.setVisible(false);

        HBox wordHeader = new HBox(12);
        wordHeader.setAlignment(Pos.CENTER_LEFT);

        wordLabel = new Label();
        wordLabel.getStyleClass().add("word-heading");

        phoneticLabel = new Label();
        phoneticLabel.getStyleClass().add("phonetic-badge");

        posLabel = new Label();
        posLabel.getStyleClass().add("pos-tag");

        wordHeader.getChildren().addAll(wordLabel, phoneticLabel, posLabel);

        Label defTitle = new Label("Definition:");
        defTitle.getStyleClass().add("section-title");

        definitionArea = new TextArea();
        definitionArea.setEditable(false);
        definitionArea.setWrapText(true);
        definitionArea.setPrefRowCount(3);

        Label exTitle = new Label("Example Sentence:");
        exTitle.getStyleClass().add("section-title");

        exampleArea = new TextArea();
        exampleArea.setEditable(false);
        exampleArea.setWrapText(true);
        exampleArea.setPrefRowCount(2);

        HBox synBox = new HBox(8);
        synBox.setAlignment(Pos.CENTER_LEFT);
        Label synTitle = new Label("Synonyms: ");
        synTitle.getStyleClass().add("section-title");
        synonymsLabel = new Label();
        synBox.getChildren().addAll(synTitle, synonymsLabel);

        saveWordButton = new Button("SAVE TO VOCABULARY");
        saveWordButton.getStyleClass().add("button-success");
        saveWordButton.setOnAction(e -> handleSaveCurrentWord());

        resultCard.getChildren().addAll(
            wordHeader,
            defTitle, definitionArea,
            exTitle, exampleArea,
            synBox,
            new Separator(),
            saveWordButton
        );

        ScrollPane scrollPane = new ScrollPane(resultCard);
        scrollPane.setFitToWidth(true);
        scrollPane.setStyle("-fx-background-color: transparent; -fx-background: transparent;");

        container.getChildren().addAll(searchBar, scrollPane);
        return container;
    }

    private SplitPane createSavedTabContent() {
        SplitPane splitPane = new SplitPane();
        splitPane.setPadding(new Insets(15));

        // Left Pane: Vocabulary List & Controls
        VBox leftPane = new VBox(10);
        leftPane.setPadding(new Insets(10));

        filterSavedField = new TextField();
        filterSavedField.setPromptText("Filter saved vocabulary...");
        filterSavedField.textProperty().addListener((obs, oldVal, newVal) -> handleFilterSavedWords(newVal));

        savedListView = new ListView<>(savedWordsList);
        VBox.setVgrow(savedListView, Priority.ALWAYS);

        // Selection Listener
        savedListView.getSelectionModel().selectedItemProperty().addListener((obs, oldVal, newVal) -> {
            displaySavedWordDetails(newVal);
        });

        refreshButton = new Button("REFRESH LIST");
        refreshButton.getStyleClass().add("button-secondary");
        refreshButton.setMaxWidth(Double.MAX_VALUE);
        refreshButton.setOnAction(e -> loadSavedVocabularyAsync());

        leftPane.getChildren().addAll(new Label("Saved Words:"), filterSavedField, savedListView, refreshButton);

        // Right Pane: Detail View & Actions
        VBox rightPane = new VBox(12);
        rightPane.setPadding(new Insets(10));
        rightPane.getStyleClass().add("card-panel");

        HBox header = new HBox(12);
        header.setAlignment(Pos.CENTER_LEFT);

        savedWordTitleLabel = new Label("Select a word");
        savedWordTitleLabel.getStyleClass().add("word-heading");

        savedPhoneticLabel = new Label();
        savedPhoneticLabel.getStyleClass().add("phonetic-badge");

        savedPosLabel = new Label();
        savedPosLabel.getStyleClass().add("pos-tag");

        header.getChildren().addAll(savedWordTitleLabel, savedPhoneticLabel, savedPosLabel);

        savedDateLabel = new Label();
        savedDateLabel.setStyle("-fx-text-fill: #64748b; -fx-font-size: 11px;");

        Label defLbl = new Label("Definition:");
        defLbl.getStyleClass().add("section-title");
        savedDefinitionArea = new TextArea();
        savedDefinitionArea.setEditable(false);
        savedDefinitionArea.setWrapText(true);
        savedDefinitionArea.setPrefRowCount(3);

        Label exLbl = new Label("Example Sentence:");
        exLbl.getStyleClass().add("section-title");
        savedExampleArea = new TextArea();
        savedExampleArea.setEditable(false);
        savedExampleArea.setWrapText(true);
        savedExampleArea.setPrefRowCount(2);

        HBox synBox = new HBox(8);
        Label synLbl = new Label("Synonyms: ");
        synLbl.getStyleClass().add("section-title");
        savedSynonymsLabel = new Label();
        synBox.getChildren().addAll(synLbl, savedSynonymsLabel);

        HBox actionBox = new HBox(10);
        editButton = new Button("EDIT / UPDATE");
        editButton.setDisable(true);
        editButton.setOnAction(e -> handleUpdateSavedWord());

        deleteButton = new Button("DELETE");
        deleteButton.getStyleClass().add("button-danger");
        deleteButton.setDisable(true);
        deleteButton.setOnAction(e -> handleDeleteSavedWord());

        actionBox.getChildren().addAll(editButton, deleteButton);

        rightPane.getChildren().addAll(
            header,
            savedDateLabel,
            defLbl, savedDefinitionArea,
            exLbl, savedExampleArea,
            synBox,
            new Separator(),
            actionBox
        );

        splitPane.getItems().addAll(leftPane, rightPane);
        splitPane.setDividerPositions(0.35);

        return splitPane;
    }

    private HBox createStatusBar() {
        HBox bar = new HBox();
        bar.getStyleClass().add("status-bar");
        statusLabel = new Label("Ready");
        statusLabel.getStyleClass().add("status-label");
        bar.getChildren().add(statusLabel);
        return bar;
    }

    private void updateStatus(String message) {
        Platform.runLater(() -> statusLabel.setText(message));
    }

    // =========================================================================
    // MULTITHREADED DICTIONARY API SEARCH HANDLER (Week 4 & Week 7)
    // =========================================================================
    private void handleSearch() {
        String input = searchField.getText();
        if (!ValidationUtil.isValidSearchTerm(input)) {
            showAlert(Alert.AlertType.WARNING, "Input Error", "Please enter an English word to search.");
            return;
        }

        String targetWord = ValidationUtil.sanitizeWord(input);
        searchButton.setDisable(true);
        progressIndicator.setVisible(true);
        updateStatus("Searching API for '" + targetWord + "'...");

        // Create asynchronous JavaFX Task
        SearchTask task = new SearchTask(dictionaryService, targetWord);

        task.setOnSucceeded(event -> {
            searchButton.setDisable(false);
            progressIndicator.setVisible(false);
            List<DictionaryEntry> entries = task.getValue();

            if (entries != null && !entries.isEmpty()) {
                currentEntry = entries.get(0);
                displaySearchResult(currentEntry);
                updateStatus("Found definition for '" + targetWord + "'");
            } else {
                resultCard.setVisible(false);
                updateStatus("No definition found for '" + targetWord + "'");
                showAlert(Alert.AlertType.INFORMATION, "Not Found", "No definitions found for '" + targetWord + "'.");
            }
        });

        task.setOnFailed(event -> {
            searchButton.setDisable(false);
            progressIndicator.setVisible(false);
            Throwable exception = task.getException();
            String errorMsg = exception != null ? exception.getMessage() : "Unknown API error";
            updateStatus("Search failed: " + errorMsg);
            showAlert(Alert.AlertType.ERROR, "Search Failed", errorMsg);
        });

        // Submit task to ExecutorService thread pool (Week 4: Executors)
        executorService.submit(task);
    }

    private void handleClearSearch() {
        searchField.clear();
        resultCard.setVisible(false);
        currentEntry = null;
        updateStatus("Search cleared");
    }

    private void displaySearchResult(DictionaryEntry entry) {
        wordLabel.setText(entry.getWord());
        phoneticLabel.setText(entry.getPhonetic());
        posLabel.setText(entry.getPrimaryPartOfSpeech());
        definitionArea.setText(entry.getPrimaryDefinition());
        exampleArea.setText(entry.getPrimaryExample());
        synonymsLabel.setText(entry.getAggregatedSynonyms());
        resultCard.setVisible(true);
    }

    // =========================================================================
    // SQLITE DATABASE CRUD OPERATIONS (Week 6)
    // =========================================================================

    /**
     * CREATE / INSERT: Saves the currently searched word into SQLite.
     */
    private void handleSaveCurrentWord() {
        if (currentEntry == null) {
            showAlert(Alert.AlertType.WARNING, "No Word Selected", "Please search for a word first.");
            return;
        }

        SavedWord wordToSave = new SavedWord(
            currentEntry.getWord(),
            currentEntry.getPhonetic(),
            currentEntry.getPrimaryPartOfSpeech(),
            currentEntry.getPrimaryDefinition(),
            currentEntry.getPrimaryExample(),
            currentEntry.getAggregatedSynonyms()
        );

        // Run database insert on background thread to prevent UI lock
        executorService.submit(() -> {
            try {
                if (databaseService.isWordSaved(wordToSave.getWord())) {
                    Platform.runLater(() -> showAlert(Alert.AlertType.INFORMATION, "Already Saved", 
                        "The word '" + wordToSave.getWord() + "' is already in your vocabulary collection."));
                    return;
                }

                boolean success = databaseService.saveWord(wordToSave);
                Platform.runLater(() -> {
                    if (success) {
                        updateStatus("Saved '" + wordToSave.getWord() + "' to database.");
                        showAlert(Alert.AlertType.INFORMATION, "Success", "Word '" + wordToSave.getWord() + "' saved successfully!");
                        loadSavedVocabularyAsync();
                    } else {
                        showAlert(Alert.AlertType.ERROR, "Database Error", "Failed to save word.");
                    }
                });
            } catch (SQLException e) {
                Platform.runLater(() -> showAlert(Alert.AlertType.ERROR, "Database Error", "Error saving word: " + e.getMessage()));
            }
        });
    }

    /**
     * READ / SELECT: Loads saved vocabulary from SQLite on a background thread.
     * Demonstrates explicitly using the Runnable interface (Week 4 requirement).
     */
    private void loadSavedVocabularyAsync() {
        updateStatus("Loading vocabulary from database...");

        // Week 4: Runnable implementation
        Runnable loadTask = new Runnable() {
            @Override
            public void run() {
                try {
                    List<SavedWord> words = databaseService.getAllSavedWords();
                    // Update JavaFX UI control on JavaFX Application Thread
                    Platform.runLater(() -> {
                        savedWordsList.setAll(words);
                        updateStatus("Loaded " + words.size() + " saved vocabulary words.");
                    });
                } catch (SQLException e) {
                    Platform.runLater(() -> {
                        updateStatus("Database read failure: " + e.getMessage());
                        showAlert(Alert.AlertType.ERROR, "Database Failure", "Could not load saved vocabulary: " + e.getMessage());
                    });
                }
            }
        };

        executorService.submit(loadTask);
    }

    /**
     * READ / SELECT Filter: Searches local SQLite records by keyword.
     */
    private void handleFilterSavedWords(String keyword) {
        executorService.submit(() -> {
            try {
                List<SavedWord> filtered = databaseService.searchSavedWords(keyword);
                Platform.runLater(() -> savedWordsList.setAll(filtered));
            } catch (SQLException e) {
                Platform.runLater(() -> updateStatus("Filter error: " + e.getMessage()));
            }
        });
    }

    private void displaySavedWordDetails(SavedWord word) {
        if (word == null) {
            savedWordTitleLabel.setText("Select a word");
            savedPhoneticLabel.setText("");
            savedPosLabel.setText("");
            savedDefinitionArea.clear();
            savedExampleArea.clear();
            savedSynonymsLabel.setText("");
            savedDateLabel.setText("");
            editButton.setDisable(true);
            deleteButton.setDisable(true);
            return;
        }

        savedWordTitleLabel.setText(word.getWord());
        savedPhoneticLabel.setText(word.getPhonetic());
        savedPosLabel.setText(word.getPartOfSpeech());
        savedDefinitionArea.setText(word.getDefinition());
        savedExampleArea.setText(word.getExample());
        savedSynonymsLabel.setText(word.getSynonyms());
        savedDateLabel.setText("Saved on: " + (word.getCreatedAt() != null ? word.getCreatedAt() : "N/A"));

        editButton.setDisable(false);
        deleteButton.setDisable(false);
    }

    /**
     * UPDATE: Edits selected saved word details in SQLite database.
     */
    private void handleUpdateSavedWord() {
        SavedWord selected = savedListView.getSelectionModel().getSelectedItem();
        if (selected == null) return;

        Dialog<SavedWord> dialog = new Dialog<>();
        dialog.setTitle("Edit Saved Word");
        dialog.setHeaderText("Update details for '" + selected.getWord() + "'");

        ButtonType updateButtonType = new ButtonType("Update", ButtonBar.ButtonData.OK_DONE);
        dialog.getDialogPane().getButtonTypes().addAll(updateButtonType, ButtonType.CANCEL);

        GridPane grid = new GridPane();
        grid.setHgap(10);
        grid.setVgap(10);
        grid.setPadding(new Insets(20, 150, 10, 10));

        TextField phoneticInput = new TextField(selected.getPhonetic());
        TextField posInput = new TextField(selected.getPartOfSpeech());
        TextArea defInput = new TextArea(selected.getDefinition());
        defInput.setPrefRowCount(3);
        TextArea exInput = new TextArea(selected.getExample());
        exInput.setPrefRowCount(2);
        TextField synInput = new TextField(selected.getSynonyms());

        grid.add(new Label("Phonetic:"), 0, 0);
        grid.add(phoneticInput, 1, 0);
        grid.add(new Label("Part of Speech:"), 0, 1);
        grid.add(posInput, 1, 1);
        grid.add(new Label("Definition:"), 0, 2);
        grid.add(defInput, 1, 2);
        grid.add(new Label("Example:"), 0, 3);
        grid.add(exInput, 1, 3);
        grid.add(new Label("Synonyms:"), 0, 4);
        grid.add(synInput, 1, 4);

        dialog.getDialogPane().setContent(grid);

        dialog.setResultConverter(dialogButton -> {
            if (dialogButton == updateButtonType) {
                selected.setPhonetic(phoneticInput.getText());
                selected.setPartOfSpeech(posInput.getText());
                selected.setDefinition(defInput.getText());
                selected.setExample(exInput.getText());
                selected.setSynonyms(synInput.getText());
                return selected;
            }
            return null;
        });

        Optional<SavedWord> result = dialog.showAndWait();
        result.ifPresent(updatedWord -> {
            executorService.submit(() -> {
                try {
                    boolean ok = databaseService.updateSavedWord(updatedWord);
                    Platform.runLater(() -> {
                        if (ok) {
                            updateStatus("Updated word '" + updatedWord.getWord() + "'");
                            displaySavedWordDetails(updatedWord);
                            savedListView.refresh();
                        } else {
                            showAlert(Alert.AlertType.ERROR, "Update Failed", "Could not update word record.");
                        }
                    });
                } catch (SQLException e) {
                    Platform.runLater(() -> showAlert(Alert.AlertType.ERROR, "Database Error", e.getMessage()));
                }
            });
        });
    }

    /**
     * DELETE: Deletes selected word record from SQLite database.
     */
    private void handleDeleteSavedWord() {
        SavedWord selected = savedListView.getSelectionModel().getSelectedItem();
        if (selected == null) return;

        Alert confirm = new Alert(Alert.AlertType.CONFIRMATION);
        confirm.setTitle("Confirm Delete");
        confirm.setHeaderText("Delete '" + selected.getWord() + "'?");
        confirm.setContentText("Are you sure you want to delete this word from your saved vocabulary?");

        Optional<ButtonType> result = confirm.showAndWait();
        if (result.isPresent() && result.get() == ButtonType.OK) {
            executorService.submit(() -> {
                try {
                    boolean deleted = databaseService.deleteSavedWord(selected.getId());
                    Platform.runLater(() -> {
                        if (deleted) {
                            updateStatus("Deleted '" + selected.getWord() + "' from database.");
                            loadSavedVocabularyAsync();
                            displaySavedWordDetails(null);
                        } else {
                            showAlert(Alert.AlertType.ERROR, "Delete Failed", "Could not delete record from database.");
                        }
                    });
                } catch (SQLException e) {
                    Platform.runLater(() -> showAlert(Alert.AlertType.ERROR, "Database Error", e.getMessage()));
                }
            });
        }
    }

    private void showAlert(Alert.AlertType type, String title, String content) {
        Alert alert = new Alert(type);
        alert.setTitle(title);
        alert.setHeaderText(null);
        alert.setContentText(content);
        alert.showAndWait();
    }

    /**
     * Cleanly shuts down the multithreaded ExecutorService on application exit.
     */
    public void shutdown() {
        if (executorService != null && !executorService.isShutdown()) {
            executorService.shutdownNow();
            System.out.println("[MainController] ExecutorService shutdown complete.");
        }
    }
}
