#ifndef XML_H
#define XML_H

#include <SD.h>
#include "AudioProcessor.h"

void parseColor(const char* hex, CRGB& color);
void loadSongFromXML(const char* filename);



#endif