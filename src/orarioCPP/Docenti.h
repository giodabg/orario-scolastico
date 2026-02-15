#pragma once

#include <vector>
#include <string>
#include "Docente.h"

class Docenti {
private:
    std::vector<Docente*> elenco;

public:
    Docenti();
    ~Docenti();

    // Gestione base
    void aggiungiDocente(Docente* d);
    void rimuoviDocente(int index);
    Docente* getDocente(int index) const;
    Docente* getByCognome(const std::string& cognome) const;

    int size() const;
    void clear();

    std::string toString() const;
};
