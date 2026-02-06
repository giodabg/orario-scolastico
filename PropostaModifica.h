#include <iostream>
#include <vector>
#include <string>

class Utente;
struct CoppiaOre;
enum class TipoModifica { SCAMBIO, PRIMA_VA_IN_SECONDA };
enum class StatoProposta { NESSUN_COMMENTO, APPROVATA };

class PropostaModifica {
private:
    int id;
    Utente* proponente;
    std::vector<CoppiaOre> coppieOre;
    TipoModifica tipo;
    std::string descrizione;
    StatoProposta stato;
    std::string commento;

public:
    std::string toString();

    void toCSV();
    void fromCSV(std::string rigaCSV);

    string toXML();
    void fromXML(std::string nomeFile);

    bool checkConflitto();
};