#include "DisplayManager.h"


ILI9341_T4::ILI9341Driver tft(PIN_CS, PIN_DC, PIN_SCK, PIN_MOSI, PIN_MISO, PIN_RESET, PIN_TOUCH_CS, PIN_TOUCH_IRQ);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

DMAMEM ILI9341_T4::DiffBuffStatic<6000> diff1;
DMAMEM ILI9341_T4::DiffBuffStatic<6000> diff2;

DMAMEM uint16_t internal_fb[H * W]; 
DMAMEM uint16_t fb[H * W]; 


// Fonction de map + inversion des axes
void mapTouchToScreen(uint16_t raw_x, uint16_t raw_y, uint16_t &screen_x, uint16_t &screen_y) {
    // Inversion des axes : X tactile -> -Y écran, Y tactile -> X écran (Pour rotation 1)
    //screen_x = map(raw_y, TS_MINY, TS_MAXY, 0, W);
    //screen_y = map(raw_x, TS_MINX, TS_MAXX, H, 0);
    // Inversion des axes : X tactile -> Y écran, Y tactile -> -X écran (Pour rotation 3 et text à l'endroit)
    screen_x = map(raw_y, TS_MINY, TS_MAXY, W, 0);
    screen_y = map(raw_x, TS_MINX, TS_MAXX, 0, H);
    // Limiter les débordements éventuels
    screen_x = constrain(screen_x, 0, W - 1);
    screen_y = constrain(screen_y, 0, H - 1);
}

void initDisplay() {
    //tft.output(&Serial);                // output debug infos to serial port.  
    while (!tft.begin(SPI_SPEED_DISPLAY));      // init the display
    tft.setRotation(3);                 // 3 pour que le texte soit à l'endroit (landscape 320x240) TODO: voir si ca fou pas trop la merde dans drawTAB
    tft.setFramebuffer(internal_fb);    // set the internal framebuffer (enables double buffering)
    tft.setDiffBuffers(&diff1, &diff2); // set 2 diff buffers -> enables diffenrential updates. 
    tft.setRefreshRate(90);  // start with a screen refresh rate around 40hz
    
}
void clearDisplay(uint16_t color) {
    for (int i = 0; i < H * W; i++) fb[i] = color;
}

void drawIcon(uint16_t x, uint16_t y, const uint16_t* icon, uint16_t w, uint16_t h) {
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

void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t thickness, uint16_t color) {
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


void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t thickness, uint16_t color) {
    for (int i = 0; i < thickness; i++) {
        drawLine(x, y + i, x + width - 1, y + i, 1, color);
        drawLine(x, y + height - 1 - i, x + width - 1, y + height - 1 - i, 1, color);
        drawLine(x + i, y, x + i, y + height - 1, 1, color);
        drawLine(x + width - 1 - i, y, x + width - 1 - i, y + height - 1, 1, color);
    }
}

void drawRectangleCentered(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t thickness, uint16_t color) {
    
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

void drawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color, bool fill, uint16_t thickness) {
    if (fill) {
        for (uint8_t i = -radius; i <= radius; i++) {
            for (uint8_t j = -radius; j <= radius; j++) {
                if (i * i + j * j <= radius * radius) {
                    uint16_t px = x + i;
                    uint16_t py = y + j;
                    if (px >= 0 && px < W && py >= 0 && py < H) {
                        fb[py * W + px] = color;
                    }
                }
            }
        }
    }
    else {
        for (uint8_t t = 0; t < thickness; t++) {
            uint16_t r = radius - t;
            if (r < 0) break;

            uint16_t dx = r - 1;
            uint16_t dy = 0;
            uint16_t err = 1 - dx;

            while (dx >= dy) {
                auto setPixel = [&](uint16_t px, uint16_t py) {
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
void drawNote(uint16_t corde, uint16_t fret, bool fill, uint16_t color) {
    uint16_t x, y;
    y = BOTTOM_BORDER + (corde - 1) * CORDS_ECART;
    if (fret == 0) {
        x = LEFT_BORDER;
    }
    else if (fret == 1) {
        x = LEFT_BORDER + 0.5 * FRET_ECART;
    }
    else {
        x = LEFT_BORDER + 0.5 * FRET_ECART + (fret - 1) * FRET_ECART;
    }

    drawCircle(x, y, 10,color, fill, 1);
    // update region for the note
    int startX = x - 10;
    int endX = x + 10;
    int startY = y - 10;
    int endY = y + 10;
    if (startX < 0) startX = 0;
    if (endX >= W) endX = W - 1;
    if (startY < 0) startY = 0;
    if (endY >= H) endY = H - 1;
    

}

void drawTabulation() {

    drawRectangle(LEFT_BORDER, BOTTOM_BORDER, W -(RIGHT_BORDER + LEFT_BORDER), H -(TOP_BORDER + BOTTOM_BORDER), 3, ILI9341_T4_COLOR_BLACK);
    
    // cords lines
    drawLine(LEFT_BORDER, BOTTOM_BORDER + CORDS_ECART, W - RIGHT_BORDER - 1, BOTTOM_BORDER + CORDS_ECART, 3, ILI9341_T4_COLOR_BLACK);
    drawLine(LEFT_BORDER, BOTTOM_BORDER + 2 * CORDS_ECART, W - RIGHT_BORDER - 1, BOTTOM_BORDER + 2 * CORDS_ECART, 3, ILI9341_T4_COLOR_BLACK);
    drawLine(LEFT_BORDER, BOTTOM_BORDER + 3 * CORDS_ECART, W - RIGHT_BORDER - 1, BOTTOM_BORDER + 3 * CORDS_ECART, 3, ILI9341_T4_COLOR_BLACK);
    drawLine(LEFT_BORDER, BOTTOM_BORDER + 4 * CORDS_ECART, W - RIGHT_BORDER - 1, BOTTOM_BORDER + 4 * CORDS_ECART, 3, ILI9341_T4_COLOR_BLACK);

    // frets lines
    drawLine(LEFT_BORDER + FRET_ECART, BOTTOM_BORDER, LEFT_BORDER + FRET_ECART, H - TOP_BORDER - 1, 1, ILI9341_T4_COLOR_BLACK);
    drawLine(LEFT_BORDER + 2 * FRET_ECART, BOTTOM_BORDER, LEFT_BORDER + 2 * FRET_ECART, H - TOP_BORDER - 1, 1, ILI9341_T4_COLOR_BLACK);
    drawLine(LEFT_BORDER + 3 * FRET_ECART, BOTTOM_BORDER, LEFT_BORDER + 3 * FRET_ECART, H - TOP_BORDER - 1, 1, ILI9341_T4_COLOR_BLACK);
}

void getNotePosition(uint8_t corde, uint8_t fret, uint16_t &x, uint16_t &y) {
  y = W - LEFT_BORDER - (corde - 1) * CORDS_ECART;
  if (fret == 0) {
    x = H - TOP_BORDER;
  } else if (fret == 1) {
    x = H - TOP_BORDER - 0.5 * FRET_ECART;
  } else {
    x = H - TOP_BORDER - 0.5 * FRET_ECART - (fret - 1) * FRET_ECART;
  }
}

void writeText(uint16_t x, uint16_t y, const char* text, uint16_t color, uint8_t fontSize, bool centered) {
    
    if (centered) {
        tft.printTextCentered(fb, text, x, y, fontSize, color); // draw centered text
    }
    else{
        tft.printText(fb, text, x, y, fontSize, color); // draw text with specific color
    }    
}

void updateDisplay() {
    tft.update(fb);
}



