#include "Orario.h"

void toCSV() {}
void fromCSV(std::string rigaCSV){}

string toXML(){
    std::string xml = << "<Orario>\n"
        << "  <Lezioni>\n";
        for (auto& l : lezioni) xml += "    <Lezione>" << l.info << "</Lezione>\n"
            << "  </Lezioni>\n"
            << "  <CodaProposte count=\"" << codaProposte.size() << "\"/>\n"
            << "</Orario>";
    return xml;

}
void fromXML(){}

void Orario::visualizzaOrarioDocente(Docente nomeDocente)
{
}

void Orario::visualizzaOrarioAula(Aula idAula)
{
}

void Orario::visualizzaOrarioClasse(Classe idClasse)
{
}

void Orario::controllaStatoProposteAttive()
{
}