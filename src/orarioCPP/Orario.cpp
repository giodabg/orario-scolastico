#include <iostream>
#include <fstream>
#include <sstream>
#include "Orario.h"

using namespace std;

// ============================================
// COSTRUTTORE
// ============================================
Orario::Orario() {
    // Vector vuoto per default
}

// ============================================
// DISTRUTTORE
// ============================================
Orario::~Orario() {
    clear();
}

// ============================================
// GESTIONE CONTENITORE
// ============================================
void Orario::clear() {
    for (size_t i = 0; i < items.size(); ++i) {
        delete items[i];
    }
    items.clear();

    codaProposte.clear();
}

size_t Orario::size() const {
    return items.size();
}

// ============================================
// CONTROLLO ESISTENZA ID
// ============================================
bool Orario::esisteId(int id) const {
    bool trovato = false;

    for (size_t i = 0; i < items.size() && !trovato; ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            trovato = true;
        }
    }

    return trovato;
}

// ============================================
// CARICAMENTO DA FILE XML (TinyXML2)
// Root attesa: <Orario>
// Contenuto:  <Lezione>...</Lezione>
// ============================================
bool Orario::loadFromFile(const string& path) {
    clear();

    XMLDocument doc;
    XMLError err = doc.LoadFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore loadFromFile(Orario): impossibile caricare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    XMLElement* root = doc.RootElement();
    if (root == nullptr || string(root->Name()) != "Orario") {
        cerr << "Errore loadFromFile(Orario): root non valida (attesa <Orario>)\n";
        return false;
    }

    XMLElement* lezioneElem = root->FirstChildElement("Lezione");
    while (lezioneElem != nullptr) {
        Lezione* l = Lezione::fromXML(lezioneElem);

        if (l == nullptr) {
            cerr << "Warning loadFromFile(Orario): nodo <Lezione> non valido, scartato\n";
            lezioneElem = lezioneElem->NextSiblingElement("Lezione");
        }
        else {
            int id = l->getId();

            if (esisteId(id)) {
                cerr << "Warning loadFromFile(Orario): ID duplicato (" << id << "), scarto\n";
                delete l;
                lezioneElem = lezioneElem->NextSiblingElement("Lezione");
            }
            else {
                items.push_back(l);
                lezioneElem = lezioneElem->NextSiblingElement("Lezione");
            }
        }
    }

    return true;
}

// ============================================
// SALVATAGGIO SU FILE XML (TinyXML2)
// ============================================
bool Orario::saveToFile(const string& path) const {
    XMLDocument doc;

    // Dichiarazione XML
    XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);

    // Root
    XMLElement* root = doc.NewElement("Orario");
    doc.InsertEndChild(root);

    // Contenuto
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            XMLElement* lezioneElem = items[i]->toXML(doc);
            root->InsertEndChild(lezioneElem);
        }
    }

    XMLError err = doc.SaveFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore saveToFile(Orario): impossibile salvare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    return true;
}

// ============================================
// CARICAMENTO DA FILE CSV
// Ogni riga rappresenta una Lezione nel formato Lezione::fromCSV
// ============================================
bool Orario::loadFromCSV(const string& path) {
    clear();

    ifstream in(path.c_str(), ios::in);
    if (!in.is_open()) {
        cerr << "Errore loadFromCSV(Orario): impossibile aprire '" << path << "'\n";
        return false;
    }

    string riga;
    while (getline(in, riga)) {
        if (riga.size() == 0) {
            // salta righe vuote
        }
        else {
            Lezione* l = new Lezione();
            l->fromCSV(riga);

            int id = l->getId();
            if (esisteId(id)) {
                cerr << "Warning loadFromCSV(Orario): ID duplicato (" << id << "), scarto\n";
                delete l;
            }
            else {
                items.push_back(l);
            }
        }
    }

    in.close();
    return true;
}

// ============================================
// SALVATAGGIO SU FILE CSV
// Ogni riga è Lezione::toCSV()
// ============================================
bool Orario::saveToCSV(const string& path) const {
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) {
        cerr << "Errore saveToCSV(Orario): impossibile aprire '" << path << "'\n";
        return false;
    }

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            out << items[i]->toCSV() << "\n";
        }
    }

    out.close();

    if (!out.good()) {
        cerr << "Errore saveToCSV(Orario): errore in scrittura su '" << path << "'\n";
        return false;
    }

    return true;
}

// ============================================
// CRUD - CREATE
// ============================================
bool Orario::aggiungi(Lezione* lezione) {
    if (lezione == nullptr) {
        cerr << "Errore aggiungi(Orario): puntatore nullo\n";
        return false;
    }

    if (esisteId(lezione->getId())) {
        cerr << "Errore aggiungi(Orario): ID " << lezione->getId() << " gia esistente\n";
        return false;
    }

    items.push_back(lezione);
    return true;
}

// ============================================
// CRUD - READ
// ============================================
Lezione* Orario::cercaPerId(int id) const {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            return items[i];
        }
    }
    return nullptr;
}

Lezione* Orario::get(size_t index) const {
    if (index < items.size()) {
        return items[index];
    }
    return nullptr;
}

// --------------------------------------------
// Ricerche specifiche richieste da Orario.h
// --------------------------------------------
vector<Lezione*> Orario::cercaPerGiorno(GiornoSettimana giorno) const {
    vector<Lezione*> trovate;

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getGiorno() == giorno) {
            trovate.push_back(items[i]);
        }
    }

    return trovate;
}

vector<Lezione*> Orario::cercaPerClasseId(int classeId) const {
    vector<Lezione*> trovate;

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getClasseId() == classeId) {
            trovate.push_back(items[i]);
        }
    }

    return trovate;
}

vector<Lezione*> Orario::cercaPerDocenteId(int docenteId) const {
    vector<Lezione*> trovate;

    for (size_t i = 0; i < items.size(); ++i) {
        Lezione* l = items[i];
        if (l != nullptr) {
            const vector<int>& dIds = l->getDocentiIds();

            bool ok = false;
            for (size_t j = 0; j < dIds.size() && !ok; ++j) {
                if (dIds[j] == docenteId) {
                    ok = true;
                }
            }

            if (ok) {
                trovate.push_back(l);
            }
        }
    }

    return trovate;
}

vector<Lezione*> Orario::cercaPerAulaId(int aulaId) const {
    vector<Lezione*> trovate;

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getAulaId() == aulaId) {
            trovate.push_back(items[i]);
        }
    }

    return trovate;
}

Lezione* Orario::cercaPerGiornoOraClasse(GiornoSettimana giorno, int ora, int classeId) const {
    for (size_t i = 0; i < items.size(); ++i) {
        Lezione* l = items[i];
        if (l != nullptr &&
            l->getGiorno() == giorno &&
            l->getOra() == ora &&
            l->getClasseId() == classeId) {
            return l;
        }
    }
    return nullptr;
}

// ============================================
// CRUD - UPDATE
// ============================================
bool Orario::modifica(int id, Lezione* nuovaLezione) {
    if (nuovaLezione == nullptr) {
        cerr << "Errore modifica(Orario): puntatore nullo\n";
        return false;
    }

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            delete items[i];
            items[i] = nuovaLezione;

            // Assicura ID coerente
            nuovaLezione->setId(id);

            return true;
        }
    }

    cerr << "Errore modifica(Orario): ID " << id << " non trovato\n";
    return false;
}

// ============================================
// CRUD - DELETE
// ============================================
bool Orario::rimuoviPerId(int id) {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr && items[i]->getId() == id) {
            delete items[i];
            items.erase(items.begin() + i);
            return true;
        }
    }

    cerr << "Errore rimuoviPerId(Orario): ID " << id << " non trovato\n";
    return false;
}

bool Orario::rimuoviPerIndex(size_t index) {
    if (index >= items.size()) {
        cerr << "Errore rimuoviPerIndex(Orario): indice " << index << " fuori range\n";
        return false;
    }

    if (items[index] != nullptr) {
        delete items[index];
    }

    items.erase(items.begin() + index);
    return true;
}

// ============================================
// RESOLVE - collega tutti i riferimenti (ID -> puntatori)
// - delega al metodo Lezione::resolve(...)
// ============================================
void Orario::resolveAll(const vector<Classe*>& tutteClassi,
    const vector<Aula*>& tutteAule,
    const vector<Materia*>& tutteMaterie,
    const vector<Docente*>& tuttiDocenti) {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            items[i]->resolve(tutteClassi, tutteAule, tutteMaterie, tuttiDocenti);
        }
    }
}

bool Orario::aggiungiProposta(const PropostaModifica& p) {
    codaProposte.push_back(p);
    return true;
}

size_t Orario::sizeProposte() const {
    return codaProposte.size();
}

PropostaModifica* Orario::getProposta(size_t index) {
    if (index < codaProposte.size()) {
        return &(codaProposte[index]);
    }
    return nullptr;
}

void Orario::resolveProposte(const vector<Utente*>& tuttiUtenti) {
    for (size_t i = 0; i < codaProposte.size(); ++i) {
        codaProposte[i].resolveProponente(tuttiUtenti);
        codaProposte[i].resolveLezioni(*this);
    }
}

bool Orario::saveProposteToFile(const string& path) const {
    XMLDocument doc;

    XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);

    XMLElement* root = doc.NewElement("ProposteModifica");
    doc.InsertEndChild(root);

    for (size_t i = 0; i < codaProposte.size(); ++i) {
        XMLElement* pElem = codaProposte[i].toXML(doc);
        root->InsertEndChild(pElem);
    }

    XMLError err = doc.SaveFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore saveProposteToFile(Orario): impossibile salvare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    return true;
}

bool Orario::loadProposteFromFile(const string& path) {
    codaProposte.clear();

    XMLDocument doc;
    XMLError err = doc.LoadFile(path.c_str());
    if (err != XML_SUCCESS) {
        cerr << "Errore loadProposteFromFile(Orario): impossibile caricare '" << path
            << "': " << doc.ErrorStr() << "\n";
        return false;
    }

    XMLElement* root = doc.RootElement();
    if (root == nullptr || string(root->Name()) != "ProposteModifica") {
        cerr << "Errore loadProposteFromFile(Orario): root non valida (attesa <ProposteModifica>)\n";
        return false;
    }

    XMLElement* propostaElem = root->FirstChildElement("PropostaModifica");
    while (propostaElem != nullptr) {
        PropostaModifica* p = PropostaModifica::fromXML(propostaElem);
        if (p == nullptr) {
            cerr << "Warning loadProposteFromFile(Orario): nodo <PropostaModifica> non valido, scartato\n";
        }
        else {
            codaProposte.push_back(*p);
            delete p;
        }

        propostaElem = propostaElem->NextSiblingElement("PropostaModifica");
    }

    return true;
}

bool Orario::saveProposteToCSV(const string& path) const {
    ofstream out(path.c_str(), ios::out | ios::trunc);
    if (!out.is_open()) {
        cerr << "Errore saveProposteToCSV(Orario): impossibile aprire '" << path << "'\n";
        return false;
    }

    for (size_t i = 0; i < codaProposte.size(); ++i) {
        out << codaProposte[i].toCSV() << "\n";
    }

    out.close();
    if (!out.good()) {
        cerr << "Errore saveProposteToCSV(Orario): errore in scrittura su '" << path << "'\n";
        return false;
    }

    return true;
}

bool Orario::loadProposteFromCSV(const string& path) {
    codaProposte.clear();

    ifstream in(path.c_str(), ios::in);
    if (!in.is_open()) {
        cerr << "Errore loadProposteFromCSV(Orario): impossibile aprire '" << path << "'\n";
        return false;
    }

    string riga;
    while (getline(in, riga)) {
        if (riga.size() == 0) {
            // skip
        }
        else {
            PropostaModifica p;
            p.fromCSV(riga);
            codaProposte.push_back(p);
        }
    }

    in.close();
    return true;
}
// ============================================
// UTILITY - toXML (stringa)
// ============================================
string Orario::toXML() const {
    string xml;

    xml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml += "<Orario>\n";

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            xml += "  " + items[i]->toXML() + "\n";
        }
    }

    xml += "</Orario>\n";
    return xml;
}

// ============================================
// UTILITY - toString
// ============================================
string Orario::toString() const {
    string s;

    s += "=== ORARIO ===\n";
    s += "Numero lezioni: ";
    s += to_string(items.size());
    s += "\n\n";

    if (items.size() == 0) {
        s += "(nessuna lezione caricata)\n";
        return s;
    }

    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] != nullptr) {
            s += items[i]->toString();
            s += "--------------------\n";
        }
    }

    s += "\n=== PROPOSTE MODIFICA ===\n";
    s += "Numero proposte: ";
    s += to_string(codaProposte.size());
    s += "\n\n";

    if (codaProposte.size() == 0) {
        s += "(nessuna proposta presente)\n";
    }
    else {
        for (size_t i = 0; i < codaProposte.size(); ++i) {
            s += codaProposte[i].toString();
            s += "--------------------\n";
        }
    }
    return s;
}