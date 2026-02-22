#pragma once
#include <string>
#include <vector>
#include <deque>
#include "Lezione.h"
#include "Utenti.h" 
#include "PropostaModifica.h"
#include "ContenitoreXML.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


class Orario : public ContenitoreXML {
private:
    vector<Lezione*> items;
    deque<PropostaModifica> codaProposte;

public:
    // Costruttore di default
    Orario();

    // Distruttore (libera memoria)
    ~Orario();

    bool loadFromFile(const string& path) override;
    bool saveToFile(const string& path) const override;

    bool loadFromCSV(const string& path);
    bool saveToCSV(const string& path) const;

    void clear() override;
    size_t size() const override;

    // CRUD - CREATE
    bool aggiungi(Lezione* lezione);

    // CRUD - READ
    Lezione* cercaPerId(int id) const;
    Lezione* get(size_t index) const;

    vector<Lezione*> cercaPerGiorno(GiornoSettimana giorno) const;
    vector<Lezione*> cercaPerClasseId(int classeId) const;
    vector<Lezione*> cercaPerDocenteId(int docenteId) const;
    vector<Lezione*> cercaPerAulaId(int aulaId) const;
    Lezione* cercaPerGiornoOraClasse(GiornoSettimana giorno, int ora, int classeId) const;

    // CRUD - UPDATE
    bool modifica(int id, Lezione* nuovaLezione);

    // CRUD - DELETE
    bool rimuoviPerId(int id);
    bool rimuoviPerIndex(size_t index);

    // Metodo per risolvere tutti i riferimenti
    void resolveAll(const vector<Classe*>& tutteClassi,
        const vector<Aula*>& tutteAule,
        const vector<Materia*>& tutteMaterie,
        const vector<Docente*>& tuttiDocenti);


    // metodi sulla coda di proposte
    bool aggiungiProposta(const PropostaModifica& p);
    size_t sizeProposte() const;
    PropostaModifica* getProposta(size_t index);
    bool saveProposteToFile(const string& path) const;
    bool loadProposteFromFile(const string& path);
    bool saveProposteToCSV(const string& path) const;
    bool loadProposteFromCSV(const string& path);
    void resolveProposte(const vector<Utente*>& tuttiUtenti);

    // utility
    bool esisteId(int id) const;
    string toXML() const;
    string toString() const;
};
