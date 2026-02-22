#pragma once
#include <string>
#include <vector>
#include "Studente.h"
#include "ContenitoreXML.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Studenti : public ContenitoreXML {
private:
    vector<Studente*> items;

public:
    // Costruttore
    Studenti();

    // Distruttore (libera memoria)
    ~Studenti();


    // Metodi principali
    bool loadFromFile(const string& path) override;
    bool saveToFile(const string& path) const override;

    // Metodo alternativo che usa il toXML() a stringa di Studente
    bool saveToFileOld(const string& path) const;

    bool loadFromCSV(const string& path);
    bool saveToCSV(const string& path) const;

    void clear() override;
    size_t size() const override;

    // CRUD - CREATE
    bool aggiungi(Studente* studente);

    // CRUD - READ
    Studente* cercaPerId(int id) const;
    Studente* cercaPerUsername(const string& username) const;
    Studente* get(size_t index) const;

    // CRUD - UPDATE
    bool modifica(int id, Studente* nuovoStudente);

    // CRUD - DELETE
    bool rimuoviPerId(int id);
    bool rimuoviPerIndex(size_t index);

    // Utilità
    bool esisteId(int id) const;
    string toXML() const;
    string toString() const;
};
