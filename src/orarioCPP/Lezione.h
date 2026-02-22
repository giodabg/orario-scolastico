#pragma once
#include <string>
#include <vector>
#include "tinyxml2.h"

// Forward declarations
class Docente;
class Classe;
class Aula;
class Materia;

using namespace std;
using namespace tinyxml2;

enum GiornoSettimana {
    LUNEDI = 1,
    MARTEDI,
    MERCOLEDI,
    GIOVEDI,
    VENERDI,
    SABATO,
    DOMENICA
};

class Lezione {
private:
    int id;
    GiornoSettimana giorno;
    int ora;
    // IDs per serializzazione
    vector<int> docentiIds;
    int classeId;              // ID della classe
    int aulaId;                // ID dell'aula
    int materiaId;             // ID della materia
    // Puntatori per uso runtime
    vector<Docente*> docenti;
    Classe* classe;
    Aula* aula;
    Materia* materia;
public:
    // Costruttore di default
    Lezione();

    // Getter
    int getId() const { return id; }
    GiornoSettimana getGiorno() const { return giorno; }
    int getOra() const { return ora; }
    const vector<int>& getDocentiIds() const { return docentiIds; }
    int getClasseId() const { return classeId; }
    int getAulaId() const { return aulaId; }
    int getMateriaId() const { return materiaId; }
    // Getter puntatori
    const vector<Docente*>& getDocenti() const { return docenti; }
    Classe* getClasse() const { return classe; }
    Aula* getAula() const { return aula; }
    Materia* getMateria() const { return materia; }

    // Setter
    void setId(int num) { id = num; }
    void setGiorno(GiornoSettimana g) { giorno = g; }
    void setOra(int o) { ora = o; }
    void addDocenteId(int docId) { docentiIds.push_back(docId); }
    void setClasseId(int cId) { classeId = cId; }
    void setAulaId(int aId) { aulaId = aId; }
    void setMateriaId(int mId) { materiaId = mId; }

    // Setter puntatori
    void addDocente(Docente* doc) { docenti.push_back(doc); }
    void setClasse(Classe* c) { classe = c; }
    void setAula(Aula* a) { aula = a; }
    void setMateria(Materia* m) { materia = m; }

    // Metodi di risoluzione
    void resolve(const vector<Classe*>& tutteClassi,
        const vector<Aula*>& tutteAule,
        const vector<Materia*>& tutteMaterie,
        const vector<Docente*>& tuttiDocenti);

    // Ritorna un oggetto Lezione* costruito da XML
    static Lezione* fromXML(XMLElement* lezioneElem);

    // Crea <Lezione>...</Lezione> e lo ritorna
    XMLElement* toXML(XMLDocument& doc) const;

    // Metodi CSV
    string toCSV() const;
    void fromCSV(const string& riga);

    // Conversione GiornoSettimana
    static const char* giornoToString(GiornoSettimana g);
    static GiornoSettimana stringToGiorno(const string& s);

    // utility
    string toXML() const;
    string toString() const;
};