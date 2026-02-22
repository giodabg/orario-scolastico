#include "Utente.h"
#include "libUtils.h"
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

// ============================================
// COSTRUTTORE
// ============================================
Utente::Utente() {
    id = -1;
    nome = "";
    cognome = "";
    username = "";
    password = "";
}

// ============================================
// TIPO UTENTE
// ============================================
string Utente::getTipoUtente() const {
    return "UTENTE";
}

// ============================================
// XML - SCRITTURA (toXML con TinyXML2)
// ============================================
XMLElement* Utente::toXML(XMLDocument& doc) const {
    XMLElement* utenteElem = doc.NewElement("Utente");

    // id attribute
    string idStr = "UTE-" + to_string(this->id);
    utenteElem->SetAttribute("id", idStr.c_str());

    XMLElement* userElem = doc.NewElement("Username");
    userElem->SetText(username.c_str());
    utenteElem->InsertEndChild(userElem);

    XMLElement* passElem = doc.NewElement("Password");
    passElem->SetText(password.c_str());
    utenteElem->InsertEndChild(passElem);

    XMLElement* nomeElem = doc.NewElement("Nome");
    nomeElem->SetText(nome.c_str());
    utenteElem->InsertEndChild(nomeElem);

    XMLElement* cognomeElem = doc.NewElement("Cognome");
    cognomeElem->SetText(cognome.c_str());
    utenteElem->InsertEndChild(cognomeElem);

    return utenteElem;
}

// ============================================
// XML - LETTURA (fromXML)
// Atteso:
// <Utente id="UTE-1">
//   <Username>...</Username>
//   <Password>...</Password>
//   <Nome>...</Nome>
//   <Cognome>...</Cognome>
// </Utente>
// ============================================
Utente* Utente::fromXML(XMLElement* utenteElem) {
    if (utenteElem == nullptr || string(utenteElem->Name()) != "Utente") {
        return nullptr;
    }

    Utente* u = new Utente();

    // id come attributo
    const char* idAttr = utenteElem->Attribute("id");
    if (idAttr) {
        u->id = estraiIdNumerico(idAttr);
    }

    // Username
    XMLElement* userElem = utenteElem->FirstChildElement("Username");
    if (userElem && userElem->GetText()) {
        u->username = userElem->GetText();
    }

    // Password
    XMLElement* passElem = utenteElem->FirstChildElement("Password");
    if (passElem && passElem->GetText()) {
        u->password = passElem->GetText();
    }

    // Nome
    XMLElement* nomeElem = utenteElem->FirstChildElement("Nome");
    if (nomeElem && nomeElem->GetText()) {
        u->nome = nomeElem->GetText();
    }

    // Cognome
    XMLElement* cognomeElem = utenteElem->FirstChildElement("Cognome");
    if (cognomeElem && cognomeElem->GetText()) {
        u->cognome = cognomeElem->GetText();
    }

    return u;
}

// ============================================
// CSV - SCRITTURA (toCSV)
// Formato:
// id,username,password,nome,cognome
// ============================================
string Utente::toCSV() const {
    char splitter = ',';

    string CSV = to_string(this->id) + splitter;
    CSV += this->username + splitter;
    CSV += this->password + splitter;
    CSV += this->nome + splitter;
    CSV += this->cognome;

    return CSV;
}

// ============================================
// CSV - LETTURA (fromCSV)
// ============================================
void Utente::fromCSV(const string& riga) {
    stringstream ss(riga);
    string temp;

    // ID
    if (getline(ss, temp, ',')) {
        this->id = estraiIdNumerico(temp);
    }

    // Username
    if (getline(ss, temp, ',')) {
        this->username = temp;
    }

    // Password
    if (getline(ss, temp, ',')) {
        this->password = temp;
    }

    // Nome
    if (getline(ss, temp, ',')) {
        this->nome = temp;
    }

    // Cognome
    if (getline(ss, temp, ',')) {
        this->cognome = temp;
    }
}

// ============================================
// XML - SCRITTURA STRING (toXML stringa)
// ============================================
string Utente::toXML() const {
    string xml;

    xml += "<Utente id=\"";
    xml += "UTE-" + to_string(id);
    xml += "\">";

    xml += "<Username>";
    xml += escapeXML(username);
    xml += "</Username>";

    xml += "<Password>";
    xml += escapeXML(password);
    xml += "</Password>";

    xml += "<Nome>";
    xml += escapeXML(nome);
    xml += "</Nome>";

    xml += "<Cognome>";
    xml += escapeXML(cognome);
    xml += "</Cognome>";

    xml += "</Utente>";
    return xml;
}

// ============================================
// RAPPRESENTAZIONE TESTUALE (toString)
// ============================================
string Utente::toString() const {
    string s;

    s += "Utente ID: ";
    s += "UTE-" + to_string(id);
    s += "\n";

    s += "  Username: ";
    s += username;
    s += "\n";

    s += "  Nome: ";
    s += nome;
    s += "\n";

    s += "  Cognome: ";
    s += cognome;
    s += "\n";

    return s;
}
