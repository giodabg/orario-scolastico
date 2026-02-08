#include "PropostaModifica.h"
#include "Orario.h"
using namespace std;

std::string PropostaModifica::toString()
{
	return std::string();
}

void PropostaModifica::toCSV()
{
}

void PropostaModifica::fromCSV(std::string rigaCSV)
{
}

string PropostaModifica::toXML()
{
    std::string xml = "<PropostaModifica>\n" 
     << "  <id>" + id << "</id>\n"
     << "  <descrizione>" << descrizione << "</descrizione>\n"
     << "  <stato>" << (int)stato << "</stato>\n"
     << "  <commento>" << commento << "</commento>\n"
     << "</PropostaModifica>";
    return xml;
}

void Orario::fromXML()
{
}

void PropostaModifica::fromXML(std::string nomeFile)
{
}

bool PropostaModifica::checkConflitto()
{
	return false;
}
