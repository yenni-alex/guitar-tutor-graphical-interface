#ifndef UI_H
#define UI_H

#include "DisplayManager.h"
#include "globals.h"
#include "xml.h"

// Max widgets par screen
#define MAX_WIDGETS 10

// === Paramètres écran ===
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

#define BUTTON_WIDTH  200
#define BUTTON_HEIGHT 50
#define BUTTON_SPACING 20
#define BUTTON_START_Y ((SCREEN_HEIGHT - (3 * BUTTON_HEIGHT + 2 * BUTTON_SPACING)) / 2)
#define BUTTON_X ((SCREEN_WIDTH - BUTTON_WIDTH) / 2)

// ---------- Types de base ----------
typedef void (*DrawFunction)();
typedef void (*TouchFunction)(int x, int y);

// Screen (écran complet)
struct Screen {
    DrawFunction draw;
    TouchFunction handleTouch;
};

// Widget (bouton etc.)
struct Widget {
    int x, y, w, h;
    void (*onPress)();
    void (*draw)();
};

extern Screen menuScreen;
extern Screen resumeScreen;
extern Screen loadScreen;
extern Screen optionsScreen;

void drawMenuScreen();
void drawResumeScreen();
void drawLoadScreen();
void drawOptionsScreen();

void handleTouchMenuScreen(int x, int y);
void handleTouchResumeScreen(int x, int y);
void handleTouchLoadScreen(int x, int y);
void handleTouchOptionsScreen(int x, int y);

void setupMenuScreen();
void setupResumeScreen();
void setupLoadScreen();
void setupOptionsScreen();

void onResumePressed();
void onLoadPressed();
void onOptionsPressed();
void onBackToMenu();

void addBackButton();

// plus generique
// Gestion des widgets
void addWidget(Widget w);
void drawWidgets();
void handleTouchWidgets(int x, int y);

// Gestion des écrans
void setScreen(Screen* screen);
void updateUI();
void handleTouchUI(int x, int y);

// Doit être appelé quand tu changes d'écran pour effacer les widgets précédents
void clearWidgets();
#endif