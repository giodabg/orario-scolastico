#include "Aula.h"
#include "Materia.h"
#include "libUtils.h"
#include <sstream>
#include <string>

using namespace std;

// ============================================
// COSTRUTTORE
// ============================================
Aula::Aula() {
    id = -1;
    nome = "";
    posti = 0;
    tipo = AULA_STANDARD;
}

// ============================================
// METODO DI RISOLUZIONE
// ============================================
void Aula::resolveMaterie(const vector<Materia*>& tutteMaterie) {
    materieAmmesse.clear();

    for (size_t i = 0; i < materieAmmesseIds.size(); ++i) {
        int materiaId = materieAmmesseIds[i];

        // Cerca la materia con questo ID
        for (size_t j = 0; j < tutteMaterie.size(); ++j) {
            if (tutteMaterie[j] != nullptr && tutteMaterie[j]->getId() == materiaId) {
                materieAmmesse.push_back(tutteMaterie[j]);
            }
        }
    }
}

// ============================================
// XML - LETTURA (fromXML)
// ============================================
Aula* Aula::fromXML(XMLElement* aulaElem) {
    if (aulaElem == nullptr || string(aulaElem->Name()) != "Aula") {
        return nullptr;
    }

    Aula* aula = new Aula();

    // Leggi id come attributo
    const char* idAttr = aulaElem->Attribute("id");
    if (idAttr) {
        aula->setId(estraiIdNumerico(idAttr));
    }

    // Leggi Nome
    XMLElement* nomeElem = aulaElem->FirstChildElement("Nome");
    if (nomeElem && nomeElem->GetText()) {
        aula->nome = nomeElem->GetText();
    }

    // Leggi Posti
    XMLElement* postiElem = aulaElem->FirstChildElement("Posti");
    if (postiElem && postiElem->GetText()) {
        aula->posti = atoi(postiElem->GetText());
    }

    // Leggi Tipo
    XMLElement* tipoElem = aulaElem->FirstChildElement("Tipo");
    if (tipoElem && tipoElem->GetText()) {
        string tipoStr = tipoElem->GetText();

        if (tipoStr == "AULA_STANDARD") {
            aula->tipo = AULA_STANDARD;
        }
        else {
            if (tipoStr == "AULA_MULTIFUNZIONALE") {
                aula->tipo = AULA_MULTIFUNZIONALE;
            }
            else {
                if (tipoStr == "AULA_DEDICATA") {
                    aula->tipo = AULA_DEDICATA;
                }
            }
        }
    }

    // Leggi MaterieAmmesse
    XMLElement* materieElem = aulaElem->FirstChildElement("MaterieAmmesse");
    if (materieElem) {
        XMLElement* materiaRef = materieElem->FirstChildElement("MateriaRef");
        while (materiaRef != nullptr) {
            const char* materiaIdAttr = materiaRef->Attribute("id");
            if (materiaIdAttr) {
                int matId = estraiIdNumerico(materiaIdAttr);
                aula->addMateriaAmmessaId(matId);
            }
            materiaRef = materiaRef->NextSiblingElement("MateriaRef");
        }
    }

    return aula;
}

// ============================================
// XML - SCRITTURA (toXML con TinyXML2)
// ============================================
XMLElement* Aula::toXML(XMLDocument& doc) const {
    XMLElement* aulaElem = doc.NewElement("Aula");
    string idStr = "AUL-" + to_string(this->id);
    aulaElem->SetAttribute("id", idStr.c_str());

    XMLElement* nomeElem = doc.NewElement("Nome");
    nomeElem->SetText(nome.c_str());
    aulaElem->InsertEndChild(nomeElem);

    XMLElement* postiElem = doc.NewElement("Posti");
    postiElem->SetText(posti);
    aulaElem->InsertEndChild(postiElem);

    XMLElement* tipoElem = doc.NewElement("Tipo");
    tipoElem->SetText(tipoAulaToString(tipo));
    aulaElem->InsertEndChild(tipoElem);

    // MaterieAmmesse
    if (materieAmmesseIds.size() > 0) {
        XMLElement* materieElem = doc.NewElement("MaterieAmmesse");

        for (size_t i = 0; i < materieAmmesseIds.size(); ++i) {
            XMLElement* materiaRef = doc.NewElement("MateriaRef");
            string matIdStr = "MAT-" + to_string(materieAmmesseIds[i]);
            materiaRef->SetAttribute("id", matIdStr.c_str());
            materieElem->InsertEndChild(materiaRef);
        }

        aulaElem->InsertEndChild(materieElem);
    }

    return aulaElem;
}

// ============================================
// CONVERSIONE TipoAula → String
// ============================================
const char* Aula::tipoAulaToString(TipoAula t) {
    if (t == AULA_STANDARD) {
        return "AULA_STANDARD";
    }
    if (t == AULA_MULTIFUNZIONALE) {
        return "AULA_MULTIFUNZIONALE";
    }
    if (t == AULA_DEDICATA) {
        return "AULA_DEDICATA";
    }
    return "UNKNOWN";
}

// ============================================
// CSV - SCRITTURA (toCSV)
// ============================================
string Aula::toCSV() const {
    char splitter = ',';
    string CSV = to_string(this->id) + splitter;
    CSV += this->nome + splitter;
    CSV += to_string(this->posti) + splitter;
    CSV += string(tipoAulaToString(this->tipo)) + splitter;

    // Materie ammesse (separatore interno: punto e virgola)
    string materieStr = "";
    for (size_t i = 0; i < materieAmmesseIds.size(); ++i) {
        if (i > 0) {
            materieStr += ";";
        }
        materieStr += to_string(materieAmmesseIds[i]);
    }
    CSV += materieStr;

    return CSV;
}

// ============================================
// CSV - LETTURA (fromCSV)
// ============================================
void Aula::fromCSV(const string& riga) {
    stringstream ss(riga);
    string tempID;

    // Leggi ID
    if (getline(ss, tempID, ',')) {
        this->id = estraiIdNumerico(tempID);
    }

    // Leggi Nome
    getline(ss, this->nome, ',');

    // Leggi Posti
    string postiStr;
    if (getline(ss, postiStr, ',')) {
        this->posti = atoi(postiStr.c_str());
    }

    // Leggi Tipo
    string tipoStr;
    if (getline(ss, tipoStr, ',')) {
        if (tipoStr == "AULA_STANDARD") {
            this->tipo = AULA_STANDARD;
        }
        else {
            if (tipoStr == "AULA_MULTIFUNZIONALE") {
                this->tipo = AULA_MULTIFUNZIONALE;
            }
            else {
                if (tipoStr == "AULA_DEDICATA") {
                    this->tipo = AULA_DEDICATA;
                }
                else {
                    this->tipo = AULA_STANDARD;
                }
            }
        }
    }

    // Leggi MaterieAmmesse (separate da punto e virgola)
    string materieStr;
    if (getline(ss, materieStr, ',')) {
        if (!materieStr.empty()) {
            stringstream ssMat(materieStr);
            string matIdStr;

            while (getline(ssMat, matIdStr, ';')) {
                if (!matIdStr.empty()) {
                    int matId = atoi(matIdStr.c_str());
                    this->materieAmmesseIds.push_back(matId);
                }
            }
        }
    }
}

// ============================================
// XML - SCRITTURA STRING (toXML stringa)
// ============================================
string Aula::toXML() const {
    string xml;

    xml += "<Aula id=\"";
    xml += "AUL-" + to_string(id);
    xml += "\">";

    xml += "<Nome>";
    xml += escapeXML(nome);
    xml += "</Nome>";

    xml += "<Posti>";
    xml += to_string(posti);
    xml += "</Posti>";

    xml += "<Tipo>";
    xml += tipoAulaToString(tipo);
    xml += "</Tipo>";

    // MaterieAmmesse
    if (materieAmmesseIds.size() > 0) {
        xml += "<MaterieAmmesse>";

        for (size_t i = 0; i < materieAmmesseIds.size(); ++i) {
            xml += "<MateriaRef id=\"MAT-";
            xml += to_string(materieAmmesseIds[i]);
            xml += "\"/>";
        }

        xml += "</MaterieAmmesse>";
    }

    xml += "</Aula>";
    return xml;
}

// ============================================
// RAPPRESENTAZIONE TESTUALE (toString)
// ============================================
string Aula::toString() const {
    string s;

    s += "Aula ID: ";
    s += "AUL-" + to_string(id);
    s += "\n";

    s += "  Nome: ";
    s += nome;
    s += "\n";

    s += "  Posti: ";
    s += to_string(posti);
    s += "\n";

    s += "  Tipo: ";
    s += tipoAulaToString(tipo);
    s += "\n";

    s += "  Materie ammesse IDs: ";
    if (materieAmmesseIds.size() == 0) {
        s += "(nessuna)";
    }
    else {
        for (size_t i = 0; i < materieAmmesseIds.size(); ++i) {
            if (i > 0) {
                s += ", ";
            }
            s += "MAT-" + to_string(materieAmmesseIds[i]);
        }
    }
    s += "\n";

    return s;
}