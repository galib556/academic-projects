package com.dictionaryapp;

import com.dictionaryapp.controller.MainController;
import com.dictionaryapp.database.DatabaseManager;
import javafx.application.Application;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.net.URL;

/**
 * Main JavaFX Application class.
 * Demonstrates Week 3 syllabus (JavaFX application structure, Stage, Scene, CSS styling)
 * and initializes SQLite database (Week 6).
 */
public class DictionaryApplication extends Application {

    private MainController mainController;

    @Override
    public void init() throws Exception {
        super.init();
        // Initialize SQLite database table on startup (Week 6)
        DatabaseManager.initializeDatabase();
    }

    @Override
    public void start(Stage primaryStage) {
        mainController = new MainController();
        Parent root = mainController.buildUI(primaryStage);

        Scene scene = new Scene(root, 900, 680);

        // Load custom CSS styling
        URL cssUrl = getClass().getResource("/style.css");
        if (cssUrl != null) {
            scene.getStylesheets().add(cssUrl.toExternalForm());
        } else {
            System.err.println("[DictionaryApplication] CSS stylesheet /style.css not found.");
        }

        primaryStage.setTitle("Dictionary & Vocabulary Builder");
        primaryStage.setMinWidth(800);
        primaryStage.setMinHeight(600);
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    @Override
    public void stop() throws Exception {
        if (mainController != null) {
            mainController.shutdown();
        }
        super.stop();
    }

    public static void launchApp(String[] args) {
        launch(args);
    }
}
