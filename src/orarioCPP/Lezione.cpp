#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "libUtils.h"
#include "Lezione.h"
#include "Classe.h"
#include "Aula.h"
#include "Materia.h"
#include "Docente.h"

using namespace std;

// ============================================
// COSTRUTTORE
// ============================================
Lezione::Lezione() {
    id = -1;
    giorno = LUNEDI;
    ora = 1;

    classeId = -1;
    aulaId = -1;
    materiaId = -1;

    classe = nullptr;
    aula = nullptr;
    materia = nullptr;
    // docentiIds e docenti vuoti per default
}

// ============================================
// CONVERSIONE GiornoSettimana -> string
// ============================================
const char* Lezione::giornoToString(GiornoSettimana g) {
    if (g == LUNEDI) return "LUNEDI";
    if (g == MARTEDI) return "MARTEDI";
    if (g == MERCOLEDI) return "MERCOLEDI";
    if (g == GIOVEDI) return "GIOVEDI";
    if (g == VENERDI) return "VENERDI";
    if (g == SABATO) return "SABATO";
    if (g == DOMENICA) return "DOMENICA";
    return "LUNEDI";
}

// ============================================
// CONVERSIONE string -> GiornoSettimana
// Accetta "LUNEDI" ecc. oppure "1".."7"
// ============================================
GiornoSettimana Lezione::stringToGiorno(const string& s) {
    if (s == "LUNEDI" || s == "lunedi" || s == "Lunedì" || s == "LUNEDÌ") return LUNEDI;
    if (s == "MARTEDI" || s == "martedi" || s == "Martedì" || s == "MARTEDÌ") return MARTEDI;
    if (s == "MERCOLEDI" || s == "mercoledi" || s == "Mercoledì" || s == "MERCOLEDÌ") return MERCOLEDI;
    if (s == "GIOVEDI" || s == "giovedi" || s == "Giovedì" || s == "GIOVEDÌ") return GIOVEDI;
    if (s == "VENERDI" || s == "venerdi" || s == "Venerdì" || s == "VENERDÌ") return VENERDI;
    if (s == "SABATO" || s == "sabato") return SABATO;
    if (s == "DOMENICA" || s == "domenica") return DOMENICA;

    // Se è numerico
    int n = atoi(s.c_str());
    if (n >= 1 && n <= 7) {
        return (GiornoSettimana)n;
    }

    return LUNEDI;
}

// ============================================
// RESOLVE - collega ID -> puntatori
// - classeId  -> classe
// - aulaId    -> aula
// - materiaId -> materia
// - docentiIds -> docenti
// ============================================
void Lezione::resolve(const vector<Classe*>& tutteClassi,
    const vector<Aula*>& tutteAule,
    const vector<Materia*>& tutteMaterie,
    const vector<Docente*>& tuttiDocenti) {
    // reset puntatori
    classe = nullptr;
    aula = nullptr;
    materia = nullptr;
    docenti.clear();

    // Classe
    for (size_t i = 0; i < tutteClassi.size(); ++i) {
        if (tutteClassi[i] != nullptr && tutteClassi[i]->getId() == classeId) {
            classe = tutteClassi[i];
        }
    }

    // Aula
    for (size_t i = 0; i < tutteAule.size(); ++i) {
        if (tutteAule[i] != nullptr && tutteAule[i]->getId() == aulaId) {
            aula = tutteAule[i];
        }
    }

    // Materia
    for (size_t i = 0; i < tutteMaterie.size(); ++i) {
        if (tutteMaterie[i] != nullptr && tutteMaterie[i]->getId() == materiaId) {
            materia = tutteMaterie[i];
        }
    }

    // Docenti
    for (size_t i = 0; i < docentiIds.size(); ++i) {
        int dId = docentiIds[i];

        for (size_t j = 0; j < tuttiDocenti.size(); ++j) {
            if (tuttiDocenti[j] != nullptr && tuttiDocenti[j]->getId() == dId) {
                docenti.push_back(tuttiDocenti[j]);
            }
        }
    }
}

// ============================================
// XML - LETTURA (fromXML)
// Atteso (coerente con gli altri oggetti):
// <Lezione id="LEZ-1">
//   <Giorno>LUNEDI</Giorno>
//   <Ora>1</Ora>
//   <ClasseRef id="CLA-3"/>
//   <AulaRef id="AUL-2"/>
//   <MateriaRef id="MAT-5"/>
//   <Docenti>
//     <DocenteRef id="DOC-7"/>
//   </Docenti>
// </Lezione>
// ============================================
Lezione* Lezione::fromXML(XMLElement* lezioneElem) {
    if (lezioneElem == nullptr || string(lezioneElem->Name()) != "Lezione") {
        return nullptr;
    }

    Lezione* l = new Lezione();

    // id come attributo
    const char* idAttr = lezioneElem->Attribute("id");
    if (idAttr) {
        l->id = estraiIdNumerico(idAttr);
    }

    // Giorno
    XMLElement* giornoElem = lezioneElem->FirstChildElement("Giorno");
    if (giornoElem && giornoElem->GetText()) {
        l->giorno = Lezione::stringToGiorno(giornoElem->GetText());
    }

    // Ora
    XMLElement* oraElem = lezioneElem->FirstChildElement("Ora");
    if (oraElem && oraElem->GetText()) {
        l->ora = atoi(oraElem->GetText());
    }

    // ClasseRef
    XMLElement* classeRef = lezioneElem->FirstChildElement("ClasseRef");
    if (classeRef) {
        const char* cAttr = classeRef->Attribute("id");
        if (cAttr) {
            l->classeId = estraiIdNumerico(cAttr);
        }
    }

    // AulaRef
    XMLElement* aulaRef = lezioneElem->FirstChildElement("AulaRef");
    if (aulaRef) {
        const char* aAttr = aulaRef->Attribute("id");
        if (aAttr) {
            l->aulaId = estraiIdNumerico(aAttr);
        }
    }

    // MateriaRef
    XMLElement* materiaRef = lezioneElem->FirstChildElement("MateriaRef");
    if (materiaRef) {
        const char* mAttr = materiaRef->Attribute("id");
        if (mAttr) {
            l->materiaId = estraiIdNumerico(mAttr);
        }
    }

    // Docenti
    XMLElement* docentiElem = lezioneElem->FirstChildElement("Docenti");
    if (docentiElem) {
        XMLElement* docenteRef = docentiElem->FirstChildElement("DocenteRef");
        while (docenteRef != nullptr) {
            const char* dAttr = docenteRef->Attribute("id");
            if (dAttr) {
                int dId = estraiIdNumerico(dAttr);
                l->docentiIds.push_back(dId);
            }
            docenteRef = docenteRef->NextSiblingElement("DocenteRef");
        }
    }

    // puntatori runtime risolti dopo
    l->classe = nullptr;
    l->aula = nullptr;
    l->materia = nullptr;
    l->docenti.clear();

    return l;
}

// ============================================
// XML - SCRITTURA (toXML con TinyXML2)
// ============================================
XMLElement* Lezione::toXML(XMLDocument& doc) const {
    XMLElement* lezioneElem = doc.NewElement("Lezione");

    // id attribute
    string idStr = "LEZ-" + to_string(id);
    lezioneElem->SetAttribute("id", idStr.c_str());

    XMLElement* giornoElem = doc.NewElement("Giorno");
    giornoElem->SetText(Lezione::giornoToString(giorno));
    lezioneElem->InsertEndChild(giornoElem);

    XMLElement* oraElem = doc.NewElement("Ora");
    oraElem->SetText(ora);
    lezioneElem->InsertEndChild(oraElem);

    // ClasseRef
    if (classeId >= 0) {
        XMLElement* classeRef = doc.NewElement("ClasseRef");
        string cIdStr = "CLA-" + to_string(classeId);
        classeRef->SetAttribute("id", cIdStr.c_str());
        lezioneElem->InsertEndChild(classeRef);
    }

    // AulaRef
    if (aulaId >= 0) {
        XMLElement* aulaRef = doc.NewElement("AulaRef");
        string aIdStr = "AUL-" + to_string(aulaId);
        aulaRef->SetAttribute("id", aIdStr.c_str());
        lezioneElem->InsertEndChild(aulaRef);
    }

    // MateriaRef
    if (materiaId >= 0) {
        XMLElement* materiaRef = doc.NewElement("MateriaRef");
        string mIdStr = "MAT-" + to_string(materiaId);
        materiaRef->SetAttribute("id", mIdStr.c_str());
        lezioneElem->InsertEndChild(materiaRef);
    }

    // Docenti
    if (docentiIds.size() > 0) {
        XMLElement* docentiElem = doc.NewElement("Docenti");

        for (size_t i = 0; i < docentiIds.size(); ++i) {
            XMLElement* docenteRef = doc.NewElement("DocenteRef");
            string dIdStr = "DOC-" + to_string(docentiIds[i]);
            docenteRef->SetAttribute("id", dIdStr.c_str());
            docentiElem->InsertEndChild(docenteRef);
        }

        lezioneElem->InsertEndChild(docentiElem);
    }

    return lezioneElem;
}

// ============================================
// CSV - SCRITTURA (toCSV)
// Formato:
// id,giorno,ora,classeId,aulaId,materiaId,docentiIds(;)
// ============================================
string Lezione::toCSV() const {
    char splitter = ',';

    string CSV = to_string(id);
    CSV += splitter;

    CSV += Lezione::giornoToString(giorno);
    CSV += splitter;

    CSV += to_string(ora);
    CSV += splitter;

    CSV += to_string(classeId);
    CSV += splitter;

    CSV += to_string(aulaId);
    CSV += splitter;

    CSV += to_string(materiaId);
    CSV += splitter;

    // docentiIds separati da ';'
    string docStr = "";
    for (size_t i = 0; i < docentiIds.size(); ++i) {
        if (i > 0) {
            docStr += ";";
        }
        docStr += to_string(docentiIds[i]);
    }
    CSV += docStr;

    return CSV;
}

// ============================================
// CSV - LETTURA (fromCSV)
// ============================================
void Lezione::fromCSV(const string& riga) {
    stringstream ss(riga);
    string temp;

    docentiIds.clear();
    docenti.clear();

    // ID
    if (getline(ss, temp, ',')) {
        id = estraiIdNumerico(temp);
    }

    // Giorno
    if (getline(ss, temp, ',')) {
        giorno = Lezione::stringToGiorno(temp);
    }

    // Ora
    if (getline(ss, temp, ',')) {
        ora = atoi(temp.c_str());
    }

    // ClasseId
    if (getline(ss, temp, ',')) {
        classeId = atoi(temp.c_str());
    }

    // AulaId
    if (getline(ss, temp, ',')) {
        aulaId = atoi(temp.c_str());
    }

    // MateriaId
    if (getline(ss, temp, ',')) {
        materiaId = atoi(temp.c_str());
    }

    // DocentiIds (separati da ';')
    if (getline(ss, temp, ',')) {
        if (!temp.empty()) {
            stringstream ssD(temp);
            string idStr;

            while (getline(ssD, idStr, ';')) {
                if (!idStr.empty()) {
                    int dId = atoi(idStr.c_str());
                    docentiIds.push_back(dId);
                }
            }
        }
    }

    // puntatori runtime risolti dopo
    classe = nullptr;
    aula = nullptr;
    materia = nullptr;
}

// ============================================
// XML - SCRITTURA STRING (toXML stringa)
// ============================================
string Lezione::toXML() const {
    string xml;

    xml += "<Lezione id=\"";
    xml += "LEZ-" + to_string(id);
    xml += "\">";

    xml += "<Giorno>";
    xml += escapeXML(Lezione::giornoToString(giorno));
    xml += "</Giorno>";

    xml += "<Ora>";
    xml += to_string(ora);
    xml += "</Ora>";

    if (classeId >= 0) {
        xml += "<ClasseRef id=\"CLA-";
        xml += to_string(classeId);
        xml += "\"/>";
    }

    if (aulaId >= 0) {
        xml += "<AulaRef id=\"AUL-";
        xml += to_string(aulaId);
        xml += "\"/>";
    }

    if (materiaId >= 0) {
        xml += "<MateriaRef id=\"MAT-";
        xml += to_string(materiaId);
        xml += "\"/>";
    }

    if (docentiIds.size() > 0) {
        xml += "<Docenti>";

        for (size_t i = 0; i < docentiIds.size(); ++i) {
            xml += "<DocenteRef id=\"DOC-";
            xml += to_string(docentiIds[i]);
            xml += "\"/>";
        }

        xml += "</Docenti>";
    }

    xml += "</Lezione>";
    return xml;
}

// ============================================
// RAPPRESENTAZIONE TESTUALE (toString)
// ============================================
string Lezione::toString() const {
    string s;

    s += "Lezione ID: ";
    s += "LEZ-" + to_string(id);
    s += "\n";

    s += "  Giorno: ";
    s += Lezione::giornoToString(giorno);
    s += "\n";

    s += "  Ora: ";
    s += to_string(ora);
    s += "\n";

    s += "  ClasseId: ";
    if (classeId >= 0) s += "CLA-" + to_string(classeId);
    else s += "(non assegnata)";
    s += "\n";

    s += "  AulaId: ";
    if (aulaId >= 0) s += "AUL-" + to_string(aulaId);
    else s += "(non assegnata)";
    s += "\n";

    s += "  MateriaId: ";
    if (materiaId >= 0) s += "MAT-" + to_string(materiaId);
    else s += "(non assegnata)";
    s += "\n";

    s += "  DocentiIds: ";
    if (docentiIds.size() == 0) {
        s += "(nessuno)";
    }
    else {
        for (size_t i = 0; i < docentiIds.size(); ++i) {
            if (i > 0) s += ", ";
            s += "DOC-" + to_string(docentiIds[i]);
        }
    }
    s += "\n";

    s += "  Puntatori (runtime):\n";
    s += "    classe: ";
    s += (classe != nullptr ? "OK" : "NULL");
    s += "\n";
    s += "    aula: ";
    s += (aula != nullptr ? "OK" : "NULL");
    s += "\n";
    s += "    materia: ";
    s += (materia != nullptr ? "OK" : "NULL");
    s += "\n";
    s += "    docenti: ";
    s += to_string(docenti.size());
    s += "\n";

    return s;
}
