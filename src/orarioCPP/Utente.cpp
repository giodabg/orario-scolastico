#include "Utente.h"
#include <sstream>

string Utente::getTipoUtente()
{
    return string();
}

string Utente::toString()
{
    return string();
}

string Utente::toCSV()
{
    char splitter = ',';
    string CSV = to_string(this->id) + splitter;
    CSV += this->nome + splitter;
    CSV += this->cognome + splitter;
    CSV += this->username + splitter;
    CSV += this->password;
}

void Utente::fromCSV(string riga)
{
    stringstream ss(riga);
    string tempID; 

    if (getline(ss, tempID, ',')) {
        this->id = std::stoi(tempID); 
    }

    getline(ss, this->nome, ',');
    getline(ss, this->cognome, ',');
    getline(ss, this->username, ',');
    getline(ss, this->password, ',');
}

string Utente::toXML() {
    string  xml = "  <Utente id=\"" + to_string(this->id) + "\">\n";
    xml += "    <Nome>" + this->nome + "</Nome>\n";
    xml += "    <Cognome>" + this->cognome + "</Cognome>\n";
    xml += "    <Username>" + this->username + "</Username>\n";
    xml += "    <Password>" + this->password + "</Password>\n";
    xml += "  </Utente>";

    return xml;
}

void Utente::fromXML(string xml) {
    size_t start, end;

    //estraggo l'ID Studente
    start = xml.find("id=\"");
    if (start != string::npos) {
        start += 4; //sposto dopo id="
        end = xml.find("\"", start);
        to_string(this->id) = xml.substr(start, end - start);
    }

    //estraggo il nome
    start = xml.find("<Nome>");
    if (start != string::npos) {
        start += 6; //sposto dopo <Nome>
        end = xml.find("</Nome>", start);
        this->nome = xml.substr(start, end - start);
    }

    //estrazione il cognome
    start = xml.find("<Cognome>");
    if (start != string::npos) {
        start += 9; // Sposta dopo <Cognome>
        end = xml.find("</Cognome>", start);
        this->cognome = xml.substr(start, end - start);
    }

    //estraggo lo username
    start = xml.find("<Username>");
    if (start != string::npos) {
        start += 10;
        end = xml.find("</Username>", start);
        this->username = xml.substr(start, end - start);
    }

    //estraggo la password
    start = xml.find("<Password>");
    if (start != string::npos) {
        start += 10;
        end = xml.find("</Password>", start);
        this->password = xml.substr(start, end - start);
    }
}