#include <iostream>
#include <fstream>

#include "Aule.h"

using namespace std;

Aule::~Aule() {
    clear();
}

void Aule::clear() {
    // dealloca tutte le aule possedute dal contenitore
    for (size_t i = 0; i < items.size(); ++i) {
        delete items[i];
    }
    items.clear();
    byId.clear();
}

size_t Aule::size() const {
    return items.size();
}

bool Aule::validateUniqueIds() const {
    // Controllo semplice: se byId ha la stessa cardinalità di items non ci sono duplicati.
    // (Vale se byId è popolata coerentemente con items)
    if (byId.size() != items.size()) {
        return false;
    }

    // Controllo più “esplicito” e tradizionale
    map<int, int> occorrenze;
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] == 0) return false;
        int id = items[i]->getId();
        if (occorrenze.find(id) == occorrenze.end()) {
            occorrenze[id] = 1;
        }
        else {
            occorrenze[id] = occorrenze[id] + 1;
        }
    }

    for (map<int, int>::const_iterator it = occorrenze.begin(); it != occorrenze.end(); ++it) {
        if (it->second != 1) return false;
    }

    return true;
}

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
    if (root == 0 || string(root->Name()) != "Aule") {
        cerr << "Errore loadFromFile(Aule): root non valida (attesa <Aule>)\n";
        return false;
    }

    XMLElement* aulaElem = root->FirstChildElement("Aula");
    while (aulaElem != 0) {
        Aula* a = Aula::fromXML(aulaElem);
        if (a == 0) {
            cerr << "Warning loadFromFile(Aule): nodo <Aula> non valido, scartato\n";
            aulaElem = aulaElem->NextSiblingElement("Aula");
            continue;
        }

        int id = a->getId();

        // gestisci duplicati
        if (byId.find(id) != byId.end()) {
            cerr << "Warning loadFromFile(Aule): ID duplicato (" << id << "), scarto\n";
            delete a;
            aulaElem = aulaElem->NextSiblingElement("Aula");
            continue;
        }

        byId[id] = a;
        items.push_back(a);

        aulaElem = aulaElem->NextSiblingElement("Aula");
    }

    // opzionale: se vuoi essere “rigido”, qui puoi fallire se non è valido
    if (!validateUniqueIds()) {
        cerr << "Errore loadFromFile(Aule): rilevati ID duplicati o dati incoerenti\n";
        clear();
        return false;
    }

    return true;
}


// ------------------------------------------------------
// Aule::saveToFile (TinyXML2)
// Scrive:
//
// <?xml version="1.0" encoding="UTF-8"?>
// <Aule>
//   <Aula ...> ... </Aula>
// </Aule>
// ------------------------------------------------------
bool Aule::saveToFile(const string& path) const {
    // opzionale: controllo consistenza prima di salvare
    if (!validateUniqueIds()) {
        cerr << "Errore saveToFile(Aule): ID duplicati o indice incoerente\n";
        return false;
    }

    XMLDocument doc;

    // Dichiarazione XML
    XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);

    // Root
    XMLElement* root = doc.NewElement("Aule");
    doc.InsertEndChild(root);

    // Contenuto
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != 0) {
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

bool Aule::saveToFileOld(const string& path) const {
    // Salvataggio “semplice e tradizionale” via stream, usando Aula::toXML()
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) {
        cerr << "Errore saveToFile(Aule): impossibile aprire '" << path << "'\n";
        return false;
    }

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<Aule>\n";

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != 0) {
            out << "  " << items[i]->toXML() << "\n";
        }
    }

    out << "</Aule>\n";
    out.close();

    if (!out.good()) {
        cerr << "Errore saveToFile(Aule): errore in scrittura su '" << path << "'\n";
        return false;
    }

    return true;
}

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
        if (items[i] != 0) {
            s += items[i]->toString();
            s += "--------------------\n";
        }
    }

    return s;
}