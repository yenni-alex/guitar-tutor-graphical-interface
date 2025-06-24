#include "UI.h"

// Gestion des widgets (internes au module)
static Widget widgets[MAX_WIDGETS];
static int widgetCount = 0;

// Gestion de l'écran courant
static Screen* currentScreen = nullptr;

// Déclaration des écrans
Screen resumeScreen = { drawResumeScreen, handleTouchResumeScreen };
Screen loadScreen = { drawLoadScreen, handleTouchLoadScreen };
Screen optionsScreen = { drawOptionsScreen, handleTouchOptionsScreen };
Screen menuScreen = { drawMenuScreen, handleTouchMenuScreen };

// Fonctions DRAW de chaque SCREEN
void drawResumeScreen() {
    setupResumeScreen(); // Setup les boutons de reprise
    //drawWidgets();
}
void drawLoadScreen() {
    setupLoadScreen(); // Setup les boutons de chargement
    //drawWidgets();
}
void drawOptionsScreen() {
    setupOptionsScreen(); // Setup les options
    //drawWidgets();
}
void drawMenuScreen() {
    setupMenuScreen(); // Setup les boutons du menu
    //drawWidgets();
}

// Fonctions de gestion des touches pour chaque SCREEN
void handleTouchResumeScreen(int x, int y) {
    handleTouchWidgets(x, y);
}
void handleTouchLoadScreen(int x, int y) {
    handleTouchWidgets(x, y);
}
void handleTouchOptionsScreen(int x, int y) {
    handleTouchWidgets(x, y);
}
void handleTouchMenuScreen(int x, int y) {
    handleTouchWidgets(x, y);
}

int8_t selectedFileIndex = -1; // -1 = rien sélectionné

// === Callbacks des boutons ===
void onResumePressed() {
    setScreen(&resumeScreen); 
    Serial.println("onResumePressed called");
}
void onLoadPressed()    {
    Serial.println("onLoadPressed called");
    printFreeMemory(); // GROSSE PERTE DE MEMOIRE
    setScreen(&loadScreen);
    printFreeMemory(); // Affiche la mémoire libre après avoir chargé l'écran de chargement
}
void onOptionsPressed() { setScreen(&optionsScreen); }
void onBackToMenu() { setScreen(&menuScreen); }
void onSettingsPressed() { Serial.println("ON SETTINGS PRESSED");}//TODO
void onPlayPressed() { Serial.println("ON PLAY PRESSED"); } //TODO
void onPausePressed() { Serial.println("ON PAUSE PRESSED"); } //TODO
void onStopPressed() { Serial.println("ON STOP PRESSED"); } //TODO
void onRestartPressed() { Serial.println("ON RESTART PRESSED"); } //TODO
void onLoadXmlPressed() { 
    Serial.println("onLoadXmlPressed called"); 
    printFreeMemory(); // Affiche la mémoire libre avant de charger une chanson

    Serial.println("Song loaded from XML.");
    setScreen(&resumeScreen); // Retour à l'écran de reprise après chargement   
    printFreeMemory(); // Affiche la mémoire libre après chargement
    Serial.print("Screen has changed: ");    
} 



// === Setup du MenuScreen ===
void setupMenuScreen() {
    clearWidgets();
    clearDisplay(ILI9341_T4_COLOR_PURPLE);

    Widget resumeButton = {
        BUTTON_X, BUTTON_START_Y, BUTTON_WIDTH, BUTTON_HEIGHT,
        onResumePressed,
        [](){
            drawRectangle(BUTTON_X, BUTTON_START_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 2, ILI9341_T4_COLOR_BLUE);
            writeText(BUTTON_X + (BUTTON_WIDTH / 2), BUTTON_START_Y + (BUTTON_HEIGHT / 2) + 14, "Resume", 65535U, 14, true);
            Serial.println("Resume button drawn and written.");
        }
    };
    addWidget(resumeButton);

    Widget loadButton = {
        BUTTON_X, BUTTON_START_Y + BUTTON_HEIGHT + BUTTON_SPACING, BUTTON_WIDTH, BUTTON_HEIGHT,
        onLoadPressed,
        [](){
            drawRectangle(BUTTON_X, BUTTON_START_Y + BUTTON_HEIGHT + BUTTON_SPACING, BUTTON_WIDTH, BUTTON_HEIGHT, 2, ILI9341_T4_COLOR_YELLOW);
            writeText(BUTTON_X + (BUTTON_WIDTH / 2), BUTTON_START_Y + (1.5*BUTTON_HEIGHT) + BUTTON_SPACING + 14, "Load", 65535U, 14, true);

        }
    };
    addWidget(loadButton);

    Widget optionsButton = {
        BUTTON_X, BUTTON_START_Y + 2 * (BUTTON_HEIGHT + BUTTON_SPACING), BUTTON_WIDTH, BUTTON_HEIGHT,
        onOptionsPressed,
        [](){
            drawRectangle(BUTTON_X, BUTTON_START_Y + 2 * (BUTTON_HEIGHT + BUTTON_SPACING), BUTTON_WIDTH, BUTTON_HEIGHT, 2, ILI9341_T4_COLOR_RED);
            writeText(BUTTON_X + (BUTTON_WIDTH / 2), BUTTON_START_Y + (2.5*BUTTON_HEIGHT) + (2*BUTTON_SPACING) + 14, "Options", 65535U, 14, true);
        }
    };
    addWidget(optionsButton);

    drawWidgets(); // Affiche les boutons du menu
    Serial.println("Menu screen setup complete.");
    printFreeMemory(); // Affiche la mémoire libre après le setup du menu
}
void setupResumeScreen() {
    clearWidgets();
    clearDisplay(ILI9341_T4_COLOR_WHITE);
    drawTabulation();

    Widget settingsButton = {
        W - 48, 0, 48, 48,
        onSettingsPressed,
        [](){
            drawIcon(W - 48, 0, settings_icon, 48, 48);
        }
    };
    addWidget(settingsButton);

    Widget playButton = {
        0, H - 48, 48, 48,
        onPlayPressed,
        [](){
            drawIcon(0, H - 48, play_icon, 48, 48);
        }
    };
    addWidget(playButton);

    Widget pauseButton = {
        48, H - 48, 48, 48,
        onPausePressed,
        [](){
            drawIcon(48, H - 48, pause_icon, 48, 48);
        }
    };
    addWidget(pauseButton);

    Widget stopButton = {
        96, H - 48, 48, 48,
        onStopPressed,
        [](){
            drawIcon(96, H - 48, stop_icon, 48, 48);
        }
    };
    addWidget(stopButton);

    Widget restartButton = {
        144, H - 48, 48, 48,
        onRestartPressed,
        [](){
            drawIcon(144, H - 48, restart_icon, 48, 48);
        }
    };
    addWidget(restartButton);

    //drawButtons(); // Affiche les boutons de contrôle
    addBackButton();

    drawWidgets(); // Affiche les boutons de reprise
    Serial.println("Resume screen setup complete.");
    printFreeMemory(); // Affiche la mémoire libre après le setup de l'écran de reprise
}
void setupLoadScreen() {
    clearWidgets();
    clearDisplay(ILI9341_T4_COLOR_WHITE);
    addBackButton();
    Serial.println("Setting up Load Screen...");
    if(selectedFileIndex != -1) {
        Widget loadXmlButton = {
            W - 60, H - 40, 60, 40,
            onLoadXmlPressed,
            [](){
                drawRectangle(W - 60, H - 40, 60, 40, 2, ILI9341_T4_COLOR_GREEN);
                writeText(W - 30, H - 14, "Load", ILI9341_T4_COLOR_BLACK, 14, true);
            }
        };
        addWidget(loadXmlButton);
    }
    
    

    // Affichage des fichiers XML disponibles
    const uint8_t fileButtonHeight = 30;
    const uint8_t startY = 48;
    const uint8_t spacing = 5;
    if(fileCount == 0) {
        writeText(10, startY, "No files available", ILI9341_T4_COLOR_BLACK, 14, true);
        return;
    }
    Widget fileButton0 = {
        10, startY, 200, fileButtonHeight,
        [](){ 
            selectedFileIndex = 0;
            setupLoadScreen(); // Redraw the screen to update the selected file
            screenHasChanged = true; // Indicate that the screen has changed
         },
        [](){
            uint16_t color = (selectedFileIndex == 0) ? ILI9341_T4_COLOR_BLUE : ILI9341_T4_COLOR_GRAY; 
            drawRectangle(10, startY, 200, fileButtonHeight, 2, color);
            writeText(110, startY + 27, fileList[0], ILI9341_T4_COLOR_BLACK, 14, true);
        }
    };
    addWidget(fileButton0);
    if( fileCount > 1) {
        Widget fileButton1 = {
            10, startY + fileButtonHeight + spacing, 200, fileButtonHeight,
            [](){ 
                selectedFileIndex = 1;
                setupLoadScreen(); // Redraw the screen to update the selected file
                screenHasChanged = true; // Indicate that the screen has changed
             }, // Placeholder for actual function
            [](){
                uint16_t color = (selectedFileIndex == 1) ? ILI9341_T4_COLOR_BLUE : ILI9341_T4_COLOR_GRAY; 
                drawRectangle(10, startY + fileButtonHeight + spacing, 200, fileButtonHeight, 2, color);
                writeText(110, startY + fileButtonHeight + spacing + 27, fileList[1], ILI9341_T4_COLOR_BLACK, 14, true);
            }
        };
        addWidget(fileButton1);
    }
    if( fileCount > 2) {
        Widget fileButton2 = {
            10, startY + 2 * (fileButtonHeight + spacing), 200, fileButtonHeight,
            [](){ 
                selectedFileIndex = 2; 
                setupLoadScreen(); // Redraw the screen to update the selected file
                screenHasChanged = true; // Indicate that the screen has changed
            }, // Placeholder for actual function
            [](){
                uint16_t color = (selectedFileIndex == 2) ? ILI9341_T4_COLOR_BLUE : ILI9341_T4_COLOR_GRAY;
                drawRectangle(10, startY + 2 * (fileButtonHeight + spacing), 200, fileButtonHeight, 2, color);
                writeText(110, startY + 2 * (fileButtonHeight + spacing) + 27, fileList[2], ILI9341_T4_COLOR_BLACK, 14, true);
            }
        };
        addWidget(fileButton2);
    }
    if( fileCount > 3) {
        Widget fileButton3 = {
            10, startY + 3 * (fileButtonHeight + spacing), 200, fileButtonHeight,
            [](){ 
                selectedFileIndex = 3; 
                setupLoadScreen(); // Redraw the screen to update the selected file
                screenHasChanged = true; // Indicate that the screen has changed
            }, // Placeholder for actual function
            [](){
                uint16_t color = (selectedFileIndex == 3) ? ILI9341_T4_COLOR_BLUE : ILI9341_T4_COLOR_GRAY;
                drawRectangle(10, startY + 3 * (fileButtonHeight + spacing), 200, fileButtonHeight, 2, color);
                writeText(110, startY + 3 * (fileButtonHeight + spacing) + 27, fileList[3], ILI9341_T4_COLOR_BLACK, 14, true);
            }
        };
        addWidget(fileButton3);
    }
    if( fileCount > 4) {
        Widget fileButton4 = {
            10, startY + 4 * (fileButtonHeight + spacing), 200, fileButtonHeight,
            [](){
                selectedFileIndex = 4;
                setupLoadScreen(); // Redraw the screen to update the selected file
                screenHasChanged = true; // Indicate that the screen has changed
            }, // Placeholder for actual function
            [](){
                uint16_t color = (selectedFileIndex == 4) ? ILI9341_T4_COLOR_BLUE : ILI9341_T4_COLOR_GRAY;
                drawRectangle(10, startY + 4 * (fileButtonHeight + spacing), 200, fileButtonHeight, 2, color);
                writeText(110, startY + 4 * (fileButtonHeight + spacing) + 27, fileList[4], ILI9341_T4_COLOR_BLACK, 14, true);
            }
        };
        addWidget(fileButton4);
    }
    drawWidgets(); // Affiche les boutons de fichiers
    Serial.println("Load screen setup complete.");
    printFreeMemory(); // Affiche la mémoire libre après le setup de l'écran de charg
}
void setupOptionsScreen() {
    clearWidgets();
    clearDisplay(ILI9341_T4_COLOR_RED);
    addBackButton();

    drawWidgets(); // Affiche les boutons d'options
}


void addBackButton() {
    Widget backButton = { 10, 10, 80, 40, onBackToMenu, [](){
        //drawRectangle(10, 10, 80, 40, 2, 0xFFFF);
        drawIcon(0, 0, back_icon, 48, 48); 
    }};
    addWidget(backButton);
}
// PLUS GENERIQUE
// Gestion des widgets
void addWidget(Widget w) {
    if (widgetCount < MAX_WIDGETS) {
        widgets[widgetCount++] = w;
    }
}

void clearWidgets() {
    widgetCount = 0;
}

void drawWidgets() {
    for (uint8_t i = 0; i < widgetCount; i++) {
        if (widgets[i].draw) widgets[i].draw();
    }
}

void handleTouchWidgets(int x, int y) {
    for (uint8_t i = 0; i < widgetCount; i++) {
        if (x >= widgets[i].x && x <= widgets[i].x + widgets[i].w &&
            y >= widgets[i].y && y <= widgets[i].y + widgets[i].h) {
            if (widgets[i].onPress) widgets[i].onPress();
        }
    }
}

// Implémentation des écrans
void setScreen(Screen* screen) {
    // clearWidgets(); // POSE PRBLM
    currentScreen = screen;
    if (currentScreen) currentScreen->draw();
    screenHasChanged = true; // Indique que l'écran a été modifié
    Serial.print("Screen set");
    printFreeMemory(); // Affiche la mémoire libre après avoir changé d'écran
}

void updateUI() {
    if (currentScreen) {
        currentScreen->draw();
    }
    Serial.println("UI updated.");
    printFreeMemory(); // Affiche la mémoire libre après la mise à jour de l'
}

void handleTouchUI(int x, int y) {
    if (currentScreen) {
        currentScreen->handleTouch(x, y);
    }
}

void checkTouch() {
    TSPoint p = ts.getPoint();
    if (p.z > 100) { // Si l'écran est touché
        uint16_t screenX, screenY;
        mapTouchToScreen(p.x, p.y, screenX, screenY);
        handleTouchUI(screenX, screenY);
        Serial.print("Touch at: ");
        Serial.print(screenX);
        Serial.print(", ");
        Serial.println(screenY);
    }
}