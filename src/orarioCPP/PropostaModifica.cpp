#include "PropostaModifica.h"
#include "Utente.h"
#include "Lezione.h"
#include "Orario.h"
#include <sstream>
#include <cstdlib>

using namespace std;

// ============================================
// SUPPORTO (locali al file)
// ============================================
static string escapeXMLLocal(const string& in) {
    string out = "";
    for (size_t i = 0; i < in.size(); ++i) {
        char c = in[i];
        if (c == '&') out += "&amp;";
        else if (c == '<') out += "&lt;";
        else if (c == '>') out += "&gt;";
        else if (c == '\"') out += "&quot;";
        else if (c == '\'') out += "&apos;";
        else out += c;
    }
    return out;
}

static int estraiIdNumericoLocal(const string& s) {
    string num = "";
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] >= '0' && s[i] <= '9') {
            num += s[i];
        }
    }
    if (num.size() == 0) return -1;
    return atoi(num.c_str());
}

static string tipoToString(TipoModifica t) {
    if (t == TipoModifica::SCAMBIO) return "SCAMBIO";
    return "PRIMA_VA_IN_SECONDA";
}

static TipoModifica stringToTipo(const string& s) {
    if (s == "SCAMBIO") return TipoModifica::SCAMBIO;
    return TipoModifica::PRIMA_VA_IN_SECONDA;
}

static string statoToString(StatoProposta s) {
    if (s == StatoProposta::APPROVATA) return "APPROVATA";
    return "NESSUN_COMMENTO";
}

static StatoProposta stringToStato(const string& s) {
    if (s == "APPROVATA") return StatoProposta::APPROVATA;
    return StatoProposta::NESSUN_COMMENTO;
}

// ============================================
// COSTRUTTORE / DISTRUTTORE
// ============================================
PropostaModifica::PropostaModifica() {
    id = -1;

    proponenteId = -1;
    proponente = nullptr;

    coppieOre.clear();
    sorgentiIds.clear();
    destinazioniIds.clear();

    tipo = TipoModifica::SCAMBIO;
    descrizione = "";
    stato = StatoProposta::NESSUN_COMMENTO;
    commento = "";
}

PropostaModifica::~PropostaModifica() {
    // Non possiede la memoria delle lezioni: sono dell'Orario.
    // Quindi NON delete sui puntatori in coppieOre.
    coppieOre.clear();
}

// ============================================
// GETTER / SETTER MINIMI
// ============================================
int PropostaModifica::getId() const { return id; }
void PropostaModifica::setId(int id) { this->id = id; }

int PropostaModifica::getProponenteId() const { return proponenteId; }
void PropostaModifica::setProponenteId(int id) { proponenteId = id; }

Utente* PropostaModifica::getProponente() const { return proponente; }
void PropostaModifica::setProponente(Utente* u) {
    proponente = u;
    if (u != nullptr) {
        proponenteId = u->getId();
    }
}

TipoModifica PropostaModifica::getTipo() const { return tipo; }
void PropostaModifica::setTipo(TipoModifica t) { tipo = t; }

StatoProposta PropostaModifica::getStato() const { return stato; }
void PropostaModifica::setStato(StatoProposta s) { stato = s; }

string PropostaModifica::getDescrizione() const { return descrizione; }
void PropostaModifica::setDescrizione(const string& d) { descrizione = d; }

string PropostaModifica::getCommento() const { return commento; }
void PropostaModifica::setCommento(const string& c) { commento = c; }

const vector<CoppiaLezione>& PropostaModifica::getCoppieOre() const { return coppieOre; }

// ============================================
// XML - SCRITTURA (TinyXML2)
// Struttura:
// <PropostaModifica id="PM-1" tipo="SCAMBIO" stato="NESSUN_COMMENTO" proponenteId="5">
//   <Descrizione>...</Descrizione>
//   <Commento>...</Commento>
//   <CoppieOre>
//     <Coppia sorgenteId="12" destinazioneId="34"/>
//   </CoppieOre>
// </PropostaModifica>
// ============================================
XMLElement* PropostaModifica::toXML(XMLDocument& doc) const {
    XMLElement* elem = doc.NewElement("PropostaModifica");

    string idStr = "PM-" + to_string(id);
    elem->SetAttribute("id", idStr.c_str());
    elem->SetAttribute("tipo", tipoToString(tipo).c_str());
    elem->SetAttribute("stato", statoToString(stato).c_str());
    elem->SetAttribute("proponenteId", proponenteId);

    XMLElement* d = doc.NewElement("Descrizione");
    d->SetText(descrizione.c_str());
    elem->InsertEndChild(d);

    XMLElement* c = doc.NewElement("Commento");
    c->SetText(commento.c_str());
    elem->InsertEndChild(c);

    // coppie: salvo gli ID (robusto e coerente con puntatori)
    if (sorgentiIds.size() > 0 && sorgentiIds.size() == destinazioniIds.size()) {
        XMLElement* coppieElem = doc.NewElement("CoppieOre");

        for (size_t i = 0; i < sorgentiIds.size(); ++i) {
            XMLElement* coppia = doc.NewElement("Coppia");
            coppia->SetAttribute("sorgenteId", sorgentiIds[i]);
            coppia->SetAttribute("destinazioneId", destinazioniIds[i]);
            coppieElem->InsertEndChild(coppia);
        }

        elem->InsertEndChild(coppieElem);
    }

    return elem;
}

// ============================================
// XML - LETTURA (TinyXML2)
// ============================================
PropostaModifica* PropostaModifica::fromXML(XMLElement* propostaElem) {
    if (propostaElem == nullptr || string(propostaElem->Name()) != "PropostaModifica") {
        return nullptr;
    }

    PropostaModifica* p = new PropostaModifica();

    const char* idAttr = propostaElem->Attribute("id");
    if (idAttr != nullptr) {
        p->id = estraiIdNumericoLocal(idAttr);
    }

    const char* tipoAttr = propostaElem->Attribute("tipo");
    if (tipoAttr != nullptr) {
        p->tipo = stringToTipo(tipoAttr);
    }

    const char* statoAttr = propostaElem->Attribute("stato");
    if (statoAttr != nullptr) {
        p->stato = stringToStato(statoAttr);
    }

    p->proponenteId = propostaElem->IntAttribute("proponenteId", -1);
    p->proponente = nullptr; // resolve dopo

    XMLElement* d = propostaElem->FirstChildElement("Descrizione");
    if (d && d->GetText()) p->descrizione = d->GetText();

    XMLElement* c = propostaElem->FirstChildElement("Commento");
    if (c && c->GetText()) p->commento = c->GetText();

    // coppie (solo ids)
    p->sorgentiIds.clear();
    p->destinazioniIds.clear();
    p->coppieOre.clear();

    XMLElement* coppieElem = propostaElem->FirstChildElement("CoppieOre");
    if (coppieElem) {
        XMLElement* coppia = coppieElem->FirstChildElement("Coppia");
        while (coppia != nullptr) {
            int sId = coppia->IntAttribute("sorgenteId", -1);
            int dId = coppia->IntAttribute("destinazioneId", -1);

            p->sorgentiIds.push_back(sId);
            p->destinazioniIds.push_back(dId);

            coppia = coppia->NextSiblingElement("Coppia");
        }
    }

    return p;
}

// ============================================
// XML - SCRITTURA STRING
// ============================================
string PropostaModifica::toXML() const {
    string xml;

    xml += "<PropostaModifica id=\"PM-";
    xml += to_string(id);
    xml += "\" tipo=\"";
    xml += escapeXMLLocal(tipoToString(tipo));
    xml += "\" stato=\"";
    xml += escapeXMLLocal(statoToString(stato));
    xml += "\" proponenteId=\"";
    xml += to_string(proponenteId);
    xml += "\">";

    xml += "<Descrizione>";
    xml += escapeXMLLocal(descrizione);
    xml += "</Descrizione>";

    xml += "<Commento>";
    xml += escapeXMLLocal(commento);
    xml += "</Commento>";

    if (sorgentiIds.size() > 0 && sorgentiIds.size() == destinazioniIds.size()) {
        xml += "<CoppieOre>";
        for (size_t i = 0; i < sorgentiIds.size(); ++i) {
            xml += "<Coppia sorgenteId=\"";
            xml += to_string(sorgentiIds[i]);
            xml += "\" destinazioneId=\"";
            xml += to_string(destinazioniIds[i]);
            xml += "\"/>";
        }
        xml += "</CoppieOre>";
    }

    xml += "</PropostaModifica>";
    return xml;
}

// ============================================
// CSV - SCRITTURA
// Formato:
// id,tipo,stato,proponenteId,descrizione,commento,coppie
// coppie = "sorgenteId-destinazioneId;sorgenteId-destinazioneId;..."
// ============================================
string PropostaModifica::toCSV() const {
    char splitter = ',';

    string CSV = to_string(id);
    CSV += splitter;
    CSV += tipoToString(tipo);
    CSV += splitter;
    CSV += statoToString(stato);
    CSV += splitter;
    CSV += to_string(proponenteId);
    CSV += splitter;
    CSV += descrizione;
    CSV += splitter;
    CSV += commento;
    CSV += splitter;

    string coppie = "";
    if (sorgentiIds.size() > 0 && sorgentiIds.size() == destinazioniIds.size()) {
        for (size_t i = 0; i < sorgentiIds.size(); ++i) {
            if (i > 0) coppie += ";";
            coppie += to_string(sorgentiIds[i]);
            coppie += "-";
            coppie += to_string(destinazioniIds[i]);
        }
    }

    CSV += coppie;
    return CSV;
}

// ============================================
// CSV - LETTURA
// ============================================
void PropostaModifica::fromCSV(string rigaCSV) {
    stringstream ss(rigaCSV);
    string temp;

    coppieOre.clear();
    sorgentiIds.clear();
    destinazioniIds.clear();

    // id
    if (getline(ss, temp, ',')) {
        id = atoi(temp.c_str());
    }

    // tipo
    if (getline(ss, temp, ',')) {
        tipo = stringToTipo(temp);
    }

    // stato
    if (getline(ss, temp, ',')) {
        stato = stringToStato(temp);
    }

    // proponenteId
    if (getline(ss, temp, ',')) {
        proponenteId = atoi(temp.c_str());
    }
    else {
        proponenteId = -1;
    }
    proponente = nullptr;

    // descrizione
    if (getline(ss, temp, ',')) {
        descrizione = temp;
    }
    else {
        descrizione = "";
    }

    // commento
    if (getline(ss, temp, ',')) {
        commento = temp;
    }
    else {
        commento = "";
    }

    // coppie "a-b;c-d;..."
    if (getline(ss, temp, ',')) {
        if (temp.size() > 0) {
            stringstream ssC(temp);
            string tok;

            while (getline(ssC, tok, ';')) {
                if (tok.size() > 0) {
                    size_t pos = tok.find('-');
                    if (pos != string::npos) {
                        int sId = atoi(tok.substr(0, pos).c_str());
                        int dId = atoi(tok.substr(pos + 1).c_str());
                        sorgentiIds.push_back(sId);
                        destinazioniIds.push_back(dId);
                    }
                }
            }
        }
    }
}

// ============================================
// RESOLVE PROPONENTE (ID -> puntatore)
// ============================================
void PropostaModifica::resolveProponente(const vector<Utente*>& tuttiUtenti) {
    proponente = nullptr;

    if (proponenteId < 0) {
        return;
    }

    for (size_t i = 0; i < tuttiUtenti.size(); ++i) {
        if (tuttiUtenti[i] != nullptr && tuttiUtenti[i]->getId() == proponenteId) {
            proponente = tuttiUtenti[i];
        }
    }
}

// ============================================
// RESOLVE LEZIONI (ID -> puntatori)
// Usa Orario::cercaPerId
// ============================================
void PropostaModifica::resolveLezioni(const Orario& orario) {
    coppieOre.clear();

    if (sorgentiIds.size() == 0 || sorgentiIds.size() != destinazioniIds.size()) {
        return;
    }

    for (size_t i = 0; i < sorgentiIds.size(); ++i) {
        Lezione* s = orario.cercaPerId(sorgentiIds[i]);
        Lezione* d = orario.cercaPerId(destinazioniIds[i]);

        CoppiaLezione c;
        c.oraSorgente = s;
        c.oraDestinazione = d;

        coppieOre.push_back(c);
    }
}

// ============================================
// CHECK CONFLITTO (minimale ma utile)
// - true se una coppia ha uno dei due puntatori null
// - true se sorgente == destinazione (stesso id)
// - true se due coppie usano la stessa sorgente
// ============================================
bool PropostaModifica::checkConflitto() const {
    // Controllo coppie non risolte / incoerenti
    for (size_t i = 0; i < coppieOre.size(); ++i) {
        if (coppieOre[i].oraSorgente == nullptr || coppieOre[i].oraDestinazione == nullptr) {
            return true;
        }

        if (coppieOre[i].oraSorgente->getId() == coppieOre[i].oraDestinazione->getId()) {
            return true;
        }

        for (size_t j = i + 1; j < coppieOre.size(); ++j) {
            if (coppieOre[j].oraSorgente != nullptr &&
                coppieOre[i].oraSorgente->getId() == coppieOre[j].oraSorgente->getId()) {
                return true;
            }
        }
    }

    return false;
}

// ============================================
// TOSTRING
// ============================================
string PropostaModifica::toString() const {
    string s;

    s += "PropostaModifica ID: PM-";
    s += to_string(id);
    s += "\n";

    s += "  Tipo: ";
    s += tipoToString(tipo);
    s += "\n";

    s += "  Stato: ";
    s += statoToString(stato);
    s += "\n";

    s += "  ProponenteId: ";
    s += to_string(proponenteId);
    s += "\n";

    s += "  Proponente(ptr): ";
    s += (proponente != nullptr ? "OK" : "NULL");
    s += "\n";

    s += "  Descrizione: ";
    s += descrizione;
    s += "\n";

    s += "  Commento: ";
    if (commento.size() == 0) s += "(nessuno)";
    else s += commento;
    s += "\n";

    s += "  CoppieOre: ";
    s += to_string(coppieOre.size());
    s += "\n";

    for (size_t i = 0; i < coppieOre.size(); ++i) {
        s += "    - Coppia ";
        s += to_string(i + 1);
        s += ": ";

        if (coppieOre[i].oraSorgente != nullptr) {
            s += "Sorgente=LEZ-";
            s += to_string(coppieOre[i].oraSorgente->getId());
        }
        else {
            s += "Sorgente=NULL";
        }

        s += " -> ";

        if (coppieOre[i].oraDestinazione != nullptr) {
            s += "Destinazione=LEZ-";
            s += to_string(coppieOre[i].oraDestinazione->getId());
        }
        else {
            s += "Destinazione=NULL";
        }

        s += "\n";
    }

    return s;
}