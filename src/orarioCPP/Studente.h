#pragma once
#include "Utente.h"
#include <sstream>
#include <string>
using namespace std;
class Studente : public Utente
{
	enum TipoStudente {
		STUDENTE_NORMALE = 1,
		STUDENTE_RAPPRESENTANTE
	};

private:
	TipoStudente tipo;              
	Classe* classe;                 
public:
	string getTipoUtente();

	Studente(int id, string nome, string cognome, string username, 
			string password, TipoStudente tipo, Classe* classe);

	//void visualizzaOrario(Orario orario);
	//PropostaModifica proponiModificaOrario(Orario orario); se this.tipo== STUDENTE_RAPPRESENTANTE propone modifica
	void toCSV() override;
	void fromCSV(string rigaCSV) override;
	string toXML() override;
	void fromXml(string rigaXml) override;
	void toString() override;

	string Studente::toXML()
	{ 
	string s = "";
     if(this->tipo == STUDENTE_NORMALE)
	  {
        s += "  <Studenti>\n";
		s += "	   <Studente id= STU-" + to_string(this->id) + ">\n";
		s += "      <Nome>" + this->nome + "</Nome>\n";
		s += "		<Cognome>" + this->cognome + "</Cognome>\n";
		s += "		<Username>" + this->username + "</Username>\n";
		s += "		<Password>" + this->password + "</Password>\n";
		 s += "  </Studente>\n";
	  }
	 else if(this->tipo == STUDENTE_RAPPRESENTANTE)
	  {
		s += "      <Tipo>STUDENTE_RAPPRESENTANTE</Tipo>\n";
		s+= "		<ClasseRef id=" + this->classe->getId() + "/>\n";
	    s+= "</Studente>\n";
		s+= "	<Studente id= STU-" + to_string(this->id) + ">\n";
		s += "      <Nome>" + this->nome + "</Nome>\n";
		s += "		<Cognome>" + this->cognome + "</Cognome>\n";
		s += "		<Username>" + this->username + "</Username>\n";
		s += "		<Password>" + this->password + "</Password>\n";
		s += "      <Tipo>STUDENTE_RAPPRESENTANTE</Tipo>\n";
		s+= "		<ClasseRef id=" + this->classe->getId() + "/>\n";
	    s+= "</Studente>\n";
	  }
	  return s; 
	  }
      
};

