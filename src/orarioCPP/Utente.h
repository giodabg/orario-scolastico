#pragma once
#include <string>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Utente {
protected:
    int id; //id dell'utente
    string nome; //nome dell'utente
    string cognome; //cognome dell'utente
    string username; //username dell'utente
    string password; //password dell'utente

public:
    // Costruttore di default
    Utente();

    //metodi getter
    int getId() const { return this->id; }
    string getNome() const { return this->nome; }
    string getCognome() const { return this->cognome; }
    string getUsername() const { return this->username; }
    string getPassword() const { return this->password; }

    //metodi setter
    void setId(int id) { this->id = id; }
    void setNome(const string& nome) { this->nome = nome; }
    void setCognome(const string& cognome) { this->cognome = cognome; }
    void setUsername(const string& username) { this->username = username; }
    void setPassword(const string& password) { this->password = password; }

    //ottiene tipo di utente
    virtual string getTipoUtente() const;



    //metodo per esportare dati in file XML
    virtual XMLElement* toXML(XMLDocument& doc) const;

    //metodo per importare dati da file XML
    static Utente* fromXML(XMLElement* utenteElem);
    
    //metodo per esportare dati in file CSV
    virtual string toCSV() const;

    //metodo per importare dati da file CSV
    virtual void fromCSV(const string& riga);
    
    // utility
    string toXML() const;
    virtual string toString() const;
};