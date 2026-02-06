#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Aule.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

// -----------------------------------------------------------------------------
// Funzioni richieste
// -----------------------------------------------------------------------------
static void funzioneCarica(Aule& aule) {
    cout << "\nNome file da caricare (es. aule.xml): ";
    string nomeFile;
    getline(cin, nomeFile);

    if (nomeFile.size() == 0) {
        cout << "Nome file vuoto. Operazione annullata.\n";
        return;
    }

	nomeFile = "resources/" + nomeFile;
    if (aule.loadFromFile(nomeFile)) {
        cout << "Caricamento completato. Aule caricate: " << aule.size() << "\n";
    }
    else {
        cout << "Errore nel caricamento del file.\n";
    }
}

static void funzioneSalva(const Aule& aule) {
    cout << "\nNome file su cui salvare (es. aule_out.xml): ";
    string nomeFile;
    getline(cin, nomeFile);

    if (nomeFile.size() == 0) {
        cout << "Nome file vuoto. Operazione annullata.\n";
        return;
    }

    nomeFile = "resources/" + nomeFile;
    if (aule.saveToFile(nomeFile)) {
        cout << "Salvataggio completato.\n";
    }
    else {
        cout << "Errore nel salvataggio del file.\n";
    }
}

static void funzioneVisualizzaAule(const Aule& aule) {
    cout << aule.toString() << "============\n";

}

// -----------------------------------------------------------------------------
// Menu
// -----------------------------------------------------------------------------
static void stampaMenu() {
    cout << "\n=============================\n";
    cout << "        MENU AULE\n";
    cout << "=============================\n";
    cout << "1 - Carica file XML Aule\n";
    cout << "2 - Salva file XML Aule\n";
    cout << "3 - Visualizza Aule\n";
    cout << "0 - Esci\n";
    cout << "Scelta: ";
}

int testAule() {
    Aule aule;

    while (true) {
        stampaMenu();

        string sceltaStr;
        getline(cin, sceltaStr);

        if (sceltaStr.size() == 0) continue;

        int scelta = -1;
        try {
            scelta = atoi(sceltaStr.c_str()); // tradizionale
        }
        catch (...) {
            scelta = -1;
        }

        if (scelta == 0) {
            cout << "Uscita...\n";
            break;
        }
        else if (scelta == 1) {
            funzioneCarica(aule);
        }
        else if (scelta == 2) {
            funzioneSalva(aule);
        }
        else if (scelta == 3) {
            funzioneVisualizzaAule(aule);
        }
        else {
            cout << "Scelta non valida.\n";
        }
    }

    return 0;
}

