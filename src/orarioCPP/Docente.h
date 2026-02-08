#pragma once

#include <string>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Materia {
private:
    //Numero di classi che ha il professore
    int numeroClassi;
    //classi che ha il professore
    List<Aula> classi = new List<Aula>();
    //cognome del professore
    string cognome;
    //account del professore
    Utente utente;
    //orario del professore
    Orario orario;
    //materie che insegna il professore
    List<Materia> materie = new List<Materia>();
public:
    Docente();
    Docente(string cognome, int numeroClassi, Utente utente, Orario orario);

    string getCognome() { return cognome; }
    void setCognome(string n) { cognome = n; }

    int getNumeroClassi() { return numeroClassi; }
    void setNumeroClassi(int n) { numeroClassi = n; }

    Utente getUtente() { return utente; }
    void setUtente(Utente u) { utente = u; }

    Orario getOrario() { return orario; }
    void setOrario(Orario o) { orario = o; }

    List<Aula> getAule() { return classi; }
    void aggiungiAula(Aula a);

    List<Materia> getMaterie() { return materie; }
    void aggiungiMateria(Materia m);

    //Metodi XML
    static Docente* fromXML(XMLElement* docenteElem);
    XMLElement* toXML(XMLDocument& doc);
    string toXML();
    string toString();
    static string escapeXML(string& s);
