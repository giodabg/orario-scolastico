#include <string>

using namespace std;

// Funzione per estrarre l'ID numerico da una stringa del tipo "Aula-123"
int estraiIdNumerico(const char* idCompleto) {
    string idStr(idCompleto);
    size_t pos = idStr.find('-');

    if (pos != string::npos) {
        string numStr = idStr.substr(pos + 1);
        return stoi(numStr);
    }

    return -1;  // Errore se non trova il '-'
}