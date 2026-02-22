#pragma once
#include <string>
#include "Utente.h"
#include "tinyxml2.h"

// forward declaration
class Classe;

using namespace std;
using namespace tinyxml2;

class Studente : public Utente {
private:
    string tipo;      //normale o rappresentante
    int classeId;     // ID della classe (per serializzazione)
    Classe* classe;   //puntatore alla classe di appartenenza

public:
    // Costruttore di default
    Studente();

    // Getter
    string getTipo() const { return tipo; }
    int getClasseId() const { return classeId; }
    Classe* getClasse() const { return classe; }

    // Setter
    void setTipo(const string& t) { tipo = t; }
    void setClasseId(int cId) { classeId = cId; }
    void setClasse(Classe* c) { classe = c; }

    // Metodo di risoluzione
    void resolveClasse(Classe* c) { classe = c; }

    // Override metodi virtuali della classe base
    string getTipoUtente() const override;

    // Metodi CSV
    string toCSV() const override;
    void fromCSV(const string& riga) override;

    // Metodi XML con TinyXML2
    static Studente* fromXML(XMLElement* studenteElem);
    XMLElement* toXML(XMLDocument& doc) const override;

    // utility
    string toXML() const;
    string toString() const override;
};
