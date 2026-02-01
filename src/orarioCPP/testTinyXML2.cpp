#include <iostream>
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;


#include <iostream>
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;

int testTinyXML2_1() {
    XMLDocument doc;
    XMLElement* root = doc.NewElement("Test");
    doc.InsertEndChild(root);

    cout << "TinyXML2 funziona!" << endl;

    return 0;
}

int testTinyXML2_2() {

    // CREARE un file XML per l'orario
    XMLDocument doc;

    // Dichiarazione XML
    XMLDeclaration* declaration = doc.NewDeclaration();
    doc.InsertFirstChild(declaration);

    // Elemento root
    XMLElement* root = doc.NewElement("Orario");
    doc.InsertEndChild(root);

    // Aggiungere una lezione
    XMLElement* lezione = doc.NewElement("Lezione");
    lezione->SetAttribute("giorno", "Lunedì");
    lezione->SetAttribute("ora", "8:00-9:00");

    XMLElement* materia = doc.NewElement("Materia");
    materia->SetText("Matematica");
    lezione->InsertEndChild(materia);

    XMLElement* docente = doc.NewElement("Docente");
    docente->SetText("Prof. Rossi");
    lezione->InsertEndChild(docente);

    XMLElement* aula = doc.NewElement("Aula");
    aula->SetText("A101");
    lezione->InsertEndChild(aula);

    root->InsertEndChild(lezione);

    // Salvare il file
    doc.SaveFile("orario.xml");
    cout << "File orario.xml creato!" << endl;

    // LEGGERE il file XML
    XMLDocument docLettura;
    if (docLettura.LoadFile("orario.xml") == XML_SUCCESS) {
        XMLElement* rootLettura = docLettura.FirstChildElement("Orario");
        XMLElement* lezioneLettura = rootLettura->FirstChildElement("Lezione");

        cout << "\nLettura orario:" << endl;
        cout << "Giorno: " << lezioneLettura->Attribute("giorno") << endl;
        cout << "Ora: " << lezioneLettura->Attribute("ora") << endl;
        cout << "Materia: " << lezioneLettura->FirstChildElement("Materia")->GetText() << endl;
        cout << "Docente: " << lezioneLettura->FirstChildElement("Docente")->GetText() << endl;
        cout << "Aula: " << lezioneLettura->FirstChildElement("Aula")->GetText() << endl;
    }

    return 0;
}