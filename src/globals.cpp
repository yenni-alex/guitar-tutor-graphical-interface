#include "globals.h"

bool screenHasChanged = false;

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
