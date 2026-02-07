#include "Materia.h"
#include "libUtils.h"
#include <string>
using namespace std;

Materia::Materia()
{
    this->nome = "";
    this->Laboratorio = false;
}

Materia::Materia(string nome, bool Laboratorio) {
    this->nome = nome;
    this->Laboratorio = Laboratorio;
}

Materia* Materia::fromXML(XMLElement* materiaElem) {
    if (materiaElem == nullptr || string(materiaElem->Name()) != "Materia") {
        return nullptr;
    }
    Materia* m = new Materia();
    XMLElement* nomeElem = materiaElem->FirstChildElement("Nome");
    if (nomeElem && nomeElem->GetText()) {
        m->nome = nomeElem->GetText();
    }
    XMLElement* labElem = materiaElem->FirstChildElement("Laboratorio");
    if (labElem && labElem->GetText()) {
        string labStr = labElem->GetText();
        m->Laboratorio = (labStr == "true" || labStr == "1");
    }
    return m;
}

XMLElement* Materia::toXML(XMLDocument& doc) const {
    XMLElement* materiaElem = doc.NewElement("Materia");
    XMLElement* nomeElem = doc.NewElement("Nome");
    nomeElem->SetText(this->nome.c_str());
    materiaElem->InsertEndChild(nomeElem);
    XMLElement* labElem = doc.NewElement("Laboratorio");
    labElem->SetText(this->Laboratorio ? "true" : "false");
    materiaElem->InsertEndChild(labElem);
    return materiaElem;
}

string Materia::escapeXML(const string& s) {
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

string Materia::toXML() const {
    string xml;
    xml += "<Materia>";
    xml += "<Nome>" + escapeXML(this->nome) + "</Nome>";
    xml += "<Laboratorio>" + string(this->Laboratorio ? "true" : "false") + "</Laboratorio>";
    xml += "</Materia>";
    return xml;
}

string Materia::toString() const {
    string s;
    s += "Materia: " + this->nome + "\n";
    s += "  Richiede Laboratorio: " + string(this->Laboratorio ? "Sì" : "No") + "\n";
    return s;
}