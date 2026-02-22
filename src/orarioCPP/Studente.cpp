#include "Studente.h"
#include "Classe.h"
#include "libUtils.h"
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

// ============================================
// COSTRUTTORE
// ============================================
Studente::Studente() {
    tipo = "normale";
    classeId = -1;
    classe = nullptr;
}

// ============================================
// TIPO UTENTE
// ============================================
string Studente::getTipoUtente() const {
    return "STUDENTE";
}

// ============================================
// CSV - SCRITTURA (toCSV)
// Formato coerente con gli altri oggetti: campi base + campi Studente
// ============================================
string Studente::toCSV() const {
    char splitter = ',';

    // NB: qui assumo che Utente esponga getter base:
    // getId(), getUsername(), getPassword(), getNome(), getCognome()
    string CSV = to_string(this->getId()) + splitter;
    CSV += this->getUsername() + splitter;
    CSV += this->getPassword() + splitter;
    CSV += this->getNome() + splitter;
    CSV += this->getCognome() + splitter;

    // Campi specifici Studente
    CSV += this->tipo + splitter;
    CSV += to_string(this->classeId);

    return CSV;
}

// ============================================
// CSV - LETTURA (fromCSV)
// ============================================
void Studente::fromCSV(const string& riga) {
    stringstream ss(riga);
    string temp;

    // ID
    if (getline(ss, temp, ',')) {
        this->setId(estraiIdNumerico(temp));
    }

    // Username
    if (getline(ss, temp, ',')) {
        this->setUsername(temp);
    }

    // Password
    if (getline(ss, temp, ',')) {
        this->setPassword(temp);
    }

    // Nome
    if (getline(ss, temp, ',')) {
        this->setNome(temp);
    }

    // Cognome
    if (getline(ss, temp, ',')) {
        this->setCognome(temp);
    }

    // Tipo
    if (getline(ss, temp, ',')) {
        this->tipo = temp;
    }
    else {
        this->tipo = "normale";
    }

    // ClasseId
    if (getline(ss, temp, ',')) {
        if (!temp.empty()) {
            this->classeId = atoi(temp.c_str());
        }
        else {
            this->classeId = -1;
        }
    }
    else {
        this->classeId = -1;
    }

    // Il puntatore runtime si risolve dopo (resolve)
    this->classe = nullptr;
}

// ============================================
// XML - LETTURA (fromXML)
// Atteso:
// <Studente id="STU-1">
//   <Username>...</Username>
//   <Password>...</Password>
//   <Nome>...</Nome>
//   <Cognome>...</Cognome>
//   <Tipo>normale</Tipo>
//   <ClasseRef id="CLA-3"/>
// </Studente>
// ============================================
Studente* Studente::fromXML(XMLElement* studenteElem) {
    if (studenteElem == nullptr || string(studenteElem->Name()) != "Studente") {
        return nullptr;
    }

    Studente* s = new Studente();

    // id come attributo
    const char* idAttr = studenteElem->Attribute("id");
    if (idAttr) {
        s->setId(estraiIdNumerico(idAttr));
    }

    // Username
    XMLElement* userElem = studenteElem->FirstChildElement("Username");
    if (userElem && userElem->GetText()) {
        s->setUsername(userElem->GetText());
    }

    // Password
    XMLElement* passElem = studenteElem->FirstChildElement("Password");
    if (passElem && passElem->GetText()) {
        s->setPassword(passElem->GetText());
    }

    // Nome
    XMLElement* nomeElem = studenteElem->FirstChildElement("Nome");
    if (nomeElem && nomeElem->GetText()) {
        s->setNome(nomeElem->GetText());
    }

    // Cognome
    XMLElement* cognomeElem = studenteElem->FirstChildElement("Cognome");
    if (cognomeElem && cognomeElem->GetText()) {
        s->setCognome(cognomeElem->GetText());
    }

    // Tipo
    XMLElement* tipoElem = studenteElem->FirstChildElement("Tipo");
    if (tipoElem && tipoElem->GetText()) {
        s->tipo = tipoElem->GetText();
    }
    else {
        s->tipo = "normale";
    }

    // ClasseRef (solo id, puntatore risolto dopo)
    XMLElement* classeRef = studenteElem->FirstChildElement("ClasseRef");
    if (classeRef) {
        const char* claIdAttr = classeRef->Attribute("id");
        if (claIdAttr) {
            s->classeId = estraiIdNumerico(claIdAttr);
        }
    }

    s->classe = nullptr;
    return s;
}

// ============================================
// XML - SCRITTURA (toXML con TinyXML2)
// ============================================
XMLElement* Studente::toXML(XMLDocument& doc) const {
    XMLElement* studenteElem = doc.NewElement("Studente");

    // id attribute
    string idStr = "STU-" + to_string(this->getId());
    studenteElem->SetAttribute("id", idStr.c_str());

    XMLElement* userElem = doc.NewElement("Username");
    userElem->SetText(this->getUsername().c_str());
    studenteElem->InsertEndChild(userElem);

    XMLElement* passElem = doc.NewElement("Password");
    passElem->SetText(this->getPassword().c_str());
    studenteElem->InsertEndChild(passElem);

    XMLElement* nomeElem = doc.NewElement("Nome");
    nomeElem->SetText(this->getNome().c_str());
    studenteElem->InsertEndChild(nomeElem);

    XMLElement* cognomeElem = doc.NewElement("Cognome");
    cognomeElem->SetText(this->getCognome().c_str());
    studenteElem->InsertEndChild(cognomeElem);

    XMLElement* tipoElem = doc.NewElement("Tipo");
    tipoElem->SetText(tipo.c_str());
    studenteElem->InsertEndChild(tipoElem);

    // ClasseRef
    if (classeId >= 0) {
        XMLElement* classeRef = doc.NewElement("ClasseRef");
        string claIdStr = "CLA-" + to_string(classeId);
        classeRef->SetAttribute("id", claIdStr.c_str());
        studenteElem->InsertEndChild(classeRef);
    }

    return studenteElem;
}

// ============================================
// XML - SCRITTURA STRING (toXML stringa)
// ============================================
string Studente::toXML() const {
    string xml;

    xml += "<Studente id=\"";
    xml += "STU-" + to_string(this->getId());
    xml += "\">";

    xml += "<Username>";
    xml += escapeXML(this->getUsername());
    xml += "</Username>";

    xml += "<Password>";
    xml += escapeXML(this->getPassword());
    xml += "</Password>";

    xml += "<Nome>";
    xml += escapeXML(this->getNome());
    xml += "</Nome>";

    xml += "<Cognome>";
    xml += escapeXML(this->getCognome());
    xml += "</Cognome>";

    xml += "<Tipo>";
    xml += escapeXML(tipo);
    xml += "</Tipo>";

    if (classeId >= 0) {
        xml += "<ClasseRef id=\"CLA-";
        xml += to_string(classeId);
        xml += "\"/>";
    }

    xml += "</Studente>";
    return xml;
}

// ============================================
// RAPPRESENTAZIONE TESTUALE (toString)
// ============================================
string Studente::toString() const {
    string s;

    s += "Studente ID: ";
    s += "STU-" + to_string(this->getId());
    s += "\n";

    s += "  Username: ";
    s += this->getUsername();
    s += "\n";

    s += "  Nome: ";
    s += this->getNome();
    s += "\n";

    s += "  Cognome: ";
    s += this->getCognome();
    s += "\n";

    s += "  Tipo: ";
    s += tipo;
    s += "\n";

    s += "  ClasseId: ";
    if (classeId >= 0) {
        s += "CLA-" + to_string(classeId);
    }
    else {
        s += "(non assegnata)";
    }
    s += "\n";

    s += "  Classe (ptr): ";
    if (classe != nullptr) {
        s += "OK";
    }
    else {
        s += "NULL";
    }
    s += "\n";

    return s;
}
