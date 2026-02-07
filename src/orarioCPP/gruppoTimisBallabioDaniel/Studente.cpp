#include "Studente.h"

string Studente::getTipoUtente()
{
    return string();
}

string Studente::toString()
{
    return string();
}

string Studente::toCSV()
{

    string csv = id + ',';
    csv += nome + ',',;
    csv += cognome + ',';
    csv += username + ',';
    csv += password + ',';
    csv += tipo + ',';
    csv += classeRef;

    return csv;
}

string Studente::toXML() {
    string  xml = "  <Studente id=\"" + id + "\">\n";
            xml += "    <Nome>" + nome + "</Nome>\n";
            xml += "    <Cognome>" + cognome + "</Cognome>\n";
            xml += "    <Username>" + username + "</Username>\n";
            xml += "    <Password>" + password + "</Password>\n";
            xml += "    <Tipo>" + tipo + "</Tipo>\n";
            xml += "    <ClasseRef id=\"" + classeRef + "\"/>\n";
            xml += "  </Studente>";
    
            return xml;
}

void Studente::fromCSV(string riga) {
    stringstream ss(riga);

    getline(ss, this->id, ',');
    getline(ss, this->nome, ',');
    getline(ss, this->cognome, ',');
    getline(ss, this->username, ',');
    getline(ss, this->password, ',');
    getline(ss, this->tipo, ',');
    getline(ss, this->classeRef, ',');
}

void Studente::fromXML(string xml) {
    size_t start, end;

    //estraggo l'ID Studente
    start = xml.find("id=\"");
    if (start != string::npos) {
        start += 4; //sposto dopo id="
        end = xml.find("\"", start);
        this->id = xml.substr(start, end - start);
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

    //estraggo il tipo
    start = xml.find("<Tipo>");
    if (start != string::npos) {
        start += 6;
        end = xml.find("</Tipo>", start);
        this->tipo = xml.substr(start, end - start);
    }

    //estraggo la classeRef
    start = xml.find("<ClasseRef id=\"");
    if (start != string::npos) {
        start += 15; //sposto dopo <ClasseRef id="
        end = xml.find("\"", start);
        this->classeRef = xml.substr(start, end - start);
    }
}