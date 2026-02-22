#include "Materia.h"
#include "Docente.h"
#include "libUtils.h"
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

// ============================================
// COSTRUTTORE
// ============================================
Materia::Materia() {
    id = -1;
    nome = "";
    laboratorio = false;
    // docentiIds e docenti vuoti per default
}

Materia::Materia(int id, string nome, bool laboratorio) {
    this->id = id;
    this->nome = nome;
    this->laboratorio = laboratorio;
}

// ============================================
// METODO DI RISOLUZIONE
// ============================================
void Materia::resolveDocenti(const vector<Docente*>& tuttiDocenti) {
    docenti.clear();

    for (size_t i = 0; i < docentiIds.size(); ++i) {
        int docenteId = docentiIds[i];

        for (size_t j = 0; j < tuttiDocenti.size(); ++j) {
            if (tuttiDocenti[j] != nullptr && tuttiDocenti[j]->getId() == docenteId) {
                docenti.push_back(tuttiDocenti[j]);
            }
        }
    }
}

// ============================================
// XML - LETTURA (fromXML)
// ============================================
Materia* Materia::fromXML(XMLElement* materiaElem) {
    if (materiaElem == nullptr || string(materiaElem->Name()) != "Materia") {
        return nullptr;
    }

    Materia* m = new Materia();

    // Leggi id come attributo
    const char* idAttr = materiaElem->Attribute("id");
    if (idAttr) {
        m->setId(estraiIdNumerico(idAttr));
    }

    // Leggi Nome
    XMLElement* nomeElem = materiaElem->FirstChildElement("Nome");
    if (nomeElem && nomeElem->GetText()) {
        m->nome = nomeElem->GetText();
    }

    // Leggi Laboratorio
    XMLElement* labElem = materiaElem->FirstChildElement("Laboratorio");
    if (labElem && labElem->GetText()) {
        // accetta "0/1" o "true/false" (con fallback su atoi)
        string t = labElem->GetText();
        if (t == "true" || t == "TRUE" || t == "True") {
            m->laboratorio = true;
        }
        else if (t == "false" || t == "FALSE" || t == "False") {
            m->laboratorio = false;
        }
        else {
            m->laboratorio = (atoi(t.c_str()) != 0);
        }
    }
    else {
        m->laboratorio = false;
    }

    // Leggi Docenti
    XMLElement* docentiElem = materiaElem->FirstChildElement("Docenti");
    if (docentiElem) {
        XMLElement* docenteRef = docentiElem->FirstChildElement("DocenteRef");
        while (docenteRef != nullptr) {
            const char* docenteIdAttr = docenteRef->Attribute("id");
            if (docenteIdAttr) {
                int dId = estraiIdNumerico(docenteIdAttr);
                m->addDocenteId(dId);
            }
            docenteRef = docenteRef->NextSiblingElement("DocenteRef");
        }
    }

    return m;
}

// ============================================
// XML - SCRITTURA (toXML con TinyXML2)
// ============================================
XMLElement* Materia::toXML(XMLDocument& doc) const {
    XMLElement* materiaElem = doc.NewElement("Materia");

    string idStr = "MAT-" + to_string(this->id);
    materiaElem->SetAttribute("id", idStr.c_str());

    XMLElement* nomeElem = doc.NewElement("Nome");
    nomeElem->SetText(nome.c_str());
    materiaElem->InsertEndChild(nomeElem);

    XMLElement* labElem = doc.NewElement("Laboratorio");
    labElem->SetText(laboratorio ? 1 : 0);
    materiaElem->InsertEndChild(labElem);

    // Docenti
    if (docentiIds.size() > 0) {
        XMLElement* docentiElem = doc.NewElement("Docenti");

        for (size_t i = 0; i < docentiIds.size(); ++i) {
            XMLElement* docenteRef = doc.NewElement("DocenteRef");
            string dIdStr = "DOC-" + to_string(docentiIds[i]);
            docenteRef->SetAttribute("id", dIdStr.c_str());
            docentiElem->InsertEndChild(docenteRef);
        }

        materiaElem->InsertEndChild(docentiElem);
    }

    return materiaElem;
}

// ============================================
// CSV - SCRITTURA (toCSV)
// Formato:
// id,nome,laboratorio,docentiIds(;)
// ============================================
string Materia::toCSV() const {
    char splitter = ',';

    string CSV = to_string(this->id) + splitter;
    CSV += this->nome + splitter;
    CSV += (laboratorio ? "1" : "0");
    CSV += splitter;

    // Docenti (separatore interno: punto e virgola)
    string docentiStr = "";
    for (size_t i = 0; i < docentiIds.size(); ++i) {
        if (i > 0) {
            docentiStr += ";";
        }
        docentiStr += to_string(docentiIds[i]);
    }
    CSV += docentiStr;

    return CSV;
}

// ============================================
// CSV - LETTURA (fromCSV)
// ============================================
void Materia::fromCSV(const string& riga) {
    stringstream ss(riga);
    string temp;

    docentiIds.clear();
    docenti.clear();

    // ID
    if (getline(ss, temp, ',')) {
        this->id = estraiIdNumerico(temp);
    }

    // Nome
    if (getline(ss, temp, ',')) {
        this->nome = temp;
    }

    // Laboratorio
    if (getline(ss, temp, ',')) {
        if (temp == "true" || temp == "TRUE" || temp == "True") {
            this->laboratorio = true;
        }
        else if (temp == "false" || temp == "FALSE" || temp == "False") {
            this->laboratorio = false;
        }
        else {
            this->laboratorio = (atoi(temp.c_str()) != 0);
        }
    }
    else {
        this->laboratorio = false;
    }

    // DocentiIds (separati da ';')
    if (getline(ss, temp, ',')) {
        if (!temp.empty()) {
            stringstream ssD(temp);
            string dIdStr;

            while (getline(ssD, dIdStr, ';')) {
                if (!dIdStr.empty()) {
                    int dId = atoi(dIdStr.c_str());
                    this->docentiIds.push_back(dId);
                }
            }
        }
    }
}

// ============================================
// XML - SCRITTURA STRING (toXML stringa)
// ============================================
string Materia::toXML() const {
    string xml;

    xml += "<Materia id=\"";
    xml += "MAT-" + to_string(id);
    xml += "\">";

    xml += "<Nome>";
    xml += escapeXML(nome);
    xml += "</Nome>";

    xml += "<Laboratorio>";
    xml += (laboratorio ? "1" : "0");
    xml += "</Laboratorio>";

    // Docenti
    if (docentiIds.size() > 0) {
        xml += "<Docenti>";

        for (size_t i = 0; i < docentiIds.size(); ++i) {
            xml += "<DocenteRef id=\"DOC-";
            xml += to_string(docentiIds[i]);
            xml += "\"/>";
        }

        xml += "</Docenti>";
    }

    xml += "</Materia>";
    return xml;
}

// ============================================
// RAPPRESENTAZIONE TESTUALE (toString)
// ============================================
string Materia::toString() const {
    string s;

    s += "Materia ID: ";
    s += "MAT-" + to_string(id);
    s += "\n";

    s += "  Nome: ";
    s += nome;
    s += "\n";

    s += "  Laboratorio: ";
    s += (laboratorio ? "SI" : "NO");
    s += "\n";

    s += "  Docenti IDs: ";
    if (docentiIds.size() == 0) {
        s += "(nessuno)";
    }
    else {
        for (size_t i = 0; i < docentiIds.size(); ++i) {
            if (i > 0) {
                s += ", ";
            }
            s += "DOC-" + to_string(docentiIds[i]);
        }
    }
    s += "\n";

    return s;
}
