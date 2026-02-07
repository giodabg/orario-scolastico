#include <iostream>
#include <fstream>

#include "Aule.h"

using namespace std;

Aule::~Aule() {
    clear();
}

void Aule::clear() {
    // dealloca tutti gli items possedute dal contenitore
    for (size_t i = 0; i < items.size(); ++i) {
        delete items[i];
    }
    items.clear();
}

size_t Aule::size() const {
    return items.size();
}

// Controlla che id non sia duplicato
bool Aule::esisteId(int id) const {

    // Controlla se l'id è già presente
    bool trovato = false;
    for (size_t j = 0; j < items.size(); ++j) {
        if (items[j]->getId() == id) {
            return true;  // Duplicato trovato
        }
    }
    return false;
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
    while (aulaElem != nullptr) {
        Aula* a = Aula::fromXML(aulaElem);
        if (a == nullptr) {
            cerr << "Warning loadFromFile(Aule): nodo <Aula> non valido, scartato\n";
			// passo al prossimo nodo <Aula>
            aulaElem = aulaElem->NextSiblingElement("Aula");
			// continua con il prossimo ciclo, senza aggiungere nulla al contenitore
            continue;
        }

        int id = a->getId();

        // gestisci duplicati
        if (esisteId(id)) {
            cerr << "Warning loadFromFile(Aule): ID duplicato (" << id << "), scarto\n";
			// cancello l'oggetto appena creato, che non sarà aggiunto al contenitore
            delete a;
			// passo al prossimo nodo <Aula>
            aulaElem = aulaElem->NextSiblingElement("Aula");
        }
        else {
			// ID univoco, posso aggiungere al contenitore
            items.push_back(a);

            // passo al prossimo nodo <Aula>
            aulaElem = aulaElem->NextSiblingElement("Aula");
        }
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
        if (items[i] != nullptr) {
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
        if (items[i] != nullptr) {
            s += items[i]->toString();
            s += "--------------------\n";
        }
    }

    return s;
}