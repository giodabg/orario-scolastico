#pragma once

#include <iostream>
#include <string>

using namespace std;

class Utente {
protected:
    string id;                 // Identificativo univoco (es. "STU-050")
    string nome;               // Nome dell'utente
    string cognome;            // Cognome dell'utente
    string username;           // Nome utente per il login
    string password;           // Password per il login

public:
    // Restituisce il tipo di utente (es. "STUDENTE", "DOCENTE")
    virtual string getTipoUtente() = 0;

    // Restituisce una rappresentazione testuale leggibile dell'oggetto
    virtual string toString() = 0;

    // Converte l'oggetto in una stringa in formato CSV
    virtual string toCSV() = 0;

    // Ricostruisce l'oggetto a partire da una stringa CSV
    virtual void fromCSV(string riga) = 0;

    string toXML();

    // Ricostruisce l'oggetto a partire da un frammento XML
    virtual void fromXML(string xml) = 0;
};