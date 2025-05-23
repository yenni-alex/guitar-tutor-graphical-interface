
#include "xml.h"

void parseColor(const char* hex, CRGB& color) {
    unsigned long val = strtoul(hex, nullptr, 16);
    color.r = (val >> 16) & 0xFF;
    color.g = (val >> 8) & 0xFF;
    color.b = val & 0xFF;
}

void loadSongFromXML(const char* filename) {
    File file = SD.open(filename);
    if (!file) {
        Serial.println("Erreur ouverture fichier XML");
        return;
    }
    currentSong.chordCount = 0;
    Chord* chord = nullptr;
    char line[128];
    while (file.available()) {
        file.readBytesUntil('\n', line, sizeof(line) - 1);
        line[sizeof(line) - 1] = 0;
        // Début d'un accord
        if (strstr(line, "<chord")) {
            if (currentSong.chordCount < Song::MAX_CHORDS) {
                chord = &currentSong.chords[currentSong.chordCount++];
                chord->noteCount = 0;
                sscanf(line, "<chord time=\"%d\" heightOfHand=\"%d\">", &chord->time, &chord->heightOfHand);
            }
        }
        // Note
        else if (strstr(line, "<note") && chord && chord->noteCount < Chord::MAX_NOTES) {
            float freq = 0, threshold = 0;
            int led = -1, corde = -1, caseFret = -1;
            char colorHex[7] = "000000";
            sscanf(line, "<note freq=\"%f\" threshold=\"%f\" color=\"%6s\" led=\"%d\" corde=\"%d\" caseFret=\"%d\"/>",
                   &freq, &threshold, colorHex, &led, &corde, &caseFret);
            CRGB color;
            parseColor(colorHex, color);
            chord->notes[chord->noteCount++] = Note(freq, threshold, color, led, corde, caseFret);
        }
    }
    file.close();
}