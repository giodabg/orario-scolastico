#include <iostream>
#include <fstream>

#include "Materie.h"

using namespace std;

Materie::~Materie() {
    clear();
}

void Materie::clear() {
    // dealloca tutti gli items possedute dal contenitore
    for (size_t i = 0; i < items.size(); ++i) {
        delete items[i];
    }
    items.clear();
}

size_t Materie::size() const {
    return items.size();
}

// Controlla che id non sia duplicato
bool Materie::esisteId(int id) const {

    // Controlla se l'id è già presente
    bool trovato = false;
    for (size_t j = 0; j < items.size(); ++j) {
        if (items[j]->getId() == id) {
            return true;  // Duplicato trovato
        }
    }
    return false;
}

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
    if (root == 0 || string(root->Name()) != "Materie") {
        cerr << "Errore loadFromFile(Materie): root non valida (attesa <Materie>)\n";
        return false;
    }

    XMLElement* materiaElem = root->FirstChildElement("Materia");
    while (materiaElem != nullptr) {
        Materia* m = Materia::fromXML(materiaElem);
        if (m == nullptr) {
            cerr << "Warning loadFromFile(Materie): nodo <Materia> non valido, scartato\n";
            // passo al prossimo nodo <Materia>
            materiaElem = materiaElem->NextSiblingElement("Materia");
            // continua con il prossimo ciclo, senza aggiungere nulla al contenitore
            continue;
        }

        int id = m->getId();

        // gestisci duplicati
        if (esisteId(id)) {
            cerr << "Warning loadFromFile(Materie): ID duplicato (" << id << "), scarto\n";
            // cancello l'oggetto appena creato, che non sarà aggiunto al contenitore
            delete m;
            // passo al prossimo nodo <Materia>
            materiaElem = materiaElem->NextSiblingElement("Materia");
        }
        else {
            // ID univoco, posso aggiungere al contenitore
            items.push_back(m);

            // passo al prossimo nodo <Materie>
            materiaElem = materiaElem->NextSiblingElement("Materia");
        }
    }

    return true;
}


// ------------------------------------------------------
// Materie::saveToFile (TinyXML2)
// Scrive:
//
// <?xml version="1.0" encoding="UTF-8"?>
// <Materie>
//   <Materia ...> ... </Materia>
// </Materia>
// ------------------------------------------------------
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

bool Materie::saveToFileOld(const string& path) const {
    // Salvataggio “semplice e tradizionale” via stream, usando Materia::toXML()
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) {
        cerr << "Errore saveToFile(Materie): impossibile aprire '" << path << "'\n";
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
        cerr << "Errore saveToFile(Materie): errore in scrittura su '" << path << "'\n";
        return false;
    }

    return true;
}

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