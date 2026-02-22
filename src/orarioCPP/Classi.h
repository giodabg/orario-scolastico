#pragma once
#include <string>
#include <vector>
#include "Classe.h"
#include "ContenitoreXML.h"
#include "tinyxml2.h"

// forward declaration (evita include circolare)
class Studenti;

using namespace std;
using namespace tinyxml2;

class Classi : public ContenitoreXML {
private:
    vector<Classe*> items;

public:
    // Costruttore
    Classi();

    // Distruttore (libera memoria)
    ~Classi();

    bool loadFromFile(const string& path) override;
    bool saveToFile(const string& path) const override;
    bool saveToFileOld(const string& path) const;

    void clear() override;
    size_t size() const override;

    // CRUD - CREATE
    bool aggiungi(Classe* classe);

    // CRUD - READ
    Classe* cercaPerId(int id) const;
    Classe* cercaPerAnnoSezione(int anno, char sezione) const;
    Classe* get(size_t index) const;

    // CRUD - UPDATE
    bool modifica(int id, Classe* nuovaClasse);

    // CRUD - DELETE
    bool rimuoviPerId(int id);
    bool rimuoviPerIndex(size_t index);

    // Utility
    bool esisteId(int id) const;
    string toXML() const;
    string toString() const;

    // === RESOLVE ===
    // Collega:
    // - Classe.studentiIds -> Classe.studenti (puntatori)
    // - Studente.classeId  -> Studente.classe (puntatore)
    bool resolve(Studenti& studenti);
};