#include <string>
#include "libUtils.h"

using namespace std;

// Funzione per estrarre l'ID numerico da una stringa del tipo "Aula-123"
int estraiIdNumerico(const char* idCompleto) {
    if (idCompleto == nullptr) return -1;
    return estraiIdNumerico(string(idCompleto));
}

// Funzione per estrarre l'ID numerico da una stringa del tipo "Aula-123"
int estraiIdNumerico(const string& idCompleto) {
    size_t pos = idCompleto.find('-');

    if (pos != string::npos) {
        string numStr = idCompleto.substr(pos + 1);
        return stoi(numStr);
    }

    return -1;  // Errore se non trova il '-'
}

string escapeXML(const string& s) {
    string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '&') out += "&amp;";
        else if (c == '<') out += "&lt;";
        else if (c == '>') out += "&gt;";
        else if (c == '"') out += "&quot;";
        else if (c == '\'') out += "&apos;";
        else out += c;
    }
    return out;
}