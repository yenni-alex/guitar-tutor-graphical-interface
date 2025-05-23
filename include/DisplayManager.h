#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <stdint.h>
#include <ILI9341_T4.h>
#include "Config.h"

class DisplayManager {
    private:
        ILI9341_T4::ILI9341Driver tft;
        ILI9341_T4::DiffBuffStatic<6000> diff1;
        ILI9341_T4::DiffBuffStatic<6000> diff2;

        uint16_t internal_fb[H * W]; //__attribute__((section(".dmadata")));
        uint16_t fb[H * W]; // framebuffer for the display

    public:
        DisplayManager();
        void init();
        void clear(uint16_t color = 0);
        void drawLine(int x0, int y0, int x1, int y1, int thickness, uint16_t color);
        void drawRectangle(int x, int y, int width, int height, int thickness, uint16_t color);
        void drawRectangleCentered(int x, int y, int width, int height, int thickness, uint16_t color);
        void drawCircle(int x, int y, int radius, uint16_t color, bool fill, int thickness);
        void drawNote(int corde, int fret, bool fill, uint16_t color);
        void drawTabulation();
        void getNotePosition(int corde, int fret, int &x, int &y);
        void update();
    
};

#endif
