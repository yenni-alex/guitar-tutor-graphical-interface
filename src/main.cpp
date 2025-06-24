#include <Arduino.h>
//#include "DisplayManager.h"
#include "Config.h"
#include "xml.h"
#include "Icons.h"
#include "UI.h"
#include "globals.h"
#include <TeensyThreads.h>


void UpdateDisplayThread() {
  while (true) {
    
    // Gestion tactile ici
    checkTouch();
    if(screenHasChanged) {
      screenHasChanged = false;
      updateDisplay();
    }
    //updateDisplay();
    // FIN TEST UI
    threads.delay(20);
  }
}




void setup() {
  Serial.begin(115200);
  initDisplay();
  clearDisplay(ILI9341_T4_COLOR_WHITE);
  delay(5000);
  Serial.println("Setup started.");
  printFreeMemory(); // Affiche la mémoire libre au démarrage
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Erreur : SD non détectée");
    return;
  }
  Serial.println("SD card initialized");
  readFileList(); // Lit la liste des fichiers XML
  Serial.println("File list read.");
  printFreeMemory(); // Affiche la mémoire libre après l'initialisation de la SD

  setScreen(&menuScreen);
  updateUI();
  Serial.println("UI initialized.");
  printFreeMemory(); // Affiche la mémoire libre après l'initialisation de l'UI
  // FIN TEST UI

  threads.addThread(UpdateDisplayThread);
  
}



void loop() {
  delay(1000);
}

