#include <iostream>
#include <fstream>
#include "Aule.h"

using namespace std;

// ============================================
// COSTRUTTORE
// ============================================
Aule::Aule() {
    // Vector vuoto per default
}

// ============================================
// DISTRUTTORE
// ============================================
Aule::~Aule() {
    clear();
}

// ============================================
// GESTIONE CONTENITORE
// ============================================
void Aule::clear() {
    for (size_t i = 0; i < items.size(); ++i) {
        delete items[i];
    }
    items.clear();
}

size_t Aule::size() const {
    return items.size();
}

// ============================================
// CARICAMENTO DA FILE XML
// ============================================
bool Aule::loadFromFile(const string& path) {
    clear();

    XMLDocument doc;
    XMLError err = doc.LoadFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore loadFromFile(Aule): impossibile caricare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    XMLElement* root = doc.RootElement();
    if (root == nullptr || string(root->Name()) != "Aule") {
        cerr << "Errore loadFromFile(Aule): root non valida (attesa <Aule>)\n";
        return false;
    }

    XMLElement* aulaElem = root->FirstChildElement("Aula");
    while (aulaElem != nullptr) {
        Aula* a = Aula::fromXML(aulaElem);
        if (a == nullptr) {
            cerr << "Warning loadFromFile(Aule): nodo <Aula> non valido, scartato\n";
            aulaElem = aulaElem->NextSiblingElement("Aula");
        }
        else {
            int id = a->getId();

            if (esisteId(id)) {
                cerr << "Warning loadFromFile(Aule): ID duplicato (" << id << "), scarto\n";
                delete a;
                aulaElem = aulaElem->NextSiblingElement("Aula");
            }
            else {
                items.push_back(a);
                aulaElem = aulaElem->NextSiblingElement("Aula");
            }
        }
    }

    return true;
}

// ============================================
// SALVATAGGIO SU FILE XML (TinyXML2)
// ============================================
bool Aule::saveToFile(const string& path) const {
    XMLDocument doc;

    // Dichiarazione XML
    XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);

    // Root
    XMLElement* root = doc.NewElement("Aule");
    doc.InsertEndChild(root);

    // Contenuto
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            XMLElement* aulaElem = items[i]->toXML(doc);
            root->InsertEndChild(aulaElem);
        }
    }

    XMLError err = doc.SaveFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore saveToFile(Aule): impossibile salvare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    return true;
}

// ============================================
// SALVATAGGIO SU FILE XML (Stream)
// ============================================
bool Aule::saveToFileOld(const string& path) const {
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) {
        cerr << "Errore saveToFileOld(Aule): impossibile aprire '" << path << "'\n";
        return false;
    }

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<Aule>\n";

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            out << "  " << items[i]->toXML() << "\n";
        }
    }

    out << "</Aule>\n";
    out.close();

    if (!out.good()) {
        cerr << "Errore saveToFileOld(Aule): errore in scrittura su '" << path << "'\n";
        return false;
    }

    return true;
}

// ============================================
// CRUD - CREATE
// ============================================
bool Aule::aggiungi(Aula* aula) {
    if (aula == nullptr) {
        cerr << "Errore aggiungi(Aule): puntatore nullo\n";
        return false;
    }

    // Controlla se ID già esistente
    if (esisteId(aula->getId())) {
        cerr << "Errore aggiungi(Aule): ID " << aula->getId() << " già esistente\n";
        return false;
    }

    items.push_back(aula);
    return true;
}

// ============================================
// CRUD - READ
// ============================================
Aula* Aule::cercaPerId(int id) const {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            return items[i];
        }
    }
    return nullptr;
}

Aula* Aule::cercaPerNome(const string& nome) const {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getNome() == nome) {
            return items[i];
        }
    }
    return nullptr;
}

Aula* Aule::get(size_t index) const {
    if (index < items.size()) {
        return items[index];
    }
    return nullptr;
}

bool Aule::esisteId(int id) const {
    bool trovato = false;
    for (size_t j = 0; j < items.size() && !trovato; ++j) {
        if (items[j] != nullptr && items[j]->getId() == id) {
            trovato = true;
        }
    }
    return trovato;
}

vector<Aula*> Aule::cercaPerTipo(TipoAula tipo) const {
    vector<Aula*> risultato;

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getTipo() == tipo) {
            risultato.push_back(items[i]);
        }
    }

    return risultato;
}

vector<Aula*> Aule::cercaPerPostiMinimi(int postiMin) const {
    vector<Aula*> risultato;

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getPosti() >= postiMin) {
            risultato.push_back(items[i]);
        }
    }

    return risultato;
}

// ============================================
// CRUD - UPDATE
// ============================================
bool Aule::modifica(int id, Aula* nuovaAula) {
    if (nuovaAula == nullptr) {
        cerr << "Errore modifica(Aule): puntatore nullo\n";
        return false;
    }

    // Cerca l'aula con questo ID
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            // Elimina la vecchia aula
            delete items[i];

            // Sostituisci con la nuova
            items[i] = nuovaAula;

            // Assicurati che l'ID sia corretto
            nuovaAula->setId(id);

            return true;
        }
    }

    cerr << "Errore modifica(Aule): ID " << id << " non trovato\n";
    return false;
}

// ============================================
// CRUD - DELETE
// ============================================
bool Aule::rimuoviPerId(int id) {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            // Dealloca memoria
            delete items[i];

            // Rimuovi dal vector
            items.erase(items.begin() + i);

            return true;
        }
    }

    cerr << "Errore rimuoviPerId(Aule): ID " << id << " non trovato\n";
    return false;
}

bool Aule::rimuoviPerIndex(size_t index) {
    if (index >= items.size()) {
        cerr << "Errore rimuoviPerIndex(Aule): indice " << index << " fuori range\n";
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
string Aule::toXML() const {
    string xml;

    xml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml += "<Aule>\n";

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            xml += "  " + items[i]->toXML() + "\n";
        }
    }

    xml += "</Aule>\n";

    return xml;
}

// ============================================
// UTILITY - toString
// ============================================
string Aule::toString() const {
    string s;

    s += "=== ELENCO AULE ===\n";
    s += "Numero totale: ";
    s += to_string(items.size());
    s += "\n\n";

    if (items.size() == 0) {
        s += "(nessuna aula caricata)\n";
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