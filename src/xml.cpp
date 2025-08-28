#include "xml.h"



char fileList[MAX_FILES][MAX_FILENAME_LENGTH];  // définition unique
int fileCount = 0;

void readFileList() {
  fileCount = 0;
  File root = SD.open("/");
  if (!root || !root.isDirectory()) {
    Serial.println("Erreur ouverture racine");
    return;
  }

  File file = root.openNextFile();
  while (file && fileCount < MAX_FILES) {
    if (!file.isDirectory()) {
      const char* name = file.name();
      size_t len = strlen(name);
      if (len < MAX_FILENAME_LENGTH && strstr(name, ".xml")) {
        strncpy(fileList[fileCount], name, MAX_FILENAME_LENGTH - 1);
        fileList[fileCount][MAX_FILENAME_LENGTH - 1] = '\0';  // Null-terminate
        fileCount++;
      }
    }
    file.close();
    file = root.openNextFile();
  }

  root.close();
  Serial.println("File list read");
}