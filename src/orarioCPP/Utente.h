#pragma once
#include <string>

using namespace std;
class Utente {
protected:
    int id;                 
    string nome;            
    string cognome;         
    string username;        
    string password;        
public:
    virtual string getTipoUtente() = 0;
    virtual void toCSV()=0;
    virtual void fromCSV(string csvLine)=0;
    virtual string toXML() = 0;
    virtual void fromXml(string xmlLine)=0;
    virtual void toString() = 0;
};

