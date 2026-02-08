#include <iostream>
#include <vector>
#include <string>
#include "Studente.h"

using namespace std;

class Studenti {
private:
    vector<Studente> listaStudenti;

public:

    void aggiungi(Studente s) {
        listaStudenti.push_back(s);
    }

    void elimina(string idDaEliminare) {
        for (int i = 0; i < listaStudenti.size(); i++) {
            if (listaStudenti[i].id == idDaEliminare) {
                listaStudenti.erase(listaStudenti.begin() + i);

                cout << "Studente rimosso correttamente." << endl;
                return;
            }
        }
        cout << "Studente con ID " << idDaEliminare << " non trovato." << endl;
    }

    void stampaLista() {
        for (int i = 0; i < listaStudenti.size(); i++) {
            cout << listaStudenti[i].toString() << endl;
        }
    }

    void svuotaTutto() {
        listaStudenti.clear();
    }
};