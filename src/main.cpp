#include <Arduino.h>
#include "DisplayManager.h"
#include "Config.h"
#include "AudioProcessor.h"
#include "LedController.h"
#include <TeensyThreads.h>

DisplayManager display;
LedController ledController;

void UpdateDisplayThread() {
  while (true) {
    display.clear(ILI9341_T4_COLOR_GREEN);
    display.drawTabulation();
    display.update();
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
            ledController.show();
            oldChordIndex = currentPlayingChordIndex;
        }
        threads.delay(40);
    }
}

void setup() {
  Serial.begin(115200);
  display.init();
  initAudio();
  ledController.begin();
  display.clear(ILI9341_T4_COLOR_WHITE);

  currentSong.chordCount = 3;

  // Premier accord : Do majeur (C) sur la 5e corde, case 3
  Chord& chord1 = currentSong.chords[0];
  chord1.noteCount = 1;
  chord1.notes[0] = Note(130.81f, 0.03f, CRGB::Red, 39, 5, 3); // freq, threshold, color, led, corde, caseFret
  chord1.heightOfHand = 51;
  chord1.time = 2000;

  // Deuxième accord : Mi majeur (E) sur la 4e corde, case 2 et 3e corde, case 1
  Chord& chord2 = currentSong.chords[1];
  chord2.noteCount = 2;
  chord2.notes[0] = Note(164.81f, 0.03f, CRGB::Green, 44, 4, 2);
  chord2.notes[1] = Note(196.00f, 0.03f, CRGB::Green, 45, 3, 0);
  chord2.heightOfHand = 51;
  chord2.time = 2000;

  // Troisième accord : La majeur (A) sur la 6e corde, case 5 et 5e corde, case 7
  Chord& chord3 = currentSong.chords[2];
  chord3.noteCount = 2;
  chord3.notes[0] = Note(220.00f, 0.03f, CRGB::Red, 46, 6, 5);
  chord3.notes[1] = Note(246.94f, 0.03f, CRGB::Red, 47, 5, 7);
  chord3.heightOfHand = 51;
  chord3.time = 2000;

  threads.addThread(UpdateDisplayThread);
  threads.addThread(UpdateAudioThread);
  threads.addThread(updateLedsThread);
}

void loop() {
  delay(1000);
}

