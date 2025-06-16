#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <stdint.h>
#include <ILI9341_T4.h>
#include <TouchScreen.h>
#include "Config.h"
#include "AudioProcessor.h"
#include "Icons.h"

// === Paramètres écran ===
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

#define BUTTON_WIDTH  200
#define BUTTON_HEIGHT 50
#define BUTTON_SPACING 20
#define BUTTON_START_Y ((SCREEN_HEIGHT - (3 * BUTTON_HEIGHT + 2 * BUTTON_SPACING)) / 2)
#define BUTTON_X ((SCREEN_WIDTH - BUTTON_WIDTH) / 2)

// TEST UI.h
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

// ---------- API publique ----------

// Max widgets par screen
#define MAX_WIDGETS 10

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

// FIN TEST UI.h

struct IconButton {
  int x, y;         // position coin haut-gauche
  int size;         // taille (carré pour simplifier)
  void (*onClick)(); // fonction appelée quand on clique dessus

  IconButton(int x_, int y_, int size_, void (*onClick_)()) 
    : x(x_), y(y_), size(size_), onClick(onClick_) {}
};

void checkTouch();
void drawButtons();
void mapTouchToScreen(int raw_x, int raw_y, int &screen_x, int &screen_y);
void initDisplay();
void clearDisplay(uint16_t color = 0);
void drawIcon(int x, int y, const uint16_t* icon, int w, int h);
void drawLine(int x0, int y0, int x1, int y1, int thickness, uint16_t color);
void drawRectangle(int x, int y, int width, int height, int thickness, uint16_t color);
void drawRectangleCentered(int x, int y, int width, int height, int thickness, uint16_t color);
void drawCircle(int x, int y, int radius, uint16_t color, bool fill, int thickness);
void drawNote(int corde, int fret, bool fill, uint16_t color);
void drawTabulation();
void getNotePosition(int corde, int fret, int &x, int &y);
void updateDisplay();
uint16_t RGB24_to_RGB565(uint32_t color);
    


#endif
