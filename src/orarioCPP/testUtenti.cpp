#include <iostream>
#include <string>
#include <vector>

#include "Utenti.h"

using namespace std;

// -----------------------------------------------------------------------------
// Funzioni per testare funzionamento metodi di Utenti, con interazione da console
// -----------------------------------------------------------------------------
static void funzioneCaricaXML(Utenti& utenti) {
    cout << "\nNome file XML da caricare (es. utenti.xml): ";
    string nomeFile;
    getline(cin, nomeFile);

    if (nomeFile.size() == 0) {
        cout << "Nome file vuoto. Operazione annullata.\n";
        return;
    }

    nomeFile = "resources/" + nomeFile;
    if (utenti.loadFromFile(nomeFile)) {
        cout << "Caricamento XML completato. Utenti in memoria: " << utenti.size() << "\n";
    }
    else {
        cout << "Errore nel caricamento del file XML.\n";
    }
}

static void funzioneSalvaXML(const Utenti& utenti) {
    cout << "\nNome file XML su cui salvare (es. utenti_out.xml): ";
    string nomeFile;
    getline(cin, nomeFile);

    if (nomeFile.size() == 0) {
        cout << "Nome file vuoto. Operazione annullata.\n";
        return;
    }

    nomeFile = "resources/" + nomeFile;
    if (utenti.saveToFile(nomeFile)) {
        cout << "Salvataggio XML completato.\n";
    }
    else {
        cout << "Errore nel salvataggio del file XML.\n";
    }
}

static void funzioneCaricaCSV(Utenti& utenti) {
    cout << "\nNome file CSV da caricare (es. utenti.csv): ";
    string nomeFile;
    getline(cin, nomeFile);

    nomeFile = "resources/" + nomeFile;
    if (utenti.loadFromCSV(nomeFile)) {
        cout << "Caricamento CSV completato. Utenti in memoria: " << utenti.size() << "\n";
    }
    else {
        cout << "Errore nel caricamento del file CSV.\n";
    }
}

static void funzioneSalvaCSV(const Utenti& utenti) {
    cout << "\nNome file CSV su cui salvare (es. utenti_out.csv): ";
    string nomeFile;
    getline(cin, nomeFile);

    nomeFile = "resources/" + nomeFile;
    if (utenti.saveToCSV(nomeFile)) {
        cout << "Salvataggio CSV completato.\n";
    }
    else {
        cout << "Errore nel salvataggio del file CSV.\n";
    }
}

static void funzioneVisualizzaUtenti(const Utenti& utenti) {
    cout << utenti.toString() << "============\n";
}

// -----------------------------------------------------------------------------
// Menu
// -----------------------------------------------------------------------------
static void stampaMenu() {
    cout << "\n=============================\n";
    cout << "        MENU UTENTI\n";
    cout << "=============================\n";
    cout << "1 - Carica file XML Utenti\n";
    cout << "2 - Salva file XML Utenti\n";
    cout << "3 - Carica file CSV Utenti\n";
    cout << "4 - Salva file CSV Utenti\n";
    cout << "5 - Visualizza Utenti\n";
    cout << "0 - Esci\n";
    cout << "Scelta: ";
}

int testUtenti() {
    Utenti utenti;

    while (true) {
        stampaMenu();

        string sceltaStr;
        getline(cin, sceltaStr);

        if (sceltaStr.size() == 0) continue;

        int scelta = -1;
        try {
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
            funzioneCaricaXML(utenti);
        }
        else if (scelta == 2) {
            funzioneSalvaXML(utenti);
        }
        else if (scelta == 3) {
            funzioneCaricaCSV(utenti);
        }
        else if (scelta == 4) {
            funzioneSalvaCSV(utenti);
        }
        else if (scelta == 5) {
            funzioneVisualizzaUtenti(utenti);
        }
        else {
            cout << "Scelta non valida.\n";
        }
    }

    return 0;
}