#include <iostream>
#include <fstream>
#include <sstream>
#include "Utenti.h"

using namespace std;

// ============================================
// COSTRUTTORE
// ============================================
Utenti::Utenti() {
    // Vector vuoto per default
}

// ============================================
// DISTRUTTORE
// ============================================
Utenti::~Utenti() {
    clear();
}

// ============================================
// GESTIONE CONTENITORE
// ============================================
void Utenti::clear() {
    for (size_t i = 0; i < items.size(); ++i) {
        delete items[i];
    }
    items.clear();
}

size_t Utenti::size() const {
    return items.size();
}

// ============================================
// CARICAMENTO DA FILE XML
// ============================================
bool Utenti::loadFromFile(const string& path) {
    clear();

    XMLDocument doc;
    XMLError err = doc.LoadFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore loadFromFile(Utenti): impossibile caricare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    XMLElement* root = doc.RootElement();
    if (root == nullptr || string(root->Name()) != "Utenti") {
        cerr << "Errore loadFromFile(Utenti): root non valida (attesa <Utenti>)\n";
        return false;
    }

    XMLElement* utenteElem = root->FirstChildElement("Utente");
    while (utenteElem != nullptr) {
        Utente* u = Utente::fromXML(utenteElem);

        if (u == nullptr) {
            cerr << "Warning loadFromFile(Utenti): nodo <Utente> non valido, scartato\n";
            utenteElem = utenteElem->NextSiblingElement("Utente");
        }
        else {
            int id = u->getId();

            if (esisteId(id)) {
                cerr << "Warning loadFromFile(Utenti): ID duplicato (" << id << "), scarto\n";
                delete u;
                utenteElem = utenteElem->NextSiblingElement("Utente");
            }
            else {
                items.push_back(u);
                utenteElem = utenteElem->NextSiblingElement("Utente");
            }
        }
    }

    return true;
}

// ============================================
// SALVATAGGIO SU FILE XML (TinyXML2)
// ============================================
bool Utenti::saveToFile(const string& path) const {
    XMLDocument doc;

    // Dichiarazione XML
    XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);

    // Root
    XMLElement* root = doc.NewElement("Utenti");
    doc.InsertEndChild(root);

    // Contenuto
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            XMLElement* utenteElem = items[i]->toXML(doc);
            root->InsertEndChild(utenteElem);
        }
    }

    XMLError err = doc.SaveFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore saveToFile(Utenti): impossibile salvare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    return true;
}

// ============================================
// CARICAMENTO DA FILE CSV
// ============================================
bool Utenti::loadFromCSV(const string& path) {
    clear();

    ifstream in(path.c_str(), ios::in);
    if (!in.is_open()) {
        cerr << "Errore loadFromCSV(Utenti): impossibile aprire '" << path << "'\n";
        return false;
    }

    string riga;
    while (getline(in, riga)) {
        if (riga.size() == 0) {
            // salta righe vuote
        }
        else {
            Utente* u = new Utente();
            u->fromCSV(riga);

            int id = u->getId();
            if (esisteId(id)) {
                cerr << "Warning loadFromCSV(Utenti): ID duplicato (" << id << "), scarto\n";
                delete u;
            }
            else {
                items.push_back(u);
            }
        }
    }

    in.close();
    return true;
}

// ============================================
// SALVATAGGIO SU FILE CSV
// ============================================
bool Utenti::saveToCSV(const string& path) const {
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) {
        cerr << "Errore saveToCSV(Utenti): impossibile aprire '" << path << "'\n";
        return false;
    }

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            out << items[i]->toCSV() << "\n";
        }
    }

    out.close();

    if (!out.good()) {
        cerr << "Errore saveToCSV(Utenti): errore in scrittura su '" << path << "'\n";
        return false;
    }

    return true;
}

Utente* Utenti::get(size_t index) const {
    if (index < items.size()) {
        return items[index];
    }
    return nullptr;
}

// ============================================
// UTILITY
// ============================================
bool Utenti::esisteId(int id) const {
    bool trovato = false;

    for (size_t i = 0; i < items.size() && !trovato; ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            trovato = true;
        }
    }

    return trovato;
}

// ============================================
// UTILITY - toXML (stringa)
// ============================================
string Utenti::toXML() const {
    string xml;

    xml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml += "<Utenti>\n";

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            xml += "  " + items[i]->toXML() + "\n";
        }
    }

    xml += "</Utenti>\n";
    return xml;
}

// ============================================
// UTILITY - toString
// ============================================
string Utenti::toString() const {
    string s;

    s += "=== ELENCO UTENTI ===\n";
    s += "Numero totale: ";
    s += to_string(items.size());
    s += "\n\n";

    if (items.size() == 0) {
        s += "(nessun utente caricato)\n";
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

