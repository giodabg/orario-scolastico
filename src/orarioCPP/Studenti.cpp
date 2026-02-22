#include <iostream>
#include <fstream>
#include <sstream>
#include "Studenti.h"

using namespace std;

// ============================================
// COSTRUTTORE
// ============================================
Studenti::Studenti() {
    // Vector vuoto per default
}

// ============================================
// DISTRUTTORE
// ============================================
Studenti::~Studenti() {
    clear();
}

// ============================================
// GESTIONE CONTENITORE
// ============================================
void Studenti::clear() {
    for (size_t i = 0; i < items.size(); ++i) {
        delete items[i];
    }
    items.clear();
}

size_t Studenti::size() const {
    return items.size();
}

// ============================================
// CARICAMENTO DA FILE XML
// ============================================
bool Studenti::loadFromFile(const string& path) {
    clear();

    XMLDocument doc;
    XMLError err = doc.LoadFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore loadFromFile(Studenti): impossibile caricare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    XMLElement* root = doc.RootElement();
    if (root == nullptr || string(root->Name()) != "Studenti") {
        cerr << "Errore loadFromFile(Studenti): root non valida (attesa <Studenti>)\n";
        return false;
    }

    XMLElement* studenteElem = root->FirstChildElement("Studente");
    while (studenteElem != nullptr) {
        Studente* s = Studente::fromXML(studenteElem);
        if (s == nullptr) {
            cerr << "Warning loadFromFile(Studenti): nodo <Studente> non valido, scartato\n";
            studenteElem = studenteElem->NextSiblingElement("Studente");
        }
        else {
            int id = s->getId();

            if (esisteId(id)) {
                cerr << "Warning loadFromFile(Studenti): ID duplicato (" << id << "), scarto\n";
                delete s;
                studenteElem = studenteElem->NextSiblingElement("Studente");
            }
            else {
                items.push_back(s);
                studenteElem = studenteElem->NextSiblingElement("Studente");
            }
        }
    }

    return true;
}

// ============================================
// SALVATAGGIO SU FILE XML (TinyXML2)
// ============================================
bool Studenti::saveToFile(const string& path) const {
    XMLDocument doc;

    // Dichiarazione XML
    XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);

    // Root
    XMLElement* root = doc.NewElement("Studenti");
    doc.InsertEndChild(root);

    // Contenuto
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            XMLElement* studenteElem = items[i]->toXML(doc);
            root->InsertEndChild(studenteElem);
        }
    }

    XMLError err = doc.SaveFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore saveToFile(Studenti): impossibile salvare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    return true;
}

// ============================================
// SALVATAGGIO SU FILE XML (Stream)
// ============================================
bool Studenti::saveToFileOld(const string& path) const {
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) {
        cerr << "Errore saveToFileOld(Studenti): impossibile aprire '" << path << "'\n";
        return false;
    }

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<Studenti>\n";

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            out << "  " << items[i]->toXML() << "\n";
        }
    }

    out << "</Studenti>\n";
    out.close();

    if (!out.good()) {
        cerr << "Errore saveToFileOld(Studenti): errore in scrittura su '" << path << "'\n";
        return false;
    }

    return true;
}

// ============================================
// CARICAMENTO DA FILE CSV
// ============================================
bool Studenti::loadFromCSV(const string& path) {
    clear();

    ifstream in(path.c_str(), ios::in);
    if (!in.is_open()) {
        cerr << "Errore loadFromCSV(Studenti): impossibile aprire '" << path << "'\n";
        return false;
    }

    string riga;
    while (getline(in, riga)) {
        if (riga.size() == 0) {
            // salta righe vuote
        }
        else {
            Studente* s = new Studente();
            s->fromCSV(riga);

            int id = s->getId();
            if (esisteId(id)) {
                cerr << "Warning loadFromCSV(Studenti): ID duplicato (" << id << "), scarto\n";
                delete s;
            }
            else {
                items.push_back(s);
            }
        }
    }

    in.close();
    return true;
}

// ============================================
// SALVATAGGIO SU FILE CSV
// ============================================
bool Studenti::saveToCSV(const string& path) const {
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) {
        cerr << "Errore saveToCSV(Studenti): impossibile aprire '" << path << "'\n";
        return false;
    }

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            out << items[i]->toCSV() << "\n";
        }
    }

    out.close();

    if (!out.good()) {
        cerr << "Errore saveToCSV(Studenti): errore in scrittura su '" << path << "'\n";
        return false;
    }

    return true;
}

// ============================================
// CRUD - CREATE
// ============================================
bool Studenti::aggiungi(Studente* studente) {
    if (studente == nullptr) {
        cerr << "Errore aggiungi(Studenti): puntatore nullo\n";
        return false;
    }

    if (esisteId(studente->getId())) {
        cerr << "Errore aggiungi(Studenti): ID " << studente->getId() << " gia esistente\n";
        return false;
    }

    items.push_back(studente);
    return true;
}

// ============================================
// CRUD - READ
// ============================================
Studente* Studenti::cercaPerId(int id) const {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            return items[i];
        }
    }
    return nullptr;
}

Studente* Studenti::cercaPerUsername(const string& username) const {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getUsername() == username) {
            return items[i];
        }
    }
    return nullptr;
}

Studente* Studenti::get(size_t index) const {
    if (index < items.size()) {
        return items[index];
    }
    return nullptr;
}

// ============================================
// CRUD - UPDATE
// ============================================
bool Studenti::modifica(int id, Studente* nuovoStudente) {
    if (nuovoStudente == nullptr) {
        cerr << "Errore modifica(Studenti): puntatore nullo\n";
        return false;
    }

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            delete items[i];
            items[i] = nuovoStudente;

            // Assicura ID coerente
            nuovoStudente->setId(id);

            return true;
        }
    }

    cerr << "Errore modifica(Studenti): ID " << id << " non trovato\n";
    return false;
}

// ============================================
// CRUD - DELETE
// ============================================
bool Studenti::rimuoviPerId(int id) {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            delete items[i];
            items.erase(items.begin() + i);
            return true;
        }
    }

    cerr << "Errore rimuoviPerId(Studenti): ID " << id << " non trovato\n";
    return false;
}

bool Studenti::rimuoviPerIndex(size_t index) {
    if (index >= items.size()) {
        cerr << "Errore rimuoviPerIndex(Studenti): indice " << index << " fuori range\n";
        return false;
    }

    if (items[index] != nullptr) {
        delete items[index];
    }

    items.erase(items.begin() + index);
    return true;
}

// ============================================
// UTILITY
// ============================================
bool Studenti::esisteId(int id) const {
    bool trovato = false;
    for (size_t j = 0; j < items.size() && !trovato; ++j) {
        if (items[j] != nullptr && items[j]->getId() == id) {
            trovato = true;
        }
    }
    return trovato;
}

// ============================================
// UTILITY - toXML (stringa)
// ============================================
string Studenti::toXML() const {
    string xml;

    xml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml += "<Studenti>\n";

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            xml += "  " + items[i]->toXML() + "\n";
        }
    }

    xml += "</Studenti>\n";
    return xml;
}

// ============================================
// UTILITY - toString
// ============================================
string Studenti::toString() const {
    string s;

    s += "=== ELENCO STUDENTI ===\n";
    s += "Numero totale: ";
    s += to_string(items.size());
    s += "\n\n";

    if (items.size() == 0) {
        s += "(nessuno studente caricato)\n";
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
