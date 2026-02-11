#include "Studenti.h"
#include "Studenti.h"
using namespace std;

Studenti::~Studenti() {
    clear();
}

void Studenti::clear() {
    for (size_t i = 0; i < items.size(); ++i) {
        delete items[i];
    }
    items.clear();
}

size_t Studenti::size() const {
    return items.size();
}

// Controllo duplicati usando la stringa ID
bool Studenti::esisteId(const string& id) const {
    for (size_t j = 0; j < items.size(); ++j) {
        if (items[j]->getId() == id) { 
            return true;
        }
    }
    return false;
}

bool Studenti::loadFromFile(const string& path) {
    clear();

    XMLDocument doc;
    XMLError err = doc.LoadFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore loadFromFile(Studenti): " << doc.ErrorStr() << "\n";
        return false;
    }

    XMLElement* root = doc.RootElement();
    if (root == nullptr || string(root->Name()) != "Studenti") {
        cerr << "Errore: root non valida (attesa <Studenti>)\n";
        return false;
    }

    XMLElement* studElem = root->FirstChildElement("Studente");
    while (studElem != nullptr) {
        XMLPrinter printer;
        studElem->Accept(&printer);
        string xmlString = printer.CStr();

        Studente* s = new Studente();
        s->fromXML(xmlString); 

        if (esisteId(s->getId())) {
            cerr << "Warning: ID duplicato (" << s->getId() << "), scarto\n";
            delete s;
        }
        else {
            items.push_back(s);
        }

        studElem = studElem->NextSiblingElement("Studente");
    }

    return true;
}

bool Studenti::saveToFile(const string& path) const {
    XMLDocument doc;

    // Dichiarazione XML
    XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);

    
    XMLElement* root = doc.NewElement("Studenti");
    doc.InsertEndChild(root);

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            XMLDocument tempDoc;
            tempDoc.Parse(items[i]->toXML().c_str());

            if (!tempDoc.Error()) {
                XMLNode* importNode = doc.NewElement("Studente"); // Placeholder
                importNode = tempDoc.FirstChildElement("Studente")->DeepClone(&doc);
                root->InsertEndChild(importNode);
            }
        }
    }

    return doc.SaveFile(path.c_str()) == XML_SUCCESS;
}

bool Studenti::saveToFileOld(const string& path) const {
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) return false;

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<Studenti>\n";

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            out << items[i]->toXML() << "\n";
        }
    }

    out << "</Studenti>\n";
    out.close();
    return out.good();
}

string Studenti::toString() const {
    string s = "=== ELENCO STUDENTI ===\n";
    s += "Numero totale: " + to_string(items.size()) + "\n\n";

    if (items.empty()) {
        s += "(nessun record)\n";
    }
    else {
        for (size_t i = 0; i < items.size(); ++i) {
            s += items[i]->toString() + "\n";
            s += "--------------------\n";
        }
    }
    return s;
}