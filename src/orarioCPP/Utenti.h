#pragma once
#include <string>
#include <vector>
#include "tinyxml2.h"
#include "Utente.h"
#include "ContenitoreXML.h"

using namespace std;

class Utenti : public ContenitoreXML {
private:
    
    vector<Utente*> items;

public:
    
    ~Utenti();

    bool loadFromFile(const string& path) override;
    bool saveToFile(const string& path) const override;


    bool loadFromCSV(const string& path);
    bool saveToCSV(const string& path) const;


    void clear() override;
    size_t size() const override;

    bool esisteId(int id) const;
    string toString() const;

    const vector<Utente*>& getItems() const { return items; }
};
