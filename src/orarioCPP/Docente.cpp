#include "Materia.h"
#include "libUtils.h"
#include <string>
using namespace std;

Docente::Docente()
{
    this->cognome = "";
    this->numeroClassi = 0;
    this->utente = null;
    this->orario = null;
}

Docente::Docente(string cognome, int numeroClassi, Utente utente, Orario orario) {
    this->cognome = cognome;
    this->numeroClassi = numeroClassi;
    this->utente = utente;
    this->orario = orario;
}

XMLElement* Docente::fromXML(XMLElement* docenteElem) {
    if (docenteElem == nullptr || string(docenteElem->Name()) != "Docente") {
        return nullptr;
    }
    Docente* d = new Docente();
    XMLElement* cognomeElem = docenteElem->FirstChildElement("Cognome");
    if (cognomeElem && cognomeElem->GetText()) {
        d->cognome = cognomeElem->GetText();
    }
    XMLElement* numeroClassiElem = docenteElem->FirstChildElement("NumeroClassi");
    if (numeroClassiElem && numeroClassiElem->GetText()) {
        d->numeroClassi = numeroClassiElem->GetText();
    }
    XMLElement* utenteElem = docenteElem->FirstChildElement("Utente");
    if (utenteElem && utenteElem->GetText()) {
        d->utente = utenteElem->GetText();
    }
    XMLElement* orarioElem = docenteElem->FirstChildElement("Orario");
    if (orarioElem && orarioElem->GetText()) {
        d->orario = orarioElem->GetText();
    }
    return d;
}

XMLElement* Docente::toXML(XMLDocument& doc) {
    XMLElement* DocenteElem = doc.NewElement("Docente");
    XMLElement* cognomeElem = doc.NewElement("Cognome");
    cognomeElem->SetText(this->cognome.c_str());
    docenteElem->InsertEndChild(cognomeElem);

    XMLElement* numeroClassiElem = doc.NewElement("NumeroClassi");
    numeroClassiElem->SetText(this->numeroClassi.c_str());
    docenteElem->InsertEndChild(numeroClassiElem);

    XMLElement* utenteElem = doc.NewElement("Utente");
    utenteElem->SetText(this->utente.c_str());
    docenteElem->InsertEndChild(utenteElem);

    XMLElement* orarioElem = doc.NewElement("Orario");
    orarioElem->SetText(this->orario.c_str());
    docenteElem->InsertEndChild(orarioElem);

    return docenteElem;
}

string Docente::escapeXML(string& s) {
    string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '&') out += "&amp;";
        else if (c == '<') out += "&lt;";
        else if (c == '>') out += "&gt;";
        else if (c == '"') out += "&quot;";
        else if (c == '\'') out += "&apos;";
        else out += c;
    }
    return out;
}

string Docente::toXML() {
    string xml;
    xml += "<Docente>";
    xml += "<Cognome>" + escapeXML(this->cognome) + "</Cognome>";
    xml += "<NumeroClassi>" + string(this->numeroClassi) + "</NumeroClassi>";
    xml += "<Utente>" + string(this->utente) + "</Utente>";
    xml += "<Orario>" + string(this->orario) + "</Orario>";
    xml += "</Docente>";
    return xml;
}

string Docente::toString() {
    string s;
    s += "Docente: " + this->cognome + "\n";
    s += "Numero Classi: " + string(this->numeroClassi) + "\n";
    s += "Utente: " + string(this->utente) + "\n";
    s += "Orario: " + string(this->orario) + "\n";
    return s;
}