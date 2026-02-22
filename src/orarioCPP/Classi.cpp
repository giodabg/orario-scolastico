#include <iostream>
#include <fstream>
#include "Classi.h"
#include "Studenti.h"

using namespace std;

// ============================================
// COSTRUTTORE
// ============================================
Classi::Classi() {
    // vector vuoto per default
}

// ============================================
// GET
// ============================================
Classe* Classi::get(size_t index) const {
    if (index < items.size()) {
        return items[index];
    }
    return nullptr;
}

// ============================================
// DISTRUTTORE
// - libera la memoria delle Classi allocate dinamicamente
// ============================================
Classi::~Classi() {
    clear();
}

// ============================================
// CLEAR
// - elimina tutti gli oggetti Classe* presenti nel contenitore
// - svuota il vector
// ============================================
void Classi::clear() {
    for (size_t i = 0; i < items.size(); ++i) {
        delete items[i];
    }
    items.clear();
}

// ============================================
// SIZE
// - numero di classi attualmente presenti nel contenitore
// ============================================
size_t Classi::size() const {
    return items.size();
}

// ============================================
// CONTROLLO ESISTENZA ID
// - ritorna true se esiste già una classe con quell'ID
// ============================================
bool Classi::esisteId(int id) const {
    bool trovato = false;
    for (size_t j = 0; j < items.size() && !trovato; ++j) {
        if (items[j]->getId() == id) {
            trovato = true;
        }
    }
    return trovato;
}

// ============================================
// CARICAMENTO DA FILE XML (TinyXML2)
// - svuota il contenitore
// - carica il documento XML
// - legge <Classi> come root
// - per ogni <Classe> crea l'oggetto tramite Classe::fromXML
// - evita duplicati di ID
// ============================================
bool Classi::loadFromFile(const string& path) {
    clear();

    XMLDocument doc;
    XMLError err = doc.LoadFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore loadFromFile(Classi): impossibile caricare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    // Controllo root
    XMLElement* root = doc.RootElement();
    if (root == nullptr || string(root->Name()) != "Classi") {
        cerr << "Errore loadFromFile(Classi): root non valida (attesa <Classi>)\n";
        return false;
    }

    // Lettura sequenziale dei nodi <Classe>
    XMLElement* classeElem = root->FirstChildElement("Classe");
    while (classeElem != nullptr) {
        Classe* c = Classe::fromXML(classeElem);

        if (c == nullptr) {
            // nodo non interpretabile
            cerr << "Warning loadFromFile(Classi): nodo <Classe> non valido, scartato\n";
            classeElem = classeElem->NextSiblingElement("Classe");
        }
        else {
            int id = c->getId();

            // controllo duplicati
            if (esisteId(id)) {
                cerr << "Warning loadFromFile(Classi): ID duplicato (" << id << "), scarto\n";
                delete c;
                classeElem = classeElem->NextSiblingElement("Classe");
            }
            else {
                items.push_back(c);
                classeElem = classeElem->NextSiblingElement("Classe");
            }
        }
    }

    return true;
}

// ============================================
// SALVATAGGIO SU FILE XML (TinyXML2)
// - crea documento XML
// - inserisce dichiarazione + root <Classi>
// - per ogni Classe inserisce il relativo nodo prodotto da toXML(doc)
// ============================================
bool Classi::saveToFile(const string& path) const {
    XMLDocument doc;

    // Dichiarazione XML
    XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);

    // Root
    XMLElement* root = doc.NewElement("Classi");
    doc.InsertEndChild(root);

    // Contenuto
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            XMLElement* classeElem = items[i]->toXML(doc);
            root->InsertEndChild(classeElem);
        }
    }

    // Scrittura file
    XMLError err = doc.SaveFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore saveToFile(Classi): impossibile salvare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    return true;
}

// ============================================
// SALVATAGGIO SU FILE XML (VERSIONE STREAM)
// - costruisce il file XML come stringa/stream (senza TinyXML2)
// - utile come versione "tradizionale" / di debug
// ============================================
bool Classi::saveToFileOld(const string& path) const {
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) {
        cerr << "Errore saveToFileOld(Classi): impossibile aprire '" << path << "'\n";
        return false;
    }

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<Classi>\n";

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            out << "  " << items[i]->toXML() << "\n";
        }
    }

    out << "</Classi>\n";
    out.close();

    if (!out.good()) {
        cerr << "Errore saveToFileOld(Classi): errore in scrittura su '" << path << "'\n";
        return false;
    }

    return true;
}

// ============================================
// TOSTRING
// - rappresentazione testuale del contenitore
// - stampa numero classi e dettaglio di ciascuna Classe
// ============================================
string Classi::toString() const {
    string s;

    s += "=== ELENCO CLASSI ===\n";
    s += "Numero totale: ";
    s += to_string(items.size());
    s += "\n\n";

    if (items.size() == 0) {
        s += "(nessuna classe caricata)\n";
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

// ============================================
// RESOLVE - collega ID -> puntatori
// Scopo:
// 1) per ogni Classe: studentiIds -> studenti (puntatori reali a Studente)
// 2) per ogni Studente: classeId -> classe (puntatore reale a Classe)
// Nota:
// - gli ID vengono caricati da XML/CSV
// - i puntatori vengono ricostruiti a runtime dopo il caricamento
// ============================================
bool Classi::resolve(Studenti& studenti) {
    bool tuttoOk = true;

    // 1) Costruisco un vettore con tutti gli studenti
    //    (serve a Classe::resolveStudenti per cercare i puntatori a partire dagli ID)
    vector<Studente*> tuttiStudenti;
    for (size_t i = 0; i < studenti.size(); ++i) {
        Studente* s = studenti.get(i);
        if (s != nullptr) {
            tuttiStudenti.push_back(s);
        }
    }

    // 2) Risolvo per ogni Classe: studentiIds -> studenti (puntatori)
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            items[i]->resolveStudenti(tuttiStudenti);
        }
        else {
            tuttoOk = false;
        }
    }

    // 3) Risolvo per ogni Studente: classeId -> classe (puntatore)
    for (size_t i = 0; i < studenti.size(); ++i) {
        Studente* s = studenti.get(i);
        if (s != nullptr) {
            int cId = s->getClasseId();
            Classe* c = this->cercaPerId(cId);

            if (c != nullptr) {
                s->resolveClasse(c);
            }
            else {
                // classe non trovata: lascio nullptr, ma segnalo fallimento
                s->resolveClasse(nullptr);
                tuttoOk = false;
            }
        }
        else {
            tuttoOk = false;
        }
    }

    return tuttoOk;
}

Classe* Classi::cercaPerId(int id) const {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            return items[i];
        }
    }
    return nullptr;
}