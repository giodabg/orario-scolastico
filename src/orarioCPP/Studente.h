#pragma once
#include <iostream>
#include <string>
#include "Utente.h"
using namespace std;

class Studente : public Utente {
private:
    string tipo;      //normale o rappresentante
    int classeRef; //ID della classe (es. "CLA-3A")

public:
    //restituisce il valore dell'attributo: tipo
    string getTipoUtente() override;

    //netodo toString
    string toString() override;

    //esporta i dati in un file CSV
    string toCSV() override;

    //prende i dati da un CSV
    void fromCSV(string riga) override;


    //esporta i dati in un file XML
    string toXML();

    //prende i dati da file XML
    void fromXML(string xml) override;

};
