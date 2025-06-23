#include "xml.h"

bool loadSongFromJson(const char* filename) {
    File file = SD.open(filename);
    if (!file) {
        Serial.println("Erreur : fichier introuvable");
        return false;
    }

    StaticJsonDocument<8192> doc; // 8 Ko → adapte si plus petit fichier

    DeserializationError error = deserializeJson(doc, file);
    file.close();
    if (error) {
        Serial.print("Erreur JSON: ");
        Serial.println(error.c_str());
        return false;
    }

    // Nom de la chanson
    const char* songName = doc["name"] | "Unnamed";
    strncpy(currentSong.name, songName, sizeof(currentSong.name) - 1);
    currentSong.name[sizeof(currentSong.name) - 1] = '\0';

    // BPM
    currentSong.bpm = doc["bpm"] | 120;

    // Chargement des accords
    JsonArray chordArray = doc["chords"];
    currentSong.chordCount = 0;

    for (JsonObject c : chordArray) {
        if (currentSong.chordCount >= Song::MAX_CHORDS) break;

        Chord& chord = currentSong.chords[currentSong.chordCount++];
        chord.time = c["time"] | 0;
        chord.heightOfHand = c["height"] | 0;

        JsonArray noteArray = c["notes"];
        chord.noteCount = 0;
        for (JsonObject n : noteArray) {
            if (chord.noteCount >= Chord::MAX_NOTES) break;
    
            //float freq = n["freq"] | 0;
            float freq = n["freq"] | 0.0f; // Utilise 0.0f pour les fréquences
            float threshold = n["threshold"] | 0.05;
            int color = n["color"] | 0;
            int led = n["led"] | -1;
            int corde = n["corde"] | -1;
            int caseFret = n["case"] | -1;

            chord.notes[chord.noteCount++] = Note(freq, threshold, color, led, corde, caseFret);
        }
    }

    currentSong.numChords = currentSong.chordCount;

    Serial.print("Chargé : ");
    Serial.print(currentSong.chordCount);
    Serial.println(" accords");

    return true;
}
/*
Note loadNoteFromCSV(const char* line) {  // ca ca marche
    // Copie de la ligne pour la modifier avec strtok
    char buffer[128];
    strncpy(buffer, line, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0'; // Sécurité terminaison

    const char* delimiter = ",";
    char* token = strtok(buffer, delimiter);

    if (token == nullptr || strcmp(token, "NOTE") != 0) {
        // Ligne invalide
        return Note();
    }

    float freq = 0.0f;
    float threshold = 0.0f;
    int colorInt = 0;
    int led = -1;
    int corde = -1;
    int caseFret = -1;

    token = strtok(nullptr, delimiter);
    if (token) freq = atof(token);

    token = strtok(nullptr, delimiter);
    if (token) threshold = atof(token);

    token = strtok(nullptr, delimiter);
    if (token) colorInt = atoi(token);

    token = strtok(nullptr, delimiter);
    if (token) led = atoi(token);

    token = strtok(nullptr, delimiter);
    if (token) corde = atoi(token);

    token = strtok(nullptr, delimiter);
    if (token) caseFret = atoi(token);

    return Note(freq, threshold, colorInt, led, corde, caseFret);
}

Chord loadChordFromCSVLines(const char* lines[], int lineCount) {
    Chord chord;

    if (lineCount == 0) return chord;

    // Parse la première ligne (CHORD)
    char buffer[128];
    strncpy(buffer, lines[0], sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    const char* delimiter = ",";
    char* token = strtok(buffer, delimiter);

    if (token == nullptr || strcmp(token, "CHORD") != 0) {
        // Ligne invalide
        return chord;
    }

    int time = 0;
    int height = 0;
    int numNotes = 0;

    token = strtok(nullptr, delimiter);
    if (token) time = atoi(token);

    token = strtok(nullptr, delimiter);
    if (token) height = atoi(token);

    token = strtok(nullptr, delimiter);
    if (token) numNotes = atoi(token);

    chord.time = time;
    chord.heightOfHand = height;
    chord.noteCount = 0;

    // Charger les lignes NOTE suivantes
    for (int i = 1; i <= numNotes && i < lineCount && chord.noteCount < Chord::MAX_NOTES; ++i) {
        const char* line = lines[i];

        // Appel à la fonction précédente
        Note n = loadNoteFromCSV(line);
        chord.notes[chord.noteCount++] = n;
    }

    return chord;
}
*/




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
      if (len < MAX_FILENAME_LENGTH && strstr(name, ".json")) {
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