#pragma once
#include <string>
#include <vector>
#include "tinyxml2.h"

// Forward declaration
class Studente;

using namespace std;
using namespace tinyxml2;

class Classe {
private:
    int id;                  // es. 1, 4
    int anno;                // es. 3, 5
    char sezione;            // es. 'A', 'B'
    string indirizzo;        // es. "INF", "CHI"
    
    vector<int> studentiIds; // IDs per serializzazione
    vector<Studente*> studenti;    // Puntatori per uso runtime
public:
    // Costruttore di default
    Classe();

    // Getter
    int getId() const { return id; }
    int getAnno() const { return anno; }
    char getSezione() const { return sezione; }
    const string& getInd() const { return indirizzo; }

    // Getter IDs
    const vector<int>& getStudentiIds() const { return studentiIds; }

    // Getter puntatori
    const vector<Studente*>& getStudenti() const { return studenti; }
    Studente* getStudente(size_t index) const;

    // Setter
    void setId(int num) { id = num; }
    void setAnno(int a) { anno = a; }
    void setSezione(char s) { sezione = s; }
    void setInd(const string& i) { indirizzo = i; }

    // Setter IDs
    void addStudenteId(int stuId) { studentiIds.push_back(stuId); }

    // Setter puntatori
    void addStudente(Studente* stu) { studenti.push_back(stu); }

    // Metodo di risoluzione
    void resolveStudenti(const vector<Studente*>& tuttiStudenti);

    // Ritorna un oggetto Classe* costruito da XML
    static Classe* fromXML(XMLElement* classeElem);

    // Crea <Classe>...</Classe> e lo ritorna
    XMLElement* toXML(XMLDocument& doc) const;

    // Converte in stringa CSV
    string toCSV() const;

    // Carica da stringa CSV
    void fromCSV(const string& riga);

    // utility
    string toXML() const;
    string toString() const;
};
