#pragma once
#include <iostream>
#include <string>   

// using namespace std;


class ContenitoreXML {
public:
    virtual ~ContenitoreXML() {}

    virtual bool loadFromFile(const std::string& path) = 0;
    virtual bool saveToFile(const std::string& path) const = 0;

    virtual void clear() = 0;
    virtual size_t size() const = 0;
};