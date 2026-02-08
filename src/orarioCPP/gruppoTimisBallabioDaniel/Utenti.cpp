#include "Utenti.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Utenti {
private:
    
    vector<Utente> listaUtenti;

public:
    
    void aggiungi(Utente u) {
        listaUtenti.push_back(u);
    }

    void elimina(string idCercato) {
        for (int i = 0; i < listaUtenti.size(); i++) {
            
            if (listaUtenti[i].id == idCercato) {
                listaUtenti.erase(listaUtenti.begin() + i);
                cout << "Utente rimosso con successo." << endl;
                return;
            }
        }
        cout << "Utente non trovato." << endl;
    }

    void stampaLista() {
        for (int i = 0; i < listaUtenti.size(); i++) {
            cout << listaUtenti[i].toString() << endl;
        }
    }

    void svuotaTutto() {
        listaUtenti.clear();
    }
};