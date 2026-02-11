#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Materia.h"
#include "tinyxml2.h"
#include "ContenitoreXML.h"

using namespace std;
using namespace tinyxml2;

public class Materie : public ContenitoreXML {
private:
	vector<Materia*> items;

public:
	Materie() {}
	~Materie();

	bool loadFromFile(const string& path) override;
	bool saveToFile(const string& path) const override;
	bool saveToFileOld(const string& path) const;
	void clear() override;
	size_t size() const override;
	bool esisteId(const string& id) const;
	string toString() const;
}