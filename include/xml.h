#ifndef XML_H
#define XML_H

#include <SD.h>
#include "AudioProcessor.h"
#include "globals.h"
//#define MAX_FILES 20 // Limite le nombre max de fichiers affichés pour ne pas saturer la RAM
//
//extern String fileList[MAX_FILES];
//extern uint8_t fileCount;
//
#define MAX_FILES 20
#define MAX_FILENAME_LENGTH 40

extern char fileList[MAX_FILES][MAX_FILENAME_LENGTH];
extern int fileCount;

void loadSongFromCSV(const char* filename);
void readFileList();


#endif