#include "xml.h"

void parseColor(const char* hex, CRGB& color) {
    unsigned long val = strtoul(hex, nullptr, 16);
    color.r = (val >> 16) & 0xFF;
    color.g = (val >> 8) & 0xFF;
    color.b = val & 0xFF;
}

static bool extractAttr(const char* line, const char* key, char* out, size_t outlen) {
    const char* p = strstr(line, key);
    if (!p) return false;
    p += strlen(key);
    while (*p == ' ' || *p == '=') ++p;
    if (*p == '"') ++p;
    size_t i = 0;
    while (*p && *p != '"' && i < outlen-1) out[i++] = *p++;
    out[i] = 0;
    return i > 0;
}

void loadSongFromXML(const char* filename) {
    File file = SD.open(filename);
    if (!file) {
        Serial.println("Erreur ouverture fichier XML");
        return;
    }
    Serial.println("Fichier XML ouvert avec succès");
    currentSong.chordCount = 0;
    Chord* chord = nullptr;
    char line[128];
    bool songEnd = false;
    while (true) {
        int len = file.readBytesUntil('\n', line, sizeof(line) - 1);
        line[len] = '\0';
        Serial.print("Ligne lue : ");
        Serial.println(line);
        if (strstr(line, "</song>")) {
            Serial.println("Fin de la lecture du fichier XML");
            songEnd = true;
            break;
        }
        // Début d'un accord
        if (strstr(line, "<chord")) {
            Serial.println("lecture d'un accord");
            if (currentSong.chordCount < Song::MAX_CHORDS) {
                chord = &currentSong.chords[currentSong.chordCount++];
                chord->noteCount = 0;
                char buf[16];
                chord->time = 0; chord->heightOfHand = 0;
                if (extractAttr(line, "time", buf, sizeof(buf))) chord->time = atoi(buf);
                if (extractAttr(line, "heightOfHand", buf, sizeof(buf))) chord->heightOfHand = atoi(buf);
            }
        }
        // Note
        else if (strstr(line, "<note") && chord && chord->noteCount < Chord::MAX_NOTES) {
            Serial.println("lecture d'une note");
            float freq = 0, threshold = 0;
            int led = -1, corde = -1, caseFret = -1;
            char colorHex[8] = "000000";
            char buf[16];
            if (extractAttr(line, "freq", buf, sizeof(buf))) freq = atof(buf);
            if (extractAttr(line, "threshold", buf, sizeof(buf))) threshold = atof(buf);
            if (extractAttr(line, "color", colorHex, sizeof(colorHex))) {/* ok */}
            if (extractAttr(line, "led", buf, sizeof(buf))) led = atoi(buf);
            if (extractAttr(line, "corde", buf, sizeof(buf))) corde = atoi(buf);
            if (extractAttr(line, "caseFret", buf, sizeof(buf))) caseFret = atoi(buf);
            CRGB color;
            parseColor(colorHex, color);
            chord->notes[chord->noteCount++] = Note(freq, threshold, color, led, corde, caseFret);
        }
        if (len == 0 && file.available() == 0) break; // Sécurité si </song> absent
    }
    // Si la boucle s'est arrêtée sans voir </song>, on traite la dernière ligne lue
    if (!songEnd && strlen(line) > 0 && strstr(line, "</song>")) {
        Serial.println("Fin de la lecture du fichier XML (fin sans retour ligne)");
    }
    Serial.print("Nombre d'accords lus : ");
    Serial.println(currentSong.chordCount);
    file.close();
}

// --- TEST Gestion des fichiers XML ---

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