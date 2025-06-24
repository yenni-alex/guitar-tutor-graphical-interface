#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <ILI9341_t4.h>
extern bool screenHasChanged;
extern bool songLoaded;

extern "C" char* sbrk(int incr);
void printFreeMemory();

#endif