#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
extern bool screenHasChanged;

extern "C" char* sbrk(int incr);
void printFreeMemory();
#endif