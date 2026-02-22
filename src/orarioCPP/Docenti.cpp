#include <iostream>
#include <fstream>
#include "Docenti.h"

using namespace std;

// ============================================
// COSTRUTTORE
// ============================================
Docenti::Docenti() {
    // Vector vuoto per default
}

// ============================================
// DISTRUTTORE
// ============================================
Docenti::~Docenti() {
    clear();
}

// ============================================
// GESTIONE CONTENITORE
// ============================================
void Docenti::clear() {
    for (size_t i = 0; i < items.size(); ++i) {
        delete items[i];
    }
    items.clear();
}

size_t Docenti::size() const {
    return items.size();
}

// ============================================
// CARICAMENTO DA FILE XML
// ============================================
bool Docenti::loadFromFile(const string& path) {
    clear();

    XMLDocument doc;
    XMLError err = doc.LoadFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore loadFromFile(Docenti): impossibile caricare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    XMLElement* root = doc.RootElement();
    if (root == nullptr || string(root->Name()) != "Docenti") {
        cerr << "Errore loadFromFile(Docenti): root non valida (attesa <Docenti>)\n";
        return false;
    }

    XMLElement* docenteElem = root->FirstChildElement("Docente");
    while (docenteElem != nullptr) {
        Docente* d = Docente::fromXML(docenteElem);

        if (d == nullptr) {
            cerr << "Warning loadFromFile(Docenti): nodo <Docente> non valido, scartato\n";
            docenteElem = docenteElem->NextSiblingElement("Docente");
        }
        else {
            int id = d->getId();

            if (esisteId(id)) {
                cerr << "Warning loadFromFile(Docenti): ID duplicato (" << id << "), scarto\n";
                delete d;
                docenteElem = docenteElem->NextSiblingElement("Docente");
            }
            else {
                items.push_back(d);
                docenteElem = docenteElem->NextSiblingElement("Docente");
            }
        }
    }

    return true;
}

// ============================================
// SALVATAGGIO SU FILE XML (TinyXML2)
// ============================================
bool Docenti::saveToFile(const string& path) const {
    XMLDocument doc;

    // Dichiarazione XML
    XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);

    // Root
    XMLElement* root = doc.NewElement("Docenti");
    doc.InsertEndChild(root);

    // Contenuto
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            XMLElement* docenteElem = items[i]->toXML(doc);
            root->InsertEndChild(docenteElem);
        }
    }

    XMLError err = doc.SaveFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore saveToFile(Docenti): impossibile salvare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    return true;
}

// ============================================
// SALVATAGGIO SU FILE XML (Stream)
// ============================================
bool Docenti::saveToFileOld(const string& path) const {
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) {
        cerr << "Errore saveToFileOld(Docenti): impossibile aprire '" << path << "'\n";
        return false;
    }

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<Docenti>\n";

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            out << "  " << items[i]->toXML() << "\n";
        }
    }

    out << "</Docenti>\n";
    out.close();

    if (!out.good()) {
        cerr << "Errore saveToFileOld(Docenti): errore in scrittura su '" << path << "'\n";
        return false;
    }

    return true;
}

// ============================================
// CARICAMENTO DA FILE CSV
// ============================================
bool Docenti::loadFromCSV(const string& path) {
    clear();

    ifstream in(path.c_str(), ios::in);
    if (!in.is_open()) {
        cerr << "Errore loadFromCSV(Docenti): impossibile aprire '" << path << "'\n";
        return false;
    }

    string riga;
    while (getline(in, riga)) {
        if (riga.size() == 0) {
            // salta righe vuote
        }
        else {
            Docente* d = new Docente();
            d->fromCSV(riga);

            int id = d->getId();
            if (esisteId(id)) {
                cerr << "Warning loadFromCSV(Docenti): ID duplicato (" << id << "), scarto\n";
                delete d;
            }
            else {
                items.push_back(d);
            }
        }
    }

    in.close();
    return true;
}

// ============================================
// SALVATAGGIO SU FILE CSV
// ============================================
bool Docenti::saveToCSV(const string& path) const {
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) {
        cerr << "Errore saveToCSV(Docenti): impossibile aprire '" << path << "'\n";
        return false;
    }

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            out << items[i]->toCSV() << "\n";
        }
    }

    out.close();

    if (!out.good()) {
        cerr << "Errore saveToCSV(Docenti): errore in scrittura su '" << path << "'\n";
        return false;
    }

    return true;
}

// ============================================
// CRUD - CREATE
// ============================================
bool Docenti::aggiungi(Docente* docente) {
    if (docente == nullptr) {
        cerr << "Errore aggiungi(Docenti): puntatore nullo\n";
        return false;
    }

    if (esisteId(docente->getId())) {
        cerr << "Errore aggiungi(Docenti): ID " << docente->getId() << " gia esistente\n";
        return false;
    }

    items.push_back(docente);
    return true;
}

// ============================================
// CRUD - READ
// ============================================
Docente* Docenti::cercaPerId(int id) const {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            return items[i];
        }
    }
    return nullptr;
}

Docente* Docenti::cercaPerCognome(const string& username) const {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getUsername() == username) {
            return items[i];
        }
    }
    return nullptr;
}

Docente* Docenti::get(size_t index) const {
    if (index < items.size()) {
        return items[index];
    }
    return nullptr;
}

vector<Docente*> Docenti::cercaPerMateriaId(int materiaId) const {
    vector<Docente*> trovati;

    for (size_t i = 0; i < items.size(); ++i) {
        Docente* d = items[i];
        if (d != nullptr) {
            const vector<int>& mids = d->getMaterieIds();

            bool ok = false;
            for (size_t j = 0; j < mids.size() && !ok; ++j) {
                if (mids[j] == materiaId) {
                    ok = true;
                }
            }

            if (ok) {
                trovati.push_back(d);
            }
        }
    }

    return trovati;
}


bool Docenti::esisteId(int id) const {
    bool trovato = false;

    for (size_t i = 0; i < items.size() && !trovato; ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            trovato = true;
        }
    }

    return trovato;
}

// ============================================
// CRUD - UPDATE
// ============================================
bool Docenti::modifica(int id, Docente* nuovoDocente) {
    if (nuovoDocente == nullptr) {
        cerr << "Errore modifica(Docenti): puntatore nullo\n";
        return false;
    }

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            delete items[i];
            items[i] = nuovoDocente;

            // Assicura ID coerente
            nuovoDocente->setId(id);

            return true;
        }
    }

    cerr << "Errore modifica(Docenti): ID " << id << " non trovato\n";
    return false;
}

// ============================================
// CRUD - DELETE
// ============================================
bool Docenti::rimuoviPerId(int id) {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            delete items[i];
            items.erase(items.begin() + i);
            return true;
        }
    }

    cerr << "Errore rimuoviPerId(Docenti): ID " << id << " non trovato\n";
    return false;
}

bool Docenti::rimuoviPerIndex(size_t index) {
    if (index >= items.size()) {
        cerr << "Errore rimuoviPerIndex(Docenti): indice " << index << " fuori range\n";
        return false;
    }

    if (items[index] != nullptr) {
        delete items[index];
    }

    items.erase(items.begin() + index);
    return true;
}

// ============================================
// UTILITY - toXML (stringa)
// ============================================
string Docenti::toXML() const {
    string xml;

    xml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml += "<Docenti>\n";

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            xml += "  " + items[i]->toXML() + "\n";
        }
    }

    xml += "</Docenti>\n";
    return xml;
}

// ============================================
// UTILITY - toString
// ============================================
string Docenti::toString() const {
    string s;

    s += "=== ELENCO DOCENTI ===\n";
    s += "Numero totale: ";
    s += to_string(items.size());
    s += "\n\n";

    if (items.size() == 0) {
        s += "(nessun docente caricato)\n";
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
