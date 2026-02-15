#include "Docenti.h"
#include <sstream>

Docenti::Docenti() {
}

Docenti::~Docenti() {
    clear();
}

void Docenti::aggiungiDocente(Docente* d) {
    if (d != nullptr) {
        elenco.push_back(d);
    }
}

void Docenti::rimuoviDocente(int index) {
    if (index >= 0 && index < elenco.size()) {
        delete elenco[index];
        elenco.erase(elenco.begin() + index);
    }
}

Docente* Docenti::getDocente(int index) const {
    if (index >= 0 && index < elenco.size()) {
        return elenco[index];
    }
    return nullptr;
}

Docente* Docenti::getByCognome(const std::string& cognome) const {
    for (Docente* d : elenco) {
        if (d->getCognome() == cognome) {
            return d;
        }
    }
    return nullptr;
}

int Docenti::size() const {
    return elenco.size();
}

void Docenti::clear() {
    for (Docente* d : elenco) {
        delete d;
    }
    elenco.clear();
}

std::string Docenti::toString() const {
    std::stringstream ss;
    for (Docente* d : elenco) {
        ss << d->toString() << "\n";
    }
    return ss.str();
}
