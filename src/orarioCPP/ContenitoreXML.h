#pragma once
#include <string>   
using namespace std;


class ContenitoreXML {
public:
    virtual ~ContenitoreXML() {}

    virtual bool loadFromFile(const string& path) = 0;
    virtual bool saveToFile(const string& path) const = 0;

    virtual void clear() = 0;
    virtual size_t size() const = 0;
};