#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <FastLED.h>
#include <ILI9341_t4.h>
extern bool screenHasChanged;
extern bool songLoaded;

extern "C" char* sbrk(int incr);
void printFreeMemory();

CRGB parseColorLed(int colorInt);
uint16_t parseColorDisplay(int colorInt);
#endif