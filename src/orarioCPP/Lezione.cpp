#include "GiornoSettimana.h"
#include "Classe.h"
#include "Aula.h"
#include "Materia.h"
#include "tinyxml2.h"
#include <iostream>

using namespace std;
class Lezione {
    private:
        GiornoSettimana giorno;
        int ora; 
        vector docenti; 
        Classe* classe; /
        Aula* aula; 
        Materia* materia; 

    public:
        Lezione();
        Lezione(GiornoSettimana giorno, int ora, vector docenti, Classe* classe, Aula* aula, Materia* materia);
        ~Lezione();

        // Verifica se due lezioni nello stesso giorno occupano la stessa ora
        bool Sovrapposta(const Lezione& altra){
            // Controlla se le lezioni sono nello stesso giorno e alla stessa ora
            if (giorno == altra.giorno && ora == altra.ora) {
                return true;
            }
            return false;
        };             
        // Sposta la lezione in un altra ora
        void spostaLezione(std::string nuovoGiorno, int nuovaOra){
            giorno.setGiorno(nuovoGiorno);
            ora = nuovaOra;
        };  
        // Cambia l'aula della lezione
        void assegnaAula(std::string idNuovaAula){
            aula.setId(idNuovaAula);
        };                  
        // Aggiunge un docente
        void aggiungiDocente(Docente* d){
            docenti.add(d); // Aggiunge il docente alla lista
        };                           
        // Rimuove un docente dalla lezione
        void rimuoviDocente(std::string idDocente){
            docenti.delete(idDocente); 
        };  

        void toString();
        void toCSV();
        void fromCSV();
        void toXML(){
            cout << "<Lezione>" << endl;
            cout << "    <Giorno>" << toString(giorno) << "</Giorno>" << endl;
            cout << "    <Ora>" << ora << "</Ora>" << endl;
            cout << "    <MateriaRef id=\"" << materia.getId() << "\"/>" << endl;
            cout << "    <AulaRef id=\"" << aula.getId() << "\"/>" << endl;
            cout << "    <ClasseRef id=\"" << classe.getId() << "\"/>" << endl;
            cout << "    <Docenti>" << endl;
            for (int i = 0; i < docenti.size(); i++) { //scrive gli id in base al numero di docentl
                cout << "        <DocenteRef id=\""<< docenti[i].getId()<< "\"/>" << endl;
            }
            cout << "    </Docenti>" << endl;
            cout << "</Lezione>" << endl;
        }
        void fromXML(){            
            //così dovrebbe prendere l'elemento ora, trasformarlo in int e riempire int ora;
            xmlLezione.FirstChildElement("Ora").QueryIntText(&ora);

            //tutte le const char* servono perchè le variabli che abbiamo creato sono puntatori quindo li "tiro fuori" con la libreria ma poi devo passarli creando l'oggetto senno non ha senso
            const char* giornoTesto = xmlLezione.FirstChildElement("Giorno").GetText();
            giorno = new GiornoSettimana();
            giorno.setNome(giornoTesto); //non so i nomi delle funzioni in giornoSettimana

            //materia
            const char* idMat = xmlLezione.FirstChildElement("MateriaRef").Attribute("id");
            materia = new Materia();
            materia.setId(idMat);

            //aula
            const char* idAula = xmlLezione.FirstChildElement("AulaRef").Attribute("id");
            aula = new Aula();
            aula.setId(idAula);

            //classe
            const char* idCla = xmlLezione.FirstChildElement("ClasseRef").Attribute("id");
            classe = new Classe();
            classe.setId(idCla);
            
            //stessa roba di classe.cpp con le lezioni assegnate
            XMLElement* docentiNode = xmlLezione.FirstChildElement("Docenti");
            if (docentiNode) {
                XMLElement* docRef = docentiNode.FirstChildElement("DocenteRef");
                while (docRef) {
                    const char* idDoc = docRef.Attribute("id");
                    Docente* nuovoDocente = new Docente();
                    nuovoDocente.setId(idDoc);
                    docenti.insert(nuovoDocente);
                    docRef = docRef.NextSiblingElement("DocenteRef");
                }    
            }
        }

};
