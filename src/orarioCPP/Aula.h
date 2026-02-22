#pragma once
#include <string>
#include <vector>
#include "libUtils.h"
#include "tinyxml2.h"

// Forward declaration
class Materia;

using namespace std;
using namespace tinyxml2;

enum TipoAula { AULA_STANDARD = 1, AULA_MULTIFUNZIONALE, AULA_DEDICATA };

class Aula {
private:
    int id;
    string nome;
    int posti;
    TipoAula tipo;
    // IDs per serializzazione
    vector<int> materieAmmesseIds;

    // Puntatori per uso runtime
    vector<Materia*> materieAmmesse;
public:
// Costruttore di default
    Aula();

    // Getter
	int getId() const { return id; }
	const string& getNome() const { return nome; }
	int getPosti() const { return posti; }
	TipoAula getTipo() const { return tipo; }
    // Getter IDs
    const vector<int>& getMaterieAmmesseIds() const { return materieAmmesseIds; }
    // Getter puntatori
    const vector<Materia*>& getMaterieAmmesse() const { return materieAmmesse; }

    // Setter
    void setId(int num) { id = num; };
    void setNome(const string& n) { nome = n; }
    void setPosti(int p) { posti = p; }
    void setTipo(TipoAula t) { tipo = t; }

    // Setter IDs
    void addMateriaAmmessaId(int matId) { materieAmmesseIds.push_back(matId); }

    // Setter puntatori
    void addMateriaAmmessa(Materia* mat) { materieAmmesse.push_back(mat); }

    // Metodo di risoluzione
    void resolveMaterie(const vector<Materia*>& tutteMaterie);
    
    // Ritorna un oggetto Aula* costruito da 
    // <Aula id="..."><Nome>...</Nome> ... <MaterieAmmesse>...</MaterieAmmesse></Aula>
    static Aula* fromXML(XMLElement* aulaElem);

    // Crea <Aula>...</Aula> e lo ritorna (agganciabile al root dal chiamante)
    XMLElement* toXML(XMLDocument& doc) const;

    static const char* tipoAulaToString(TipoAula t);

    // Converte in stringa CSV
    string toCSV() const;

    // Carica da stringa CSV
    void fromCSV(const string& riga);

    // utility
    string toXML() const;
    string toString() const;
};
