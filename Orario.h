#include <iostream>
#include <vector>
#include <deque>
#include <string>


class Lezione;
class PropostaModifica;
class Docente;
class Aula;
class Classe;

class Orario {
private:
    std::vector<Lezione> lezioni;
    std::deque<PropostaModifica> codaProposte;

public:
    std::string toString();

    void toCSV();
    void fromCSV(std::string rigaCSV);

    string toXML();
    void fromXML();

    void visualizzaOrarioDocente(Docente nomeDocente);
    void visualizzaOrarioAula(Aula idAula);
    void visualizzaOrarioClasse(Classe idClasse);
    void controllaStatoProposteAttive();
};