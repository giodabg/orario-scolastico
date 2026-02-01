#pragma once

#include <string>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

enum TipoAula { AULA_STANDARD = 1, AULA_MULTIFUNZIONALE, AULA_DEDICATA };

class Aula {
private:
    int id;
    string nome;
    int posti;
    TipoAula tipo;

public:

	int getId() const { return id; }
    void setId(int num) { id = num; };
	const string& getNome() const { return nome; }
	void setNome(const string& n) { nome = n; }
	int getPosti() const { return posti; }
	void setPosti(int p) { posti = p; }
	TipoAula getTipo() const { return tipo; }
	void setTipo(TipoAula t) { tipo = t; }

	// Ritorna un oggetto Aula* costruito da 
    // <Aula id="..."><Nome>...</Nome> ... <MaterieAmmesse>...</MaterieAmmesse></Aula>
    static Aula* fromXML(XMLElement* aulaElem);

    // Crea <Aula>...</Aula> e lo ritorna (agganciabile al root dal chiamante)
    XMLElement* toXML(XMLDocument& doc) const;

    static const char* tipoAulaToString(TipoAula t);
    static string escapeXML(const string& s);
	// Ritorna la rappresentazione XML completa come stringa
    string toXML() const;

	string toString() const;

};
