#include <Arduino.h>
//#include "DisplayManager.h"
#include "Config.h"
#include "AudioProcessor.h"
#include "LedController.h"
#include "xml.h"
#include "Icons.h"
#include "UI.h"
#include "globals.h"
#include <TeensyThreads.h>

LedController ledController;

uint32_t CRGBtoUint32(const CRGB & color) {
    return ((uint32_t)color.r << 16) | ((uint32_t)color.g << 8) | ((uint32_t)color.b);
}



void UpdateDisplayThread() {
  while (true) {
    //clearDisplay(ILI9341_T4_COLOR_WHITE);
    //drawTabulation();
    //drawButtons();
//
    //if (currentPlayingChordIndex < currentSong.chordCount) {
    //  Chord& chord = currentSong.chords[currentPlayingChordIndex];
    //  uint32_t color24 = CRGBtoUint32(chord.notes->color);
    //  uint16_t color565 = RGB24_to_RGB565(color24);
    //  drawNote(chord.notes->corde, chord.notes->caseFret, true, color565);
//
    //}
    //checkTouch();
    //updateDisplay();
    
    // TEST UI
    //updateUI();
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

void UpdateAudioThread() {
    while (true) {
        if (currentPlayingChordIndex < currentSong.chordCount) {
            Chord& chord = currentSong.chords[currentPlayingChordIndex];
            float freqs[Chord::MAX_NOTES] = {0};
            float ths[Chord::MAX_NOTES] = {0};
            for (uint8_t i = 0; i < chord.noteCount; ++i) {
                freqs[i] = chord.notes[i].freq;
                ths[i] = chord.notes[i].threshold;
            }
            checkNoteDetection(freqs, ths);
        }
        threads.delay(20);
    }
}

void updateLedsThread() {
    int oldChordIndex = -1;
    while (true) {
        if (currentPlayingChordIndex != oldChordIndex) {
            ledController.clear();
            Chord& chord = currentSong.chords[currentPlayingChordIndex];
            for (uint8_t i = 0; i < chord.noteCount; ++i) {
                ledController.setLed(chord.notes[i].led, chord.notes[i].color);
            }
            for (uint8_t i = NUM_LEDS - 1; i >= chord.heightOfHand; --i) {
                ledController.setLed(i, CRGB::Yellow); // ALLUME les LEDs au-dessus de la hauteur de la main
            }
            ledController.show();
            oldChordIndex = currentPlayingChordIndex;
        }
        threads.delay(40);
    }
}

void setup() {
  Serial.begin(115200);
  initDisplay();
  initAudio();
  ledController.begin();
  clearDisplay(ILI9341_T4_COLOR_WHITE);
  delay(5000);
  SD.begin(BUILTIN_SDCARD);
  Serial.println("SD card initialized");
  if (!SD.exists("/test.xml")) {
    Serial.println("Fichier test.xml introuvable");
    return;
  }
  loadSongFromXML("/test.xml");
  Serial.print("Accords lus: ");
  Serial.println(currentSong.chordCount);
  for (uint16_t i = 0; i < currentSong.chordCount; ++i) {
    Serial.print("Chord "); Serial.print(i); Serial.print(" notes: "); Serial.println(currentSong.chords[i].noteCount);
    for (uint8_t j = 0; j < currentSong.chords[i].noteCount; ++j) {
      Serial.print("  Note "); Serial.print(j);
      Serial.print(" freq: "); Serial.print(currentSong.chords[i].notes[j].freq);
      Serial.print(" led: "); Serial.print(currentSong.chords[i].notes[j].led);
      Serial.print(" color: "); Serial.print(currentSong.chords[i].notes[j].color.r, HEX);
      Serial.print(currentSong.chords[i].notes[j].color.g, HEX);
      Serial.print(currentSong.chords[i].notes[j].color.b, HEX);
      Serial.println();
    }
  }
  // Affiche le premier accord dès le début
  ledController.clear();
  if (currentSong.chordCount > 0) {
    Chord& chord = currentSong.chords[0];
    for (uint8_t i = 0; i < chord.noteCount; ++i) {
      ledController.setLed(chord.notes[i].led, chord.notes[i].color);
    }
    ledController.show();
  }


  setScreen(&menuScreen);
  updateUI();
  // FIN TEST UI

  threads.addThread(UpdateDisplayThread);
  threads.addThread(UpdateAudioThread);
  threads.addThread(updateLedsThread);
}

void loop() {
  delay(1000);
}

