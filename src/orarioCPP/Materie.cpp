#include <iostream>
#include <fstream>
#include <sstream>
#include "Materie.h"

using namespace std;

// ============================================
// COSTRUTTORE
// ============================================
Materie::Materie() {
    // Vector vuoto per default
}

// ============================================
// DISTRUTTORE
// ============================================
Materie::~Materie() {
    clear();
}

// ============================================
// GESTIONE CONTENITORE
// ============================================
void Materie::clear() {
    for (size_t i = 0; i < items.size(); ++i) {
        delete items[i];
    }
    items.clear();
}

size_t Materie::size() const {
    return items.size();
}

// ============================================
// CARICAMENTO DA FILE XML
// ============================================
bool Materie::loadFromFile(const string& path) {
    clear();

    XMLDocument doc;
    XMLError err = doc.LoadFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore loadFromFile(Materie): impossibile caricare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    XMLElement* root = doc.RootElement();
    if (root == nullptr || string(root->Name()) != "Materie") {
        cerr << "Errore loadFromFile(Materie): root non valida (attesa <Materie>)\n";
        return false;
    }

    XMLElement* materiaElem = root->FirstChildElement("Materia");
    while (materiaElem != nullptr) {
        Materia* m = Materia::fromXML(materiaElem);

        if (m == nullptr) {
            cerr << "Warning loadFromFile(Materie): nodo <Materia> non valido, scartato\n";
            materiaElem = materiaElem->NextSiblingElement("Materia");
        }
        else {
            int id = m->getId();

            if (esisteId(id)) {
                cerr << "Warning loadFromFile(Materie): ID duplicato (" << id << "), scarto\n";
                delete m;
                materiaElem = materiaElem->NextSiblingElement("Materia");
            }
            else {
                items.push_back(m);
                materiaElem = materiaElem->NextSiblingElement("Materia");
            }
        }
    }

    return true;
}

// ============================================
// SALVATAGGIO SU FILE XML (TinyXML2)
// ============================================
bool Materie::saveToFile(const string& path) const {
    XMLDocument doc;

    // Dichiarazione XML
    XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);

    // Root
    XMLElement* root = doc.NewElement("Materie");
    doc.InsertEndChild(root);

    // Contenuto
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            XMLElement* materiaElem = items[i]->toXML(doc);
            root->InsertEndChild(materiaElem);
        }
    }

    XMLError err = doc.SaveFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore saveToFile(Materie): impossibile salvare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    return true;
}

// ============================================
// SALVATAGGIO SU FILE XML (Stream)
// ============================================
bool Materie::saveToFileOld(const string& path) const {
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) {
        cerr << "Errore saveToFileOld(Materie): impossibile aprire '" << path << "'\n";
        return false;
    }

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<Materie>\n";

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            out << "  " << items[i]->toXML() << "\n";
        }
    }

    out << "</Materie>\n";
    out.close();

    if (!out.good()) {
        cerr << "Errore saveToFileOld(Materie): errore in scrittura su '" << path << "'\n";
        return false;
    }

    return true;
}

// ============================================
// CARICAMENTO DA FILE CSV
// ============================================
bool Materie::loadFromCSV(const string& path) {
    clear();

    ifstream in(path.c_str(), ios::in);
    if (!in.is_open()) {
        cerr << "Errore loadFromCSV(Materie): impossibile aprire '" << path << "'\n";
        return false;
    }

    string riga;
    while (getline(in, riga)) {
        if (riga.size() == 0) {
            // salta righe vuote
        }
        else {
            Materia* m = new Materia();
            m->fromCSV(riga);

            int id = m->getId();
            if (esisteId(id)) {
                cerr << "Warning loadFromCSV(Materie): ID duplicato (" << id << "), scarto\n";
                delete m;
            }
            else {
                items.push_back(m);
            }
        }
    }

    in.close();
    return true;
}

// ============================================
// SALVATAGGIO SU FILE CSV
// ============================================
bool Materie::saveToCSV(const string& path) const {
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) {
        cerr << "Errore saveToCSV(Materie): impossibile aprire '" << path << "'\n";
        return false;
    }

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            out << items[i]->toCSV() << "\n";
        }
    }

    out.close();

    if (!out.good()) {
        cerr << "Errore saveToCSV(Materie): errore in scrittura su '" << path << "'\n";
        return false;
    }

    return true;
}

// ============================================
// CRUD - CREATE
// ============================================
bool Materie::aggiungi(Materia* materia) {
    if (materia == nullptr) {
        cerr << "Errore aggiungi(Materie): puntatore nullo\n";
        return false;
    }

    if (esisteId(materia->getId())) {
        cerr << "Errore aggiungi(Materie): ID " << materia->getId() << " gia esistente\n";
        return false;
    }

    items.push_back(materia);
    return true;
}

// ============================================
// CRUD - READ
// ============================================
Materia* Materie::cercaPerId(int id) const {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            return items[i];
        }
    }
    return nullptr;
}

Materia* Materie::cercaPerNome(const string& nome) const {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getNome() == nome) {
            return items[i];
        }
    }
    return nullptr;
}

Materia* Materie::get(size_t index) const {
    if (index < items.size()) {
        return items[index];
    }
    return nullptr;
}

bool Materie::esisteId(int id) const {
    bool trovato = false;

    for (size_t i = 0; i < items.size() && !trovato; ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            trovato = true;
        }
    }

    return trovato;
}

vector<Materia*> Materie::cercaLaboratori() const {
    vector<Materia*> labs;

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getLaboratorio()) {
            labs.push_back(items[i]);
        }
    }

    return labs;
}

// ============================================
// CRUD - UPDATE
// ============================================
bool Materie::modifica(int id, Materia* nuovaMateria) {
    if (nuovaMateria == nullptr) {
        cerr << "Errore modifica(Materie): puntatore nullo\n";
        return false;
    }

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            delete items[i];
            items[i] = nuovaMateria;

            // Assicura ID coerente
            nuovaMateria->setId(id);

            return true;
        }
    }

    cerr << "Errore modifica(Materie): ID " << id << " non trovato\n";
    return false;
}

// ============================================
// CRUD - DELETE
// ============================================
bool Materie::rimuoviPerId(int id) {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            delete items[i];
            items.erase(items.begin() + i);
            return true;
        }
    }

    cerr << "Errore rimuoviPerId(Materie): ID " << id << " non trovato\n";
    return false;
}

bool Materie::rimuoviPerIndex(size_t index) {
    if (index >= items.size()) {
        cerr << "Errore rimuoviPerIndex(Materie): indice " << index << " fuori range\n";
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
string Materie::toXML() const {
    string xml;

    xml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml += "<Materie>\n";

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            xml += "  " + items[i]->toXML() + "\n";
        }
    }

    xml += "</Materie>\n";
    return xml;
}

// ============================================
// UTILITY - toString
// ============================================
string Materie::toString() const {
    string s;

    s += "=== ELENCO MATERIE ===\n";
    s += "Numero totale: ";
    s += to_string(items.size());
    s += "\n\n";

    if (items.size() == 0) {
        s += "(nessuna materia caricata)\n";
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
