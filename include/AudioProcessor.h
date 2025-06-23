#ifndef AUDIO_PROCESSOR_H
#define AUDIO_PROCESSOR_H

#include <Arduino.h>
#include <Audio.h>
#include "Config.h"
#include <arm_math.h>
#include <arm_const_structs.h>  // Contient arm_cfft_sR_f32_len4096
#include <algorithm>  // pour std::sort
#include <Audio.h>
#include <FastLED.h>
#include "globals.h"

class Note {
public:
    float freq;         // Fréquence de la note
    float threshold;    // Seuil de détection pour cette note
    CRGB ledColor;
    uint16_t displayColor; // Couleur de la note pour l'affichage (en RGB565)
    int colorInt;      // Couleur de la note (en entier RGB)
    int led;            // Numéro de la LED
    int corde;          // Numéro de la corde
    int caseFret;       // Numéro de la case
    //Note(float f = 0, float t = 0, CRGB c = 0, int l = -1, int co = -1, int ca = -1)
    //    : freq(f), threshold(t), color(c), led(l), corde(co), caseFret(ca) {}
    Note(float f = 0, float t = 0, int c = 0, int l = -1, int co = -1, int ca = -1)
        : freq(f), threshold(t), colorInt(c), led(l), corde(co), caseFret(ca) {
            ledColor = parseColorLed(colorInt); // Convertit l'entier en CRGB
            displayColor = parseColorDisplay(colorInt); // Convertit l'entier RGB
        }
    
    // Opérateur d'assignation explicite

};

class Chord {
public:
    static const uint8_t MAX_NOTES = 6; // Un accord peut avoir jusqu'à 6 notes (une par corde)
    Note notes[MAX_NOTES];
    uint8_t noteCount; // Nombre de notes dans l'accord
    int heightOfHand;  // Hauteur de la main sur le manche
    int time;          // Temps de décalage par rapport à l'accord précédent
    Chord() : noteCount(0), heightOfHand(0), time(0) {}
};

class Song {
public:
    char name[32]; // Nom de la chanson
    int numChords; // Nombre d'accords dans la chanson
    int bpm;       // Tempo de la chanson en battements par minute
    static const uint16_t MAX_CHORDS = 64; // Limite mémoire (modifiable)
    Chord chords[MAX_CHORDS];
    uint16_t chordCount; // Nombre d'accords dans la chanson
    Song() : chordCount(0) {}
};

extern Song currentSong;
extern const int totalChords;

void initAudio();
bool checkNoteDetection(float frequencies[6], float thresholds[6]);


#endif