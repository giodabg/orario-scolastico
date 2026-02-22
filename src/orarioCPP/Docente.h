#pragma once
#include <string>
#include <vector>
#include "Utente.h"
#include "tinyxml2.h"

// Forward declarations
class Classe;
class Materia;

using namespace std;
using namespace tinyxml2;

class Docente : public Utente {
private:
    // IDs per serializzazione
    vector<int> classiIds;
    vector<int> materieIds;

    // Puntatori per uso runtime
    vector<Classe*> classi;
    vector<Materia*> materie;

public:
    // Costruttore di default
    Docente();

    // Getter
    const vector<int>& getClassiIds() const { return classiIds; }
    const vector<int>& getMaterieIds() const { return materieIds; }

    const vector<Classe*>& getClassi() const { return classi; }
    const vector<Materia*>& getMaterie() const { return materie; }

    // Setter
    void addClasseId(int classId) { classiIds.push_back(classId); }
    void addMateriaId(int matId) { materieIds.push_back(matId); }

    void addClasse(Classe* c) { classi.push_back(c); }
    void addMateria(Materia* m) { materie.push_back(m); }

    // Metodi di risoluzione
    void resolveClassi(const vector<Classe*>& tutteClassi);
    void resolveMaterie(const vector<Materia*>& tutteMaterie);

    // Override metodi virtuali della classe base
    string getTipoUtente() const override;

    // Metodi XML
    static Docente* fromXML(XMLElement* docenteElem);
    XMLElement* toXML(XMLDocument& doc) const override;
    // Metodi CSV
    string toCSV() const override;
    void fromCSV(const string& riga) override;

    // utility
    string toXML() const;
    string toString() const override;
};
