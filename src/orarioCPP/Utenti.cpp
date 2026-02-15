#include "Utenti.h"
#include <iostream>
#include <fstream>
#include "Utente.h"

using namespace std;

Utenti::~Utenti() {
    clear();
}

void Utenti::clear() {
    for (size_t i = 0; i < items.size(); ++i) {
        delete items[i];
    }
    items.clear();
}

size_t Utenti::size() const {
    return items.size();
}

bool Utenti::esisteId(int id) const {
    for (size_t j = 0; j < items.size(); ++j) {
        if (items[j]->getId() == id) {
            return true;
        }
    }
    return false;
}

bool Utenti::loadFromFile(const string& path) {
    clear();
    ifstream in(path.c_str());
    if (!in.is_open()) {
        cerr << "Errore loadFromFile(Utenti): impossibile aprire '" << path << "'\n";
        return false;
    }

    string line, fullXml;
    bool insideUtente = false;
    string utenteBuffer;

    while (getline(in, line)) {
        if (line.find("<Utente") != string::npos) {
            insideUtente = true;
            utenteBuffer = line + "\n";
        }
        else if (line.find("</Utente>") != string::npos) {
            utenteBuffer += line + "\n";

            Utente* u = new Utente();
            u->fromXML(utenteBuffer); // Uso il tuo parser manuale

            if (esisteId(u->getId())) {
                cerr << "Warning: ID duplicato (" << u->getId() << "), scarto utente.\n";
                delete u;
            }
            else {
                items.push_back(u);
            }
            insideUtente = false;
            utenteBuffer = "";
        }
        else if (insideUtente) {
            utenteBuffer += line + "\n";
        }
    }
    in.close();
    return true;
}

bool Utenti::saveToFile(const string& path) const {
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) {
        cerr << "Errore saveToFile(Utenti): impossibile aprire '" << path << "'\n";
        return false;
    }

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<Utenti>\n";

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            out << items[i]->toXML() << "\n";
        }
    }

    out << "</Utenti>\n";
    out.close();
    return out.good();
}

bool Utenti::saveToCSV(const string& path) const {
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) return false;

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            out << items[i]->toCSV() << "\n";
        }
    }
    out.close();
    return true;
}


bool Utenti::loadFromCSV(const string& path) {
    clear();
    ifstream in(path.c_str());
    if (!in.is_open()) return false;

    string riga;
    while (getline(in, riga)) {
        if (riga.empty()) continue;
        Utente* u = new Utente();
        u->fromCSV(riga);

        if (esisteId(u->getId())) {
            delete u;
        }
        else {
            items.push_back(u);
        }
    }
    in.close();
    return true;
}


string Utenti::toString() const {
    string s;
    s += "=== ELENCO UTENTI ===\n";
    s += "Numero totale: " + to_string(items.size()) + "\n\n";

    if (items.empty()) {
        s += "(nessun utente in elenco)\n";
        return s;
    }

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            s += items[i]->toString(); 
            s += "--------------------\n";
        }
    }
    return s;
}