#pragma once
#include <string>
#include <vector>
#include "Aula.h"
#include "ContenitoreXML.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Aule : public ContenitoreXML {
private:
    vector<Aula*> items;

public:
    // Costruttore
    Aule();

    // Distruttore (libera memoria)
    ~Aule();

    bool loadFromFile(const string& path) override;
    bool saveToFile(const string& path) const override;
    bool saveToFileOld(const string& path) const;

    void clear() override;
    size_t size() const override;

    // CRUD - CREATE
    bool aggiungi(Aula* aula);

    // CRUD - READ
    Aula* cercaPerId(int id) const;
    Aula* cercaPerNome(const string& nome) const;
    Aula* get(size_t index) const;
    vector<Aula*> cercaPerTipo(TipoAula tipo) const;
    vector<Aula*> cercaPerPostiMinimi(int postiMin) const;

    // CRUD - UPDATE
    bool modifica(int id, Aula* nuovaAula);

    // CRUD - DELETE
    bool rimuoviPerId(int id);
    bool rimuoviPerIndex(size_t index);

    // Utility
    bool esisteId(int id) const;
    string toXML() const;
    string toString() const;
};

