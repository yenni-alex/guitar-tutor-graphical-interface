#include "xml.h"

void loadSongFromCSV(const char* filename) {
    File file = SD.open(filename);
    if (!file) {
        Serial.println("Erreur ouverture fichier CSV");
        return;
    }
    Serial.println("Fichier CSV ouvert avec succès");
    file.setTimeout(1000); // Timeout de 1 seconde pour éviter les blocages
    char line[128];
    char* token;
    int chordIndex = -1;

    currentSong.chordCount = 0;
    Serial.println("on va rentrer dans le while...");
    while (file.available()) {
        Serial.println("on lit une ligne...");
        file.readBytesUntil('\n', line, sizeof(line) - 1);
        line[sizeof(line) - 1] = 0;
        Serial.print("line :");
        Serial.println(line);

        token = strtok(line, ",");
        Serial.print("token :");
        Serial.println(token);
        if (!token) continue;
        
        if (strcmp(token, "SONG") == 0) {
            Serial.println("on lit une chanson...");
            token = strtok(nullptr, ",");
            strncpy(currentSong.name, token, sizeof(currentSong.name) - 1);
            currentSong.name[sizeof(currentSong.name) - 1] = 0;

            token = strtok(nullptr, ",");
            currentSong.bpm = atoi(token);

            token = strtok(nullptr, ",");
            currentSong.numChords = atoi(token);
        }

        else if (strcmp(token, "CHORD") == 0 && currentSong.chordCount < Song::MAX_CHORDS) {
            Serial.println("on lit un accord...");
            chordIndex = currentSong.chordCount++;
            Chord& chord = currentSong.chords[chordIndex];

            token = strtok(nullptr, ",");
            chord.time = atoi(token);
            token = strtok(nullptr, ",");
            chord.heightOfHand = atoi(token);
            token = strtok(nullptr, ",");
            chord.noteCount = 0; // notes seront ajoutées ensuite
        }

        else if (strcmp(token, "NOTE") == 0 && chordIndex >= 0) {
            Serial.println("on lit une note...");
            Chord& chord = currentSong.chords[chordIndex];
            if (chord.noteCount >= Chord::MAX_NOTES) continue;

            float freq = atof(strtok(nullptr, ","));
            float threshold = atof(strtok(nullptr, ","));
            int colorInt = atoi(strtok(nullptr, ","));
            int led = atoi(strtok(nullptr, ","));
            int corde = atoi(strtok(nullptr, ","));
            int caseFret = atoi(strtok(nullptr, ","));

            //CRGB color;
            //parseColor(colorInt, color);

            chord.notes[chord.noteCount++] = Note(freq, threshold, colorInt, led, corde, caseFret);
        }
        else if (strcmp(token, "END") == 0) {
            Serial.println("Fin de la chanson");
            break;
        }
        else {
            Serial.print("Ligne ignorée : ");
            Serial.println(line);
            Serial.print("Token : ");
            Serial.println(token);
            Serial.println(strcmp(token, "SONG"));
            Serial.println(strcmp(token, "CHORD"));
            Serial.println(strcmp(token, "NOTE"));
        }
    }
    Serial.println("Lecture du fichier CSV terminée");
    file.close();
    songLoaded = true;

    Serial.print("Chanson chargée : ");
    Serial.println(currentSong.name);
    Serial.print("Nombre d'accords : ");
    Serial.println(currentSong.chordCount);
}


/*
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
    */
   /*
static bool extractAttr(const char* line, const char* key, char* out, size_t outlen) {
    const char* p = strstr(line, key);
    if (!p) return false;
    p += strlen(key);
    while (*p == ' ' || *p == '=') ++p;
    if (*p == '"') ++p;
    
    size_t i = 0;
    while (*p && *p != '"' && *p != ' ' && *p != '>' && i < outlen-1) {
        out[i++] = *p++;
    }
    out[i] = 0;
    return i > 0;
}
    */
/*
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
    while (file.available()) {
        file.readBytesUntil('\n', line, sizeof(line) - 1);
        line[sizeof(line) - 1] = 0;
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
            if (extractAttr(line, "color", colorHex, sizeof(colorHex))) {}
            if (extractAttr(line, "led", buf, sizeof(buf))) led = atoi(buf);
            if (extractAttr(line, "corde", buf, sizeof(buf))) corde = atoi(buf);
            if (extractAttr(line, "caseFret", buf, sizeof(buf))) caseFret = atoi(buf);
            CRGB color;
            parseColor(colorHex, color);
            chord->notes[chord->noteCount++] = Note(freq, threshold, color, led, corde, caseFret);
        }
    }
    Serial.print("Nombre d'accords lus : ");
    Serial.println(currentSong.chordCount);
    file.close();
}
*/

/*
void loadSongFromXML(const char* filename) {
    File file = SD.open(filename);
    if (!file) {
        Serial.println("Erreur ouverture fichier XML");
        return;
    }
    Serial.println("Fichier XML ouvert avec succès");

    currentSong.chordCount = 0;
    char line[128];
    char buf[32];

    // Lire la ligne <song ...>
    file.readBytesUntil('\n', line, sizeof(line) - 1);
    line[sizeof(line) - 1] = 0;

    // Extraire les attributs de la balise <song>
    if (extractAttr(line, "name", buf, sizeof(buf))) {
        strncpy(currentSong.name, buf, sizeof(currentSong.name) - 1);
        currentSong.name[sizeof(currentSong.name) - 1] = 0;
    }
    if (extractAttr(line, "numChords", buf, sizeof(buf))) {
        currentSong.numChords = atoi(buf);
    }
    //return;     // TODO: supprimer cette ligne pour lire les accords              JUSQUE LA CA MARCHE
    // Boucle for jusqu'à numChords
    for (int i = 0; i < currentSong.numChords; ++i) {
        // Lire ligne <chord ...>
        file.readBytesUntil('\n', line, sizeof(line) - 1);
        line[sizeof(line) - 1] = 0;

        if (!strstr(line, "<chord")) {
            Serial.print("Erreur: ligne attendue <chord> manquante à l'index ");
            Serial.println(i);
            break;
        }

        if (currentSong.chordCount < Song::MAX_CHORDS) {
            Chord* chord = &currentSong.chords[currentSong.chordCount++];
            chord->noteCount = 0;
            chord->time = 0;
            chord->heightOfHand = 0;

            int numNotes = 0;

            if (extractAttr(line, "time", buf, sizeof(buf))) chord->time = atoi(buf);
            if (extractAttr(line, "heightOfHand", buf, sizeof(buf))) chord->heightOfHand = atoi(buf);
            if (extractAttr(line, "numNotes", buf, sizeof(buf))) numNotes = atoi(buf);
            //return;                               //CA MARCHE JUSQUE LA
            // Lire les lignes <note ... /> jusqu'à </chord>
            for (int j = 0; j < numNotes; ++j) {
                //break;  // marche
                // return // marche pas
                file.readBytesUntil('\n', line, sizeof(line) - 1);
                line[sizeof(line) - 1] = 0;
                //break;  //     MARCHE
                if (strstr(line, "<note") && chord->noteCount < Chord::MAX_NOTES) {
                    
                    float freq = 0, threshold = 0;
                    int intColor = -1, led = -1, corde = -1, caseFret = -1;
                    // Initialiser proprement le buffer à chaque itération
                    //memset(colorHex, 0, sizeof(colorHex));  // Nettoyer le buffer
                    //strcpy(colorHex, "000000");             // Valeur par défaut
                    //break;  // marche
                    if (extractAttr(line, "freq", buf, sizeof(buf))) freq = atof(buf);
                    //break;  // marche
                    if (extractAttr(line, "threshold", buf, sizeof(buf))) threshold = atof(buf);
                    //break;  // marche
                    if (extractAttr(line, "color", buf, sizeof(buf))) intColor = atoi(buf); // C EST CETTE LIGNE DE MERDE QUI FOU LA MERDE 
                    //break;  // marche PAS
                    if (extractAttr(line, "led", buf, sizeof(buf))) led = atoi(buf);
                    if (extractAttr(line, "corde", buf, sizeof(buf))) corde = atoi(buf);
                    if (extractAttr(line, "caseFret", buf, sizeof(buf))) caseFret = atoi(buf);
                    //break;  //          CA MARCHE

                    //CRGB color;
                    //parseColor(intColor, color);
                    //break;  //          CA MARCHE
                    //chord->notes[chord->noteCount++] = Note(freq, threshold, intColor, led, corde, caseFret);
                    //Note newNote(freq, threshold, intColor, led, corde, caseFret);
                    //break;
                    //chord->notes[chord->noteCount++] = newNote;  // Ajouter la note

                    Note& note = chord->notes[chord->noteCount];
                    // Assignez chaque membre individuellement (adaptez selon votre classe Note)
                    note.freq = freq;
                    note.threshold = threshold;
                    note.colorInt = intColor;  // ou note.colorInt selon votre implémentation
                    note.led = led;
                    note.corde = corde;
                    note.caseFret = caseFret;
                    chord->noteCount++;
                    
                }
            }
            //return;  // TODO: supprimer cette ligne pour lire les accords          MARCHE PAS
            // Lire la ligne de fermeture </chord>
            file.readBytesUntil('\n', line, sizeof(line) - 1);
            line[sizeof(line) - 1] = 0;
            if (!strstr(line, "</chord")) {
                Serial.println("Erreur : balise </chord> manquante");
            }
            //return;  // TODO: supprimer cette ligne pour lire les accords          MARCHE PLUS
        }
    }
    //return;  // TODO: supprimer cette ligne pour lire les accords         LA CA MARCHE PLUS
    Serial.print("Nom de la chanson : ");
    Serial.println(currentSong.name);
    Serial.print("Nombre d'accords annoncés : ");
    Serial.println(currentSong.numChords);
    Serial.print("Nombre d'accords réellement lus : ");
    Serial.println(currentSong.chordCount);
    
    file.close();
}
    */

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
      if (len < MAX_FILENAME_LENGTH && strstr(name, ".txt")) {
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