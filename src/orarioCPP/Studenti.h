#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Studente.h"
#include "tinyxml2.h"
#include "ContenitoreXML.h" // Assumendo esista come nel tuo esempio

using namespace std;
using namespace tinyxml2;

class Studenti : public ContenitoreXML {
private:
    vector<Studente*> items;

public:
    Studenti() {}
    ~Studenti();

    // Metodi principali
    bool loadFromFile(const string& path) override;
    bool saveToFile(const string& path) const override;

    // Metodo alternativo che usa il toXML() a stringa di Studente
    bool saveToFileOld(const string& path) const;

    void clear() override;
    size_t size() const override;

    // Utilità
    bool esisteId(const string& id) const;
    string toString() const;
};
