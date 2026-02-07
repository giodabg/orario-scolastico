#pragma once

#include <string>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Materia { 
private:                
    string nome;            
    List<Docente> docenti;
    bool Laboratorio;
public:
    Materia();
    Materia(string nome, bool laboratorio);

    const string& getNome() const { return nome; }
    void setNome(const string& n) { nome = n; }
    bool getLaboratorio() const { return laboratorio; }
    void setLaboratorio(bool l) { laboratorio = l; }
    List<Docente>& getDocenti() { return docenti; }
    void aggiungiDocente(const Docente& d);
    //Metodi XML
    static Materia* fromXML(XMLElement* materiaElem);
    XMLElement* toXML(XMLDocument& doc) const; 
    string toXML() const;
    string toString() const;
    static string escapeXML(const string& s);
