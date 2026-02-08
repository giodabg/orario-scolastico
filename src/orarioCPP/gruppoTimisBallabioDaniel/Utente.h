#pragma once
#include <string>

using namespace std;

class Utente {
    private:
        int id; //id dell'utente
        string nome; //nome dell'utente
        string cognome; //cognome dell'utente
        string username; //username dell'utente
        string password; //password dell'utente
        
    public:
        //metodi getter
        int getId(){return this->id;}
        string getNome(){return this->nome;}
        string getCognome(){return this->cognome;}
        string getUsername(){return this->username;}
        string getPassword(){return this->password;}

        //metodi setter
        void setId(int id){this->id = id;}
        void setNome(string nome){this->nome = nome;}
        void setCognome(string cognome){this->cognome = cognome;}
        void setUsername(string username){this->username = username;}
        void setPassword(string password){this->password = password;}

        //ottiene tipo di utente
        virtual string getTipoUtente() = 0;

        //metodo toString
        virtual string toString();

        //metodo per esportare dati in file CSV
        virtual string toCSV();

        //metodo per importare dati da file CSV
        virtual void fromCSV(string riga);

        //metodo per esportare dati in file XML
        virtual string toXML();

        //metodo per importare dati da file XML
        virtual void fromXML(string xml);
};