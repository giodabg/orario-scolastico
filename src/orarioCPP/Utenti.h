#pragma once
#include <string>
#include <vector>
#include "Utente.h"
#include "ContenitoreXML.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Utenti : public ContenitoreXML {
private:
    
    vector<Utente*> items;

public:
    // Costruttore
    Utenti();

    // Distruttore (libera memoria)
    ~Utenti();

    bool loadFromFile(const string& path) override;
    bool saveToFile(const string& path) const override;

    bool loadFromCSV(const string& path);
    bool saveToCSV(const string& path) const;

    Utente* get(size_t index) const;

    void clear() override;
    size_t size() const override;

    // utility
    bool esisteId(int id) const;
    string toXML() const;
    string toString() const;
};
