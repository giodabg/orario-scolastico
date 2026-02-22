#include "Classe.h"
#include "Studente.h"
#include "libUtils.h"
#include <sstream>
#include <string>

using namespace std;

// ============================================
// COSTRUTTORE
// ============================================
Classe::Classe() {
    id = -1;
    anno = 0;
    sezione = ' ';
    indirizzo = "";
}

// ============================================
// GETTER PUNTATORI
// ============================================
Studente* Classe::getStudente(size_t index) const {
    if (index < studenti.size()) {
        return studenti[index];
    }
    return nullptr;
}

// ============================================
// METODO DI RISOLUZIONE
// ============================================
void Classe::resolveStudenti(const vector<Studente*>& tuttiStudenti) {
    studenti.clear();

    for (size_t i = 0; i < studentiIds.size(); ++i) {
        int studenteId = studentiIds[i];

        // Cerca lo studente con questo ID
        for (size_t j = 0; j < tuttiStudenti.size(); ++j) {
            if (tuttiStudenti[j] != nullptr && tuttiStudenti[j]->getId() == studenteId) {
                studenti.push_back(tuttiStudenti[j]);
            }
        }
    }
}

// ============================================
// XML - LETTURA (fromXML)
// ============================================
Classe* Classe::fromXML(XMLElement* classeElem) {
    if (classeElem == nullptr || string(classeElem->Name()) != "Classe") {
        return nullptr;
    }

    Classe* classe = new Classe();

    // Leggi id come attributo
    const char* idAttr = classeElem->Attribute("id");
    if (idAttr) {
        classe->setId(estraiIdNumerico(idAttr));
    }

    // Leggi Anno
    XMLElement* annoElem = classeElem->FirstChildElement("Anno");
    if (annoElem && annoElem->GetText()) {
        classe->anno = atoi(annoElem->GetText());
    }

    // Leggi Sezione
    XMLElement* sezioneElem = classeElem->FirstChildElement("Sezione");
    if (sezioneElem && sezioneElem->GetText()) {
        const char* sezText = sezioneElem->GetText();
        if (sezText && sezText[0] != '\0') {
            classe->sezione = sezText[0];
        }
    }

    // Leggi Indirizzo
    XMLElement* indirizzoElem = classeElem->FirstChildElement("Indirizzo");
    if (indirizzoElem && indirizzoElem->GetText()) {
        classe->indirizzo = indirizzoElem->GetText();
    }

    // Leggi Studenti
    XMLElement* studentiElem = classeElem->FirstChildElement("Studenti");
    if (studentiElem) {
        XMLElement* studenteRef = studentiElem->FirstChildElement("StudenteRef");
        while (studenteRef != nullptr) {
            const char* studenteIdAttr = studenteRef->Attribute("id");
            if (studenteIdAttr) {
                int stuId = estraiIdNumerico(studenteIdAttr);
                classe->addStudenteId(stuId);
            }
            studenteRef = studenteRef->NextSiblingElement("StudenteRef");
        }
    }

    return classe;
}

// ============================================
// XML - SCRITTURA (toXML con TinyXML2)
// ============================================
XMLElement* Classe::toXML(XMLDocument& doc) const {
    XMLElement* classeElem = doc.NewElement("Classe");
    string idStr = "CLA-" + to_string(this->id);
    classeElem->SetAttribute("id", idStr.c_str());

    XMLElement* annoElem = doc.NewElement("Anno");
    annoElem->SetText(anno);
    classeElem->InsertEndChild(annoElem);

    XMLElement* sezioneElem = doc.NewElement("Sezione");
    string sezStr(1, sezione);
    sezioneElem->SetText(sezStr.c_str());
    classeElem->InsertEndChild(sezioneElem);

    XMLElement* indirizzoElem = doc.NewElement("Indirizzo");
    indirizzoElem->SetText(indirizzo.c_str());
    classeElem->InsertEndChild(indirizzoElem);

    // Studenti
    if (studentiIds.size() > 0) {
        XMLElement* studentiElem = doc.NewElement("Studenti");

        for (size_t i = 0; i < studentiIds.size(); ++i) {
            XMLElement* studenteRef = doc.NewElement("StudenteRef");
            string stuIdStr = "STU-" + to_string(studentiIds[i]);
            studenteRef->SetAttribute("id", stuIdStr.c_str());
            studentiElem->InsertEndChild(studenteRef);
        }

        classeElem->InsertEndChild(studentiElem);
    }

    return classeElem;
}

// ============================================
// CSV - SCRITTURA (toCSV)
// ============================================
string Classe::toCSV() const {
    char splitter = ',';
    string CSV = to_string(this->id) + splitter;
    CSV += to_string(this->anno) + splitter;
    CSV += this->sezione;
    CSV += splitter;
    CSV += this->indirizzo + splitter;

    // Studenti (separatore interno: punto e virgola)
    string studentiStr = "";
    for (size_t i = 0; i < studentiIds.size(); ++i) {
        if (i > 0) {
            studentiStr += ";";
        }
        studentiStr += to_string(studentiIds[i]);
    }
    CSV += studentiStr;

    return CSV;
}

// ============================================
// CSV - LETTURA (fromCSV)
// ============================================
void Classe::fromCSV(const string& riga) {
    stringstream ss(riga);
    string tempID;

    // Leggi ID
    if (getline(ss, tempID, ',')) {
        this->id = estraiIdNumerico(tempID);
    }

    // Leggi Anno
    string annoStr;
    if (getline(ss, annoStr, ',')) {
        this->anno = atoi(annoStr.c_str());
    }

    // Leggi Sezione
    string sezioneStr;
    if (getline(ss, sezioneStr, ',')) {
        if (!sezioneStr.empty()) {
            this->sezione = sezioneStr[0];
        }
    }

    // Leggi Indirizzo
    getline(ss, this->indirizzo, ',');

    // Leggi Studenti (separati da punto e virgola)
    string studentiStr;
    if (getline(ss, studentiStr, ',')) {
        if (!studentiStr.empty()) {
            stringstream ssStu(studentiStr);
            string stuIdStr;

            while (getline(ssStu, stuIdStr, ';')) {
                if (!stuIdStr.empty()) {
                    int stuId = atoi(stuIdStr.c_str());
                    this->studentiIds.push_back(stuId);
                }
            }
        }
    }
}

// ============================================
// XML - SCRITTURA STRING (toXML stringa)
// ============================================
string Classe::toXML() const {
    string xml;

    xml += "<Classe id=\"";
    xml += "CLA-" + to_string(id);
    xml += "\">";

    xml += "<Anno>";
    xml += to_string(anno);
    xml += "</Anno>";

    xml += "<Sezione>";
    xml += sezione;
    xml += "</Sezione>";

    xml += "<Indirizzo>";
    xml += escapeXML(indirizzo);
    xml += "</Indirizzo>";

    // Studenti
    if (studentiIds.size() > 0) {
        xml += "<Studenti>";

        for (size_t i = 0; i < studentiIds.size(); ++i) {
            xml += "<StudenteRef id=\"STU-";
            xml += to_string(studentiIds[i]);
            xml += "\"/>";
        }

        xml += "</Studenti>";
    }

    xml += "</Classe>";
    return xml;
}

// ============================================
// RAPPRESENTAZIONE TESTUALE (toString)
// ============================================
string Classe::toString() const {
    string s;

    s += "Classe ID: ";
    s += "CLA-" + to_string(id);
    s += "\n";

    s += "  Anno: ";
    s += to_string(anno);
    s += "\n";

    s += "  Sezione: ";
    s += sezione;
    s += "\n";

    s += "  Indirizzo: ";
    s += indirizzo;
    s += "\n";

    s += "  Numero studenti: ";
    s += to_string(studentiIds.size());
    s += "\n";

    s += "  Studenti IDs: ";
    if (studentiIds.size() == 0) {
        s += "(nessuno)";
    }
    else {
        for (size_t i = 0; i < studentiIds.size(); ++i) {
            if (i > 0) {
                s += ", ";
            }
            s += "STU-" + to_string(studentiIds[i]);
        }
    }
    s += "\n";

    return s;
}