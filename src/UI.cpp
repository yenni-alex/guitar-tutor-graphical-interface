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
    drawWidgets();
}
void drawLoadScreen() {
    setupLoadScreen(); // Setup les boutons de chargement
    drawWidgets();
}
void drawOptionsScreen() {
    setupOptionsScreen(); // Setup les options
    drawWidgets();
}
void drawMenuScreen() {
    setupMenuScreen(); // Setup les boutons du menu
    drawWidgets();
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

// === Callbacks des boutons ===
void onResumePressed() { setScreen(&resumeScreen); }
void onLoadPressed()    { setScreen(&loadScreen); }
void onOptionsPressed() { setScreen(&optionsScreen); }
void onBackToMenu() { setScreen(&menuScreen); }

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
}
void setupResumeScreen() {
    clearWidgets();
    clearDisplay(ILI9341_T4_COLOR_BLUE);
    drawTabulation();
    addBackButton();
}
void setupLoadScreen() {
    clearWidgets();
    clearDisplay(ILI9341_T4_COLOR_YELLOW);
    addBackButton();
}
void setupOptionsScreen() {
    clearWidgets();
    clearDisplay(ILI9341_T4_COLOR_RED);
    addBackButton();
}


void addBackButton() {
    Widget backButton = { 10, 10, 80, 40, onBackToMenu, [](){
        drawRectangle(10, 10, 80, 40, 2, 0xFFFF);
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
    for (int i = 0; i < widgetCount; i++) {
        if (widgets[i].draw) widgets[i].draw();
    }
}

void handleTouchWidgets(int x, int y) {
    for (int i = 0; i < widgetCount; i++) {
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
}

void updateUI() {
    if (currentScreen) {
        currentScreen->draw();
    }
}

void handleTouchUI(int x, int y) {
    if (currentScreen) {
        currentScreen->handleTouch(x, y);
    }
}

void checkTouch() {
    TSPoint p = ts.getPoint();
    if (p.z > 100) { // Si l'écran est touché
        int screenX, screenY;
        mapTouchToScreen(p.x, p.y, screenX, screenY);
        handleTouchUI(screenX, screenY);
    }
}