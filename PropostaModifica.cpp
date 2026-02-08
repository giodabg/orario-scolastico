si#include "PropostaModifica.h"
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

void PropostaModifica::fromXML(string NomeFile)
{
        try
        {
            if (!File.Exists(NomeFile))
            {
                cout << NomeFile + " non trovato";
                return;
            }

            using StreamReader sr = new StreamReader(NomeFile);
            var risultato = myXML.Deserialize(sr) as List<PropostaModifica>;
            if (risultato == null)
            {
                cout << "XML non valido";
                return;
            }

            PropostaModifica = risultato;
            refreshList();
        }
        catch (Exception ex)
        {
            cout << "ERRORE:\n" + ex.Message;
        }
}

void PropostaModifica::fromXML(std::string nomeFile)
{
}

bool PropostaModifica::checkConflitto()
{
	return false;
}
