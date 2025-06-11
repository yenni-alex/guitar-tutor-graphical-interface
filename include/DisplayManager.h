#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <stdint.h>
#include <ILI9341_T4.h>
#include "Config.h"
    
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
