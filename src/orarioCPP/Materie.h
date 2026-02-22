#pragma once
#include <vector>
#include <string>
#include "Materia.h"
#include "ContenitoreXML.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Materie : public ContenitoreXML {
private:
    vector<Materia*> items;

public:
    // Costruttore
    Materie();

    // Distruttore (libera memoria)
    ~Materie();

    bool loadFromFile(const string& path) override;
    bool saveToFile(const string& path) const override;
    bool saveToFileOld(const string& path) const;
    bool loadFromCSV(const string& path);
    bool saveToCSV(const string& path) const;

    void clear() override;
    size_t size() const override;

    // CRUD - CREATE
    bool aggiungi(Materia* materia);

    // CRUD - READ
    Materia* cercaPerId(int id) const;
    Materia* cercaPerNome(const string& nome) const;
    Materia* get(size_t index) const;
    vector<Materia*> cercaLaboratori() const;

    // CRUD - UPDATE
    bool modifica(int id, Materia* nuovaMateria);

    // CRUD - DELETE
    bool rimuoviPerId(int id);
    bool rimuoviPerIndex(size_t index);

    // Utility
    bool esisteId(int id) const;
    string toXML() const;
    string toString() const;
};