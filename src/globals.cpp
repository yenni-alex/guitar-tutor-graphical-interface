#include "globals.h"

bool screenHasChanged = false;
bool songLoaded = false;

extern "C" char* sbrk(int incr);
void printFreeMemory() {
    char stack_top;
    char* heap_top = sbrk(0);
    Serial.print("Heap: ");
    Serial.println((int)heap_top, HEX);
    Serial.print("Stack: ");
    Serial.println((int)&stack_top, HEX);
    Serial.print("Free memory estimate: ");
    Serial.println((int)&stack_top - (int)heap_top);
}

uint32_t CRGBtoUint32(const CRGB & color) {
    return ((uint32_t)color.r << 16) | ((uint32_t)color.g << 8) | ((uint32_t)color.b);
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

CRGB parseColorLed(int colorInt) {
    // Si c'est un code hexadécimal (commence par un chiffre ou A-F)
    CRGB color;
    switch (colorInt) 
    {
    case 0:
        color.r = 0; color.g = 0; color.b = 0; // Noir
        break;
    case 1:
        color.r = 255; color.g = 0; color.b = 0; // Rouge
        break;
    case 2:
        color.r = 0; color.g = 255; color.b = 0; // Vert
        break;
    case 3:
        color.r = 0; color.g = 0; color.b = 255; // Bleu
        break;
    case 4:
        color.r = 255; color.g = 255; color.b = 0; // Jaune
        break; 
    case 5:
        color.r = 255; color.g = 0; color.b = 255; // Magenta
        break;
    case 6:
        color.r = 0; color.g = 255; color.b = 255; // Cyan
        break;
    
    default:
        color.r = 0; color.g = 0; color.b = 0; // Noir par défaut
        break;
    }
    return color;
}

uint16_t parseColorDisplay(int colorInt) {
    // Si c'est un code hexadécimal (commence par un chiffre ou A-F)
    uint16_t color;
    switch (colorInt) 
    {
    case 0:
        color = ILI9341_T4_COLOR_BLACK; // Noir
        break;
    case 1:
        color = ILI9341_T4_COLOR_RED; // Rouge
        break;
    case 2:
        color = ILI9341_T4_COLOR_GREEN; // Vert
        break;
    case 3:
        color = ILI9341_T4_COLOR_BLUE; // Bleu
        break;
    case 4:
        color = ILI9341_T4_COLOR_YELLOW; // Jaune
        break; 
    case 5:
        color = ILI9341_T4_COLOR_MAJENTA; // Magenta
        break;
    case 6:
        color = ILI9341_T4_COLOR_CYAN; // Cyan
        break;
    
    default:
        color = ILI9341_T4_COLOR_BLACK; // Noir par défaut
        break;
    }
    return color;
}
