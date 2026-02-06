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
    return string();
}

string Studente::toXML()
{
    string xml = "  <Studente id=\"" + id + "\">\n";
    xml += "    <Nome>" + nome + "</Nome>\n";
    xml += "    <Cognome>" + cognome + "</Cognome>\n";
    xml += "    <Username>" + username + "</Username>\n";
    xml += "    <Password>" + password + "</Password>\n";
    xml += "    <Tipo>" + tipo + "</Tipo>\n";
    xml += "    <ClasseRef id=\"" + classeRef + "\"/>\n";
    xml += "  </Studente>";
    return xml;
}

void Studente::fromCSV(string riga)
{
}

void Studente::fromXML(string xml)
{
}
