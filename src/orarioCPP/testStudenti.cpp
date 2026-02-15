#include <iostream>
#include <string>
#include <vector>

#include "Studenti.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

// -----------------------------------------------------------------------------
// Funzioni per testare funzionamento metodi di Studenti, con interazione da console
// -----------------------------------------------------------------------------
static void funzioneCarica(Studenti& studenti) {
    cout << "\nNome file da caricare (es. studenti.xml): ";
    string nomeFile;
    getline(cin, nomeFile);

    if (nomeFile.size() == 0) {
        cout << "Nome file vuoto. Operazione annullata.\n";
        return;
    }

    nomeFile = "resources/" + nomeFile;
    if (studenti.loadFromFile(nomeFile)) {
        cout << "Caricamento completato. Studenti caricati: " << studenti.size() << "\n";
    }
    else {
        cout << "Errore nel caricamento del file.\n";
    }
}

static void funzioneSalva(const Studenti& studenti) {
    cout << "\nNome file su cui salvare (es. studenti_out.xml): ";
    string nomeFile;
    getline(cin, nomeFile);

    if (nomeFile.size() == 0) {
        cout << "Nome file vuoto. Operazione annullata.\n";
        return;
    }

    nomeFile = "resources/" + nomeFile;
    if (studenti.saveToFile(nomeFile)) {
        cout << "Salvataggio completato.\n";
    }
    else {
        cout << "Errore nel salvataggio del file.\n";
    }
}

static void funzioneVisualizzaStudenti(const Studenti& studenti) {
    cout << studenti.toString() << "============\n";
}

// -----------------------------------------------------------------------------
// Menu
// -----------------------------------------------------------------------------
static void stampaMenu() {
    cout << "\n=============================\n";
    cout << "        MENU STUDENTI\n";
    cout << "=============================\n";
    cout << "1 - Carica file XML Studenti\n";
    cout << "2 - Salva file XML Studenti\n";
    cout << "3 - Visualizza Studenti\n";
    cout << "0 - Esci\n";
    cout << "Scelta: ";
}

int testStudenti() {
    Studenti studenti;

    while (true) {
        stampaMenu();

        string sceltaStr;
        getline(cin, sceltaStr);

        if (sceltaStr.size() == 0) continue;

        int scelta = -1;
        try {
            // Utilizzo atoi come nell'originale, oppure s.c_str() per compatibilità
            scelta = atoi(sceltaStr.c_str());
        }
        catch (...) {
            scelta = -1;
        }

        if (scelta == 0) {
            cout << "Uscita...\n";
            break;
        }
        else if (scelta == 1) {
            funzioneCarica(studenti);
        }
        else if (scelta == 2) {
            funzioneSalva(studenti);
        }
        else if (scelta == 3) {
            funzioneVisualizzaStudenti(studenti);
        }
        else {
            cout << "Scelta non valida.\n";
        }
    }

    return 0;
}