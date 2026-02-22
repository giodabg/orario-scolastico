#include <iostream>
#include <string>

#include "Orario.h"
#include "Classi.h"
#include "Aule.h"
#include "Materie.h"
#include "Docenti.h"
#include "Utenti.h"

using namespace std;

// -----------------------------------------------------------------------------
// Funzioni per testare funzionamento metodi di Orario, con interazione da console
// -----------------------------------------------------------------------------
static void funzioneCaricaOrario(Orario& orario) {
    cout << "\nNome file Orario da caricare (es. orario.xml): ";
    string nomeFile;
    getline(cin, nomeFile);

    if (nomeFile.size() == 0) {
        cout << "Nome file vuoto. Operazione annullata.\n";
        return;
    }

    nomeFile = "resources/" + nomeFile;

    if (orario.loadFromFile(nomeFile)) {
        cout << "Caricamento completato. Lezioni caricate: " << orario.size() << "\n";
    }
    else {
        cout << "Errore nel caricamento del file.\n";
    }
}

static void funzioneSalvaOrario(const Orario& orario) {
    cout << "\nNome file Orario su cui salvare (es. orario_out.xml): ";
    string nomeFile;
    getline(cin, nomeFile);

    if (nomeFile.size() == 0) {
        cout << "Nome file vuoto. Operazione annullata.\n";
        return;
    }

    nomeFile = "resources/" + nomeFile;

    if (orario.saveToFile(nomeFile)) {
        cout << "Salvataggio completato.\n";
    }
    else {
        cout << "Errore nel salvataggio del file.\n";
    }
}

static void funzioneVisualizzaOrario(const Orario& orario) {
    cout << orario.toString();
}

static void funzioneCaricaProposte(Orario& orario) {
    cout << "\nNome file Proposte da caricare (es. proposte.xml): ";
    string nomeFile;
    getline(cin, nomeFile);

    if (nomeFile.size() == 0) {
        cout << "Nome file vuoto. Operazione annullata.\n";
        return;
    }

    nomeFile = "resources/" + nomeFile;

    if (orario.loadProposteFromFile(nomeFile)) {
        cout << "Caricamento proposte completato. Proposte caricate: " << orario.sizeProposte() << "\n";
    }
    else {
        cout << "Errore nel caricamento del file proposte.\n";
    }
}

static void funzioneSalvaProposte(const Orario& orario) {
    cout << "\nNome file Proposte su cui salvare (es. proposte_out.xml): ";
    string nomeFile;
    getline(cin, nomeFile);

    if (nomeFile.size() == 0) {
        cout << "Nome file vuoto. Operazione annullata.\n";
        return;
    }

    nomeFile = "resources/" + nomeFile;

    if (orario.saveProposteToFile(nomeFile)) {
        cout << "Salvataggio proposte completato.\n";
    }
    else {
        cout << "Errore nel salvataggio del file proposte.\n";
    }
}

// Carica dati di supporto e fa resolveAll/resolveProposte (flusso completo)
static void funzioneResolveTutto(Orario& orario) {
    cout << "\nNome file Classi (es. classi.xml): ";
    string fClassi; getline(cin, fClassi);

    cout << "Nome file Aule (es. aule.xml): ";
    string fAule; getline(cin, fAule);

    cout << "Nome file Materie (es. materie.xml): ";
    string fMaterie; getline(cin, fMaterie);

    cout << "Nome file Docenti (es. docenti.xml): ";
    string fDocenti; getline(cin, fDocenti);

    cout << "Nome file Utenti (es. utenti.xml): ";
    string fUtenti; getline(cin, fUtenti);

    if (fClassi.size() == 0 || fAule.size() == 0 || fMaterie.size() == 0 || fDocenti.size() == 0 || fUtenti.size() == 0) {
        cout << "Uno o piu nomi file vuoti. Operazione annullata.\n";
        return;
    }

    fClassi = "resources/" + fClassi;
    fAule = "resources/" + fAule;
    fMaterie = "resources/" + fMaterie;
    fDocenti = "resources/" + fDocenti;
    fUtenti = "resources/" + fUtenti;

    Classi classi;
    Aule aule;
    Materie materie;
    Docenti docenti;
    Utenti utenti;

    bool ok = true;

    if (!classi.loadFromFile(fClassi)) ok = false;
    if (!aule.loadFromFile(fAule)) ok = false;
    if (!materie.loadFromFile(fMaterie)) ok = false;
    if (!docenti.loadFromFile(fDocenti)) ok = false;
    if (!utenti.loadFromFile(fUtenti)) ok = false;

    if (!ok) {
        cout << "Errore: caricamento dati di supporto fallito (classi/aule/materie/docenti/utenti).\n";
        return;
    }

    // Costruisco vettori di puntatori (stile tradizionale, senza iteratori)
    vector<Classe*> tutteClassi;
    for (size_t i = 0; i < classi.size(); ++i) {
        Classe* c = classi.get(i);
        if (c != nullptr) tutteClassi.push_back(c);
    }

    vector<Aula*> tutteAule;
    for (size_t i = 0; i < aule.size(); ++i) {
        Aula* a = aule.get(i);
        if (a != nullptr) tutteAule.push_back(a);
    }

    vector<Materia*> tutteMaterie;
    for (size_t i = 0; i < materie.size(); ++i) {
        Materia* m = materie.get(i);
        if (m != nullptr) tutteMaterie.push_back(m);
    }

    vector<Docente*> tuttiDocenti;
    for (size_t i = 0; i < docenti.size(); ++i) {
        Docente* d = docenti.get(i);
        if (d != nullptr) tuttiDocenti.push_back(d);
    }

    vector<Utente*> tuttiUtenti;
    for (size_t i = 0; i < utenti.size(); ++i) {
        Utente* u = utenti.get(i);
        if (u != nullptr) tuttiUtenti.push_back(u);
    }

    // Resolve lezioni
    orario.resolveAll(tutteClassi, tutteAule, tutteMaterie, tuttiDocenti);
    cout << "resolveAll completato.\n";

    // Resolve proposte
    orario.resolveProposte(tuttiUtenti);
    cout << "resolveProposte completato.\n";
}

// -----------------------------------------------------------------------------
// Menu
// -----------------------------------------------------------------------------
static void stampaMenu() {
    cout << "\n=============================\n";
    cout << "        MENU ORARIO\n";
    cout << "=============================\n";
    cout << "1 - Carica file XML Orario\n";
    cout << "2 - Salva file XML Orario\n";
    cout << "3 - Visualizza Orario\n";
    cout << "4 - Carica file XML Proposte\n";
    cout << "5 - Salva file XML Proposte\n";
    cout << "6 - Resolve completo (lezioni + proposte)\n";
    cout << "0 - Esci\n";
    cout << "Scelta: ";
}

int testOrario() {
    Orario orario;

    while (true) {
        stampaMenu();

        string sceltaStr;
        getline(cin, sceltaStr);

        if (sceltaStr.size() == 0) {
            continue;
        }

        int scelta = atoi(sceltaStr.c_str());

        if (scelta == 0) {
            cout << "Uscita...\n";
            break;
        }
        else if (scelta == 1) {
            funzioneCaricaOrario(orario);
        }
        else if (scelta == 2) {
            funzioneSalvaOrario(orario);
        }
        else if (scelta == 3) {
            funzioneVisualizzaOrario(orario);
        }
        else if (scelta == 4) {
            funzioneCaricaProposte(orario);
        }
        else if (scelta == 5) {
            funzioneSalvaProposte(orario);
        }
        else if (scelta == 6) {
            funzioneResolveTutto(orario);
        }
        else {
            cout << "Scelta non valida.\n";
        }
    }

    return 0;
}