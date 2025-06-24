#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <stdint.h>
#include <ILI9341_T4.h>
#include <TouchScreen.h>
#include "Config.h"
#include "Icons.h"

extern TouchScreen ts;

void checkTouch();
void drawButtons();
void mapTouchToScreen(uint16_t raw_x, uint16_t raw_y, uint16_t &screen_x, uint16_t &screen_y);
void initDisplay();
void clearDisplay(uint16_t color = 0);
void drawIcon(uint16_t x, uint16_t y, const uint16_t* icon, uint16_t w, uint16_t h);
void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t thickness, uint16_t color);
void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t thickness, uint16_t color);
void drawRectangleCentered(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t thickness, uint16_t color);
void drawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color, bool fill, uint16_t thickness);
void drawNote(uint16_t corde, uint16_t fret, bool fill, uint16_t color);
void drawTabulation();
void getNotePosition(uint8_t corde, uint8_t fret, uint16_t &x, uint16_t &y);
void updateDisplay();
void writeText(uint16_t x, uint16_t y, const char* text, uint16_t color, uint8_t fontSize, bool centered = false);



#endif
