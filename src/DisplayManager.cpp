#include "DisplayManager.h"


ILI9341_T4::ILI9341Driver tft(PIN_CS, PIN_DC, PIN_SCK, PIN_MOSI, PIN_MISO, PIN_RESET, PIN_TOUCH_CS, PIN_TOUCH_IRQ);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

DMAMEM ILI9341_T4::DiffBuffStatic<6000> diff1;
DMAMEM ILI9341_T4::DiffBuffStatic<6000> diff2;

DMAMEM uint16_t internal_fb[H * W]; 
DMAMEM uint16_t fb[H * W]; 

        // UI.cpp
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
        }
    };
    addWidget(resumeButton);

    Widget loadButton = {
        BUTTON_X, BUTTON_START_Y + BUTTON_HEIGHT + BUTTON_SPACING, BUTTON_WIDTH, BUTTON_HEIGHT,
        onLoadPressed,
        [](){
            drawRectangle(BUTTON_X, BUTTON_START_Y + BUTTON_HEIGHT + BUTTON_SPACING, BUTTON_WIDTH, BUTTON_HEIGHT, 2, ILI9341_T4_COLOR_YELLOW);
        }
    };
    addWidget(loadButton);

    Widget optionsButton = {
        BUTTON_X, BUTTON_START_Y + 2 * (BUTTON_HEIGHT + BUTTON_SPACING), BUTTON_WIDTH, BUTTON_HEIGHT,
        onOptionsPressed,
        [](){
            drawRectangle(BUTTON_X, BUTTON_START_Y + 2 * (BUTTON_HEIGHT + BUTTON_SPACING), BUTTON_WIDTH, BUTTON_HEIGHT, 2, ILI9341_T4_COLOR_RED);
        }
    };
    addWidget(optionsButton);
}
void setupResumeScreen() {
    clearWidgets();
    clearDisplay(ILI9341_T4_COLOR_BLUE);
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
// Implémentation des widgets
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
//      FIN de UI.cpp
// fin de structe TODO a tester encore et adapter
void onRestart() {
  Serial.println("Restart clicked");
  currentPlayingChordIndex = 0; // Réinitialiser l'index du chord en cours
}
void onSettings() {
  Serial.println("Settings clicked");
    // Implémentez la logique pour ouvrir les paramètres
    
}

IconButton restart(120, 0, 48, onRestart); // Bouton de redémarrage
IconButton settings(0, 190, 48, onSettings); // Bouton de paramètres

void drawButtons() {
    drawIcon(W - 50, 0, play_icon, 48, 48); // play
    drawIcon(W - 100, 0, pause_icon, 48, 48); // pause
    drawIcon(W - 150, 0, stop_icon, 48, 48); // stop
    drawIcon(W - 200, 0, restart_icon, 48, 48); // restart
    drawIcon(0, H - 50, settings_icon, 48, 48); // settings
}
// Fonction de map + inversion des axes
void mapTouchToScreen(int raw_x, int raw_y, int &screen_x, int &screen_y) {
    // Inversion des axes : X tactile -> Y écran, Y tactile -> X écran
    screen_x = map(raw_y, TS_MINY, TS_MAXY, 0, W);
    screen_y = map(raw_x, TS_MINX, TS_MAXX, H, 0);
    
    // Limiter les débordements éventuels
    screen_x = constrain(screen_x, 0, W - 1);
    screen_y = constrain(screen_y, 0, H - 1);
}

void checkTouch() {
    TSPoint p = ts.getPoint();
    if (p.z > 100) { // Adjust threshold as needed
        int x, y;
        // Map touch coordinates to screen coordinates
        mapTouchToScreen(p.x, p.y, x, y);
        // Handle touch event
        Serial.printf("Touched at: (%d, %d)\n", x, y);
        handleTouchUI(x, y); // TEST UI
        //if(x >= restart.x && x <= restart.x + restart.size &&
        //   y >= restart.y && y <= restart.y + restart.size) {
        //    restart.onClick(); // Call the restart function if the button is touched
        //}
        //if(x >= settings.x && x <= settings.x + settings.size &&
        //   y >= settings.y && y <= settings.y + settings.size) {
        //    settings.onClick(); // Call the settings function if the button is touched
        //}
        // Add your touch handling logic here
        
    }
}

uint16_t RGB24_to_RGB565(uint32_t color24) {
    uint8_t r = (color24 >> 16) & 0xFF;
    uint8_t g = (color24 >> 8) & 0xFF;
    uint8_t b = color24 & 0xFF;

    uint16_t r5 = (r * 31 + 15) / 255;
    uint16_t g6 = (g * 63 + 31) / 255;
    uint16_t b5 = (b * 31 + 15) / 255;

    uint16_t color565 = (r5 << 11) | (g6 << 5) | b5;

    return color565;  // PAS DE SWAP !!!
}

void initDisplay() {
    //tft.output(&Serial);                // output debug infos to serial port.  
    while (!tft.begin(SPI_SPEED_DISPLAY));      // init the display
    tft.setRotation(1);                 // start in portrait mode 240x320
    tft.setFramebuffer(internal_fb);    // set the internal framebuffer (enables double buffering)
    tft.setDiffBuffers(&diff1, &diff2); // set 2 diff buffers -> enables diffenrential updates. 
    tft.setRefreshRate(90);  // start with a screen refresh rate around 40hz
    
}
void clearDisplay(uint16_t color) {
    for (int i = 0; i < H * W; i++) fb[i] = color;
}

void drawIcon(int x, int y, const uint16_t* icon, int w, int h) {
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            int px = x + i;
            int py = y + j;
            if (px >= 0 && px < W && py >= 0 && py < H) {
                fb[py * W + px] = icon[j * w + i];
            }
        }
    }
}

void drawLine(int x0, int y0, int x1, int y1, int thickness, uint16_t color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    for (int i = -thickness / 2; i <= thickness / 2; i++) {
        int x = x0, y = y0;
        int errTemp = err;
        while (true) {
            int drawX = x;
            int drawY = y + i;

            // Vérifie que le pixel est dans les limites de l'écran
            if (drawX >= 0 && drawX < W && drawY >= 0 && drawY < H) {
                fb[drawY * W + drawX] = color;
            }

            if (x == x1 && y == y1) break;
            e2 = errTemp;
            if (e2 > -dx) { errTemp -= dy; x += sx; }
            if (e2 < dy) { errTemp += dx; y += sy; }
        }
    }
}


void drawRectangle(int x, int y, int width, int height, int thickness, uint16_t color) {
    for (int i = 0; i < thickness; i++) {
        drawLine(x, y + i, x + width - 1, y + i, 1, color);
        drawLine(x, y + height - 1 - i, x + width - 1, y + height - 1 - i, 1, color);
        drawLine(x + i, y, x + i, y + height - 1, 1, color);
        drawLine(x + width - 1 - i, y, x + width - 1 - i, y + height - 1, 1, color);
    }
}

void drawRectangleCentered(int x, int y, int width, int height, int thickness, uint16_t color) {
    
    // Calcul du coin supérieur gauche à partir du centre
    int startX = x - width / 2;
    int startY = y - height / 2;

    for (int i = 0; i < thickness; i++) {
        // Lignes horizontales
        drawLine(startX, startY + i, startX + width - 1, startY + i, 1, color);
        drawLine(startX, startY + height - 1 - i, startX + width - 1, startY + height - 1 - i, 1, color);
        // Lignes verticales
        drawLine(startX + i, startY, startX + i, startY + height - 1, 1, color);
        drawLine(startX + width - 1 - i, startY, startX + width - 1 - i, startY + height - 1, 1, color);
    }
}

void drawCircle(int x, int y, int radius, uint16_t color, bool fill, int thickness) {
    if (fill) {
        for (int i = -radius; i <= radius; i++) {
            for (int j = -radius; j <= radius; j++) {
                if (i * i + j * j <= radius * radius) {
                    int px = x + i;
                    int py = y + j;
                    if (px >= 0 && px < W && py >= 0 && py < H) {
                        fb[py * W + px] = color;
                    }
                }
            }
        }
    }
    else {
        for (int t = 0; t < thickness; t++) {
            int r = radius - t;
            if (r < 0) break;

            int dx = r - 1;
            int dy = 0;
            int err = 1 - dx;

            while (dx >= dy) {
                auto setPixel = [&](int px, int py) {
                    if (px >= 0 && px < W && py >= 0 && py < H) {
                        fb[py * W + px] = color;
                    }
                };

                setPixel(x + dx, y + dy);
                setPixel(x - dx, y + dy);
                setPixel(x + dx, y - dy);
                setPixel(x - dx, y - dy);
                setPixel(x + dy, y + dx);
                setPixel(x - dy, y + dx);
                setPixel(x + dy, y - dx);
                setPixel(x - dy, y - dx);

                dy++;
                if (err <= 0) {
                    err += 2 * dy + 1;
                }
                else {
                    dx--;
                    err += 2 * (dy - dx) + 1;
                }
            }
        }
    }
}
void drawNote(int corde, int fret, bool fill, uint16_t color) {
    int x, y;
    y = H - TOP_BORDER - (corde - 1) * CORDS_ECART;
    if (fret == 0) {
        x = W - LEFT_BORDER;
    }
    else if (fret == 1) {
        x = W - LEFT_BORDER - 0.5 * FRET_ECART;
    }
    else {
        x = W - LEFT_BORDER - 0.5 * FRET_ECART - (fret - 1) * FRET_ECART;
    }

    drawCircle(x, y, 10,color, fill, 1);
}

void drawTabulation() {

    drawRectangle(RIGHT_BORDER, BOTTOM_BORDER, W -(RIGHT_BORDER + LEFT_BORDER), H -(TOP_BORDER + BOTTOM_BORDER), 3, ILI9341_T4_COLOR_BLACK);
    
    // cords lines
    drawLine(RIGHT_BORDER, BOTTOM_BORDER + CORDS_ECART, W - LEFT_BORDER - 1, BOTTOM_BORDER + CORDS_ECART, 3, ILI9341_T4_COLOR_BLACK);
    drawLine(RIGHT_BORDER, BOTTOM_BORDER + 2 * CORDS_ECART, W - LEFT_BORDER - 1, BOTTOM_BORDER + 2 * CORDS_ECART, 3, ILI9341_T4_COLOR_BLACK);
    drawLine(RIGHT_BORDER, BOTTOM_BORDER + 3 * CORDS_ECART, W - LEFT_BORDER - 1, BOTTOM_BORDER + 3 * CORDS_ECART, 3, ILI9341_T4_COLOR_BLACK);
    drawLine(RIGHT_BORDER, BOTTOM_BORDER + 4 * CORDS_ECART, W - LEFT_BORDER - 1, BOTTOM_BORDER + 4 * CORDS_ECART, 3, ILI9341_T4_COLOR_BLACK);

    // frets lines
    drawLine(RIGHT_BORDER + FRET_ECART, BOTTOM_BORDER, RIGHT_BORDER + FRET_ECART, H - TOP_BORDER - 1, 1, ILI9341_T4_COLOR_BLACK);
    drawLine(RIGHT_BORDER + 2 * FRET_ECART, BOTTOM_BORDER, RIGHT_BORDER + 2 * FRET_ECART, H - TOP_BORDER - 1, 1, ILI9341_T4_COLOR_BLACK);
    drawLine(RIGHT_BORDER + 3 * FRET_ECART, BOTTOM_BORDER, RIGHT_BORDER + 3 * FRET_ECART, H - TOP_BORDER - 1, 1, ILI9341_T4_COLOR_BLACK);
}

void getNotePosition(int corde, int fret, int &x, int &y) {
  y = W - LEFT_BORDER - (corde - 1) * CORDS_ECART;
  if (fret == 0) {
    x = H - TOP_BORDER;
  } else if (fret == 1) {
    x = H - TOP_BORDER - 0.5 * FRET_ECART;
  } else {
    x = H - TOP_BORDER - 0.5 * FRET_ECART - (fret - 1) * FRET_ECART;
  }
}

void updateDisplay() {
    tft.update(fb);
}



