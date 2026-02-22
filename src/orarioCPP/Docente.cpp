#include "Docente.h"
#include "Classe.h"
#include "Materia.h"
#include "libUtils.h"
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

// ============================================
// COSTRUTTORE
// ============================================
Docente::Docente() : Utente() {
    // vettori vuoti per default
    // (classiIds, materieIds, classi, materie)
}

// ============================================
// METODI DI RISOLUZIONE
// ============================================
void Docente::resolveClassi(const vector<Classe*>& tutteClassi) {
    classi.clear();

    for (size_t i = 0; i < classiIds.size(); ++i) {
        int classeId = classiIds[i];

        for (size_t j = 0; j < tutteClassi.size(); ++j) {
            if (tutteClassi[j] != nullptr && tutteClassi[j]->getId() == classeId) {
                classi.push_back(tutteClassi[j]);
            }
        }
    }
}

void Docente::resolveMaterie(const vector<Materia*>& tutteMaterie) {
    materie.clear();

    for (size_t i = 0; i < materieIds.size(); ++i) {
        int materiaId = materieIds[i];

        for (size_t j = 0; j < tutteMaterie.size(); ++j) {
            if (tutteMaterie[j] != nullptr && tutteMaterie[j]->getId() == materiaId) {
                materie.push_back(tutteMaterie[j]);
            }
        }
    }
}

// ============================================
// TIPO UTENTE
// ============================================
string Docente::getTipoUtente() const {
    return "DOCENTE";
}

// ============================================
// XML - LETTURA (fromXML)
// Atteso:
// <Docente id="DOC-1">
//   <Username>...</Username>
//   <Password>...</Password>
//   <Nome>...</Nome>
//   <Cognome>...</Cognome>
//   <Classi>
//     <ClasseRef id="CLA-3"/>
//   </Classi>
//   <Materie>
//     <MateriaRef id="MAT-2"/>
//   </Materie>
// </Docente>
// ============================================
Docente* Docente::fromXML(XMLElement* docenteElem) {
    if (docenteElem == nullptr || string(docenteElem->Name()) != "Docente") {
        return nullptr;
    }

    Docente* d = new Docente();

    // id come attributo
    const char* idAttr = docenteElem->Attribute("id");
    if (idAttr) {
        d->id = estraiIdNumerico(idAttr);
    }

    // Username
    XMLElement* userElem = docenteElem->FirstChildElement("Username");
    if (userElem && userElem->GetText()) {
        d->username = userElem->GetText();
    }

    // Password
    XMLElement* passElem = docenteElem->FirstChildElement("Password");
    if (passElem && passElem->GetText()) {
        d->password = passElem->GetText();
    }

    // Nome
    XMLElement* nomeElem = docenteElem->FirstChildElement("Nome");
    if (nomeElem && nomeElem->GetText()) {
        d->nome = nomeElem->GetText();
    }

    // Cognome
    XMLElement* cognomeElem = docenteElem->FirstChildElement("Cognome");
    if (cognomeElem && cognomeElem->GetText()) {
        d->cognome = cognomeElem->GetText();
    }

    // Classi
    XMLElement* classiElem = docenteElem->FirstChildElement("Classi");
    if (classiElem) {
        XMLElement* classeRef = classiElem->FirstChildElement("ClasseRef");
        while (classeRef != nullptr) {
            const char* cIdAttr = classeRef->Attribute("id");
            if (cIdAttr) {
                int cId = estraiIdNumerico(cIdAttr);
                d->classiIds.push_back(cId);
            }
            classeRef = classeRef->NextSiblingElement("ClasseRef");
        }
    }

    // Materie
    XMLElement* materieElem = docenteElem->FirstChildElement("Materie");
    if (materieElem) {
        XMLElement* materiaRef = materieElem->FirstChildElement("MateriaRef");
        while (materiaRef != nullptr) {
            const char* mIdAttr = materiaRef->Attribute("id");
            if (mIdAttr) {
                int mId = estraiIdNumerico(mIdAttr);
                d->materieIds.push_back(mId);
            }
            materiaRef = materiaRef->NextSiblingElement("MateriaRef");
        }
    }

    // puntatori runtime risolti dopo
    d->classi.clear();
    d->materie.clear();

    return d;
}

// ============================================
// XML - SCRITTURA (toXML con TinyXML2)
// ============================================
XMLElement* Docente::toXML(XMLDocument& doc) const {
    XMLElement* docenteElem = doc.NewElement("Docente");

    // id attribute
    string idStr = "DOC-" + to_string(this->id);
    docenteElem->SetAttribute("id", idStr.c_str());

    XMLElement* userElem = doc.NewElement("Username");
    userElem->SetText(username.c_str());
    docenteElem->InsertEndChild(userElem);

    XMLElement* passElem = doc.NewElement("Password");
    passElem->SetText(password.c_str());
    docenteElem->InsertEndChild(passElem);

    XMLElement* nomeElem = doc.NewElement("Nome");
    nomeElem->SetText(nome.c_str());
    docenteElem->InsertEndChild(nomeElem);

    XMLElement* cognomeElem = doc.NewElement("Cognome");
    cognomeElem->SetText(cognome.c_str());
    docenteElem->InsertEndChild(cognomeElem);

    // Classi
    if (classiIds.size() > 0) {
        XMLElement* classiElem = doc.NewElement("Classi");

        for (size_t i = 0; i < classiIds.size(); ++i) {
            XMLElement* classeRef = doc.NewElement("ClasseRef");
            string cIdStr = "CLA-" + to_string(classiIds[i]);
            classeRef->SetAttribute("id", cIdStr.c_str());
            classiElem->InsertEndChild(classeRef);
        }

        docenteElem->InsertEndChild(classiElem);
    }

    // Materie
    if (materieIds.size() > 0) {
        XMLElement* materieElem = doc.NewElement("Materie");

        for (size_t i = 0; i < materieIds.size(); ++i) {
            XMLElement* materiaRef = doc.NewElement("MateriaRef");
            string mIdStr = "MAT-" + to_string(materieIds[i]);
            materiaRef->SetAttribute("id", mIdStr.c_str());
            materieElem->InsertEndChild(materiaRef);
        }

        docenteElem->InsertEndChild(materieElem);
    }

    return docenteElem;
}

// ============================================
// CSV - SCRITTURA (toCSV)
// Formato:
// id,username,password,nome,cognome,classiIds(;),materieIds(;)
// ============================================
string Docente::toCSV() const {
    char splitter = ',';

    string CSV = to_string(this->id);
    CSV += splitter;
    CSV += this->username;
    CSV += splitter;
    CSV += this->password;
    CSV += splitter;
    CSV += this->nome;
    CSV += splitter;
    CSV += this->cognome;
    CSV += splitter;

    // classiIds con separatore interno ';'
    string classiStr = "";
    for (size_t i = 0; i < classiIds.size(); ++i) {
        if (i > 0) {
            classiStr += ";";
        }
        classiStr += to_string(classiIds[i]);
    }
    CSV += classiStr;
    CSV += splitter;

    // materieIds con separatore interno ';'
    string materieStr = "";
    for (size_t i = 0; i < materieIds.size(); ++i) {
        if (i > 0) {
            materieStr += ";";
        }
        materieStr += to_string(materieIds[i]);
    }
    CSV += materieStr;

    return CSV;
}

// ============================================
// CSV - LETTURA (fromCSV)
// ============================================
void Docente::fromCSV(const string& riga) {
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

    // ClassiIds (separati da ';')
    classiIds.clear();
    if (getline(ss, temp, ',')) {
        if (!temp.empty()) {
            stringstream ssC(temp);
            string idStr;

            while (getline(ssC, idStr, ';')) {
                if (!idStr.empty()) {
                    int cId = atoi(idStr.c_str());
                    classiIds.push_back(cId);
                }
            }
        }
    }

    // MaterieIds (separati da ';')
    materieIds.clear();
    if (getline(ss, temp, ',')) {
        if (!temp.empty()) {
            stringstream ssM(temp);
            string idStr;

            while (getline(ssM, idStr, ';')) {
                if (!idStr.empty()) {
                    int mId = atoi(idStr.c_str());
                    materieIds.push_back(mId);
                }
            }
        }
    }

    // puntatori runtime risolti dopo
    classi.clear();
    materie.clear();
}

// ============================================
// XML - SCRITTURA STRING (toXML stringa)
// ============================================
string Docente::toXML() const {
    string xml;

    xml += "<Docente id=\"";
    xml += "DOC-" + to_string(id);
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

    // Classi
    if (classiIds.size() > 0) {
        xml += "<Classi>";

        for (size_t i = 0; i < classiIds.size(); ++i) {
            xml += "<ClasseRef id=\"CLA-";
            xml += to_string(classiIds[i]);
            xml += "\"/>";
        }

        xml += "</Classi>";
    }

    // Materie
    if (materieIds.size() > 0) {
        xml += "<Materie>";

        for (size_t i = 0; i < materieIds.size(); ++i) {
            xml += "<MateriaRef id=\"MAT-";
            xml += to_string(materieIds[i]);
            xml += "\"/>";
        }

        xml += "</Materie>";
    }

    xml += "</Docente>";
    return xml;
}

// ============================================
// RAPPRESENTAZIONE TESTUALE (toString)
// ============================================
string Docente::toString() const {
    string s;

    s += "Docente ID: ";
    s += "DOC-" + to_string(id);
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

    s += "  ClassiIds: ";
    if (classiIds.size() == 0) {
        s += "(nessuna)";
    }
    else {
        for (size_t i = 0; i < classiIds.size(); ++i) {
            if (i > 0) {
                s += ", ";
            }
            s += "CLA-" + to_string(classiIds[i]);
        }
    }
    s += "\n";

    s += "  MaterieIds: ";
    if (materieIds.size() == 0) {
        s += "(nessuna)";
    }
    else {
        for (size_t i = 0; i < materieIds.size(); ++i) {
            if (i > 0) {
                s += ", ";
            }
            s += "MAT-" + to_string(materieIds[i]);
        }
    }
    s += "\n";

    s += "  Classi (ptr): ";
    s += to_string(classi.size());
    s += "\n";

    s += "  Materie (ptr): ";
    s += to_string(materie.size());
    s += "\n";

    return s;
}
