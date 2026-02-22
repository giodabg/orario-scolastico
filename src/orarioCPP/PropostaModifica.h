#pragma once
#include <string>
#include <vector>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Utente;
class Lezione;

struct CoppiaLezione {
    Lezione* oraSorgente;
    Lezione* oraDestinazione;
};

enum class TipoModifica { SCAMBIO, PRIMA_VA_IN_SECONDA };
enum class StatoProposta { NESSUN_COMMENTO, APPROVATA };

class PropostaModifica {
private:
    int id;

    // proponente (runtime) + id persistente
    int proponenteId;
    Utente* proponente;

    // Coppie (runtime) + ids persistenti
    vector<CoppiaLezione> coppieOre;
    vector<int> sorgentiIds;
    vector<int> destinazioniIds;

    TipoModifica tipo;
    string descrizione;
    StatoProposta stato;
    string commento;

public:
    PropostaModifica();
    ~PropostaModifica();

    // XML TinyXML2
    XMLElement* toXML(XMLDocument& doc) const;
    static PropostaModifica* fromXML(XMLElement* propostaElem);

    // XML stringa
    string toXML() const;

    // CSV
    string toCSV() const;
    void fromCSV(string rigaCSV);

    // Utility
    string toString() const;

    // Resolve
    void resolveProponente(const vector<Utente*>& tuttiUtenti);
    void resolveLezioni(const class Orario& orario);  // usa cercaPerId
    bool checkConflitto() const;

    // getter/setter minimi utili
    int getId() const;
    void setId(int id);

    int getProponenteId() const;
    void setProponenteId(int id);

    Utente* getProponente() const;
    void setProponente(Utente* u);

    TipoModifica getTipo() const;
    void setTipo(TipoModifica t);

    StatoProposta getStato() const;
    void setStato(StatoProposta s);

    string getDescrizione() const;
    void setDescrizione(const string& d);

    string getCommento() const;
    void setCommento(const string& c);

    const vector<CoppiaLezione>& getCoppieOre() const;
};