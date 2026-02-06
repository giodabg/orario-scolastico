#pragma once
#include "Utente.h"

class Studente : public Utente {
private:
    string tipo;      // Specifica se normale o rappresentante
    string classeRef; // Riferimento all'ID della classe (es. "CLA-3A")

public:
    // Restituisce il valore dell'attributo: tipo
    string getTipoUtente() override;

    // Restituisce i dettagli dello studente (nome, cognome, classe)
    string toString() override;

    // Esporta i dati dello studente separati da virgole o punti e virgola
    string toCSV() override;

    // Popola gli attributi analizzando una riga CSV
    void fromCSV(string riga) override;

    string toXML();

    // Analizza un blocco XML per assegnare i valori agli attributi
    void fromXML(string xml) override;
};