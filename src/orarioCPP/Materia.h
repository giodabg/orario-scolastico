#pragma once
#include <string>
#include <vector>
#include "tinyxml2.h"

// Forward declaration
class Docente;

using namespace std;
using namespace tinyxml2;

class Materia {
private:
    int id;
    string nome;
    bool laboratorio;
    // IDs per serializzazione
    vector<int> docentiIds;

    // Puntatori per uso runtime
    vector<Docente*> docenti;
public:
    // Costruttore di default
    Materia();
    Materia(int id, string nome, bool laboratorio);

    // Getter
    int getId() const { return id; }
    const string& getNome() const { return nome; }
    bool getLaboratorio() const { return laboratorio; }
    const vector<int>& getDocentiIds() const { return docentiIds; }
    const vector<Docente*>& getDocenti() const { return docenti; }

    // Setter
    void setId(int i) { id = i; }
    void setNome(const string& n) { nome = n; }
    void setLaboratorio(bool l) { laboratorio = l; }
    void addDocenteId(int docId) { docentiIds.push_back(docId); }
    void addDocente(Docente* doc) { docenti.push_back(doc); }

    // Metodo di risoluzione
    void resolveDocenti(const vector<Docente*>& tuttiDocenti);

    // Metodi XML
    static Materia* fromXML(XMLElement* materiaElem);
    XMLElement* toXML(XMLDocument& doc) const;
    // Metodi CSV
    string toCSV() const;
    void fromCSV(const string& riga);

    // utility
    string toXML() const;
    string toString() const;
};