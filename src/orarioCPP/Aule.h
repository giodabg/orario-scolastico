#pragma once
#include <string>
#include <map>
#include <vector>
#include "tinyxml2.h"
#include "Aula.h"
#include "ContenitoreXML.h"

class Aule : public ContenitoreXML {
private:
    vector<Aula*> items;

public:
    ~Aule();

    bool loadFromFile(const string& path) override;
    bool saveToFile(const string& path) const override;
    bool saveToFileOld(const string& path) const;

    void clear() override;
    size_t size() const override;

    bool esisteId(int id) const;

    string toString() const;
};

