#include "GiornoSettimana.h"
#include "Classe.h"
#include "Aula.h"
#include "Materia.h"
#include "tinyxml2.h"
#include <iostream>

using namespace std;


class Classe {
    private:
        int id; // es. 1, 4
        string codice; // es. "3AINF", "5BCHI"
        int numeroStudenti; // es. 22, 27
        vector studenti; // es. [Luigi, Carlo]
        vector lezioni; // es. [Lun1, Lun2]

    public:

        Classe();
        Classe(int id, string codice, int numeroStudenti, vector studenti, vector lezioni);
        ~Classe();

        // Inserisce un nuovo studente (Awais Mohamed)
        void aggiungiStudente(Studente* s){          
            studenti.add(s);
            numeroStudenti++;
        };     
        // Rimuove uno studente (Debabia || Cristain Botezatu Madalin)
        void rimuoviStudente(){
                studenti.remove(s);
                numeroStudenti--;
        };                 
        // Collega una lezione alla classe
        void associaLezione(Lezione* l){
            lezioni.add(l);
        }; 

        void toString();
        void toCSV();
        void fromCSV();
        void toXML(){
            cout << "<Classe id=\"CLA-" << id << "\">" << endl;
            cout << "    <Codice>" << codice << "</Codice>" << endl; //stampe indentate con gli spazi (prob c'è un metodo migliore boh)
            cout << "    <NumeroStudenti>" << numeroStudenti << "</NumeroStudenti>" << endl;
            cout << "    <LezioniAssegnate>" << endl;
            for (int i = 0; i < lezioni.size(); i++) { //stessa roba dei docenti in Lezione.cpp ma cvon lezioni
                cout << "        <LezioneRef id=\""<< lezioni[i].getId() << "\"/>" << endl;
            }
            cout << "    </LezioniAssegnate>" << endl;
            cout << "</Classe>" << endl; 
        }

        void fromXML(){

            id = element.Attribute("id");
            codice = element.FirstChildElement("Codice").GetText();
            element.FirstChildElement("NumeroStudenti").QueryIntText(&this.numeroStudenti);

            //prende il tag lezioniAssegnate
            XMLElement* lezioniNode = element.FirstChildElement("LezioniAssegnate");
            if (lezioniNode) { //comtrolla che esista
                XMLElement* lezRef = lezioniNode.FirstChildElement("LezioneRef"); //prende l'elemento interno (child)
                while (lezRef) {
                    const char* idLez = lezRef.Attribute("id");
                    Lezione* nuovaLezione = new Lezione(); 
                    lezioni.insert(nuovaLezione); //inserisce l'id nel vettore
                    lezRef = lezRef.NextSiblingElement("LezioneRef");  //prende quello dopo e fa ripartire il ciclo, se no si ferma
                }
            }
        }

};