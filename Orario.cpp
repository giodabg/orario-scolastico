#include "Orario.h"

void toCSV() {}
void fromCSV(std::string rigaCSV){}

void toXML(){
    cout << "<Orario>\n";
    cout << "  <Lezioni>\n";
    for (const auto& l : lezioni) {
        cout << "    <Lezione>" << l.info << "</Lezione>\n";
    }
    cout << "  </Lezioni>\n";
    cout << "  <CodaProposte count=\"" << codaProposte.size() << "\"/>\n";
    cout << "</Orario>" << endl;
}

void FromXML(string NomeFile)
{
   try
{
    if (!File.Exists(NomeFile))
    {
        cout << NomeFile + " non trovato";
        return;
    }

    using StreamReader sr = new StreamReader(NomeFile);
    var risultato = myXML.Deserialize(sr) as List<Orario>;
    if (risultato == null)
    {
        cout << "XML non valido";
        return;
    }

    Orario = risultato;
    refreshList();
}
catch (Exception ex)
{
    cout << "ERRORE:\n" + ex.Message;
}
}

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
