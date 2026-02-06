#include "Utente.h"

string Utente::toXML()
{
    string xml = "  <Utente id=\"" + id + "\">\n";
    xml += "    <Nome>" + nome + "</Nome>\n";
    xml += "    <Cognome>" + cognome + "</Cognome>\n";
    xml += "    <Username>" + username + "</Username>\n";
    xml += "    <Password>" + password + "</Password>\n";
    xml += "  </Utente>";
    return xml;
}
