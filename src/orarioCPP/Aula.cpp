#include "Aula.h"
#include "libUtils.h"
#include <string>

using namespace std;

Aula* Aula::fromXML(XMLElement* aulaElem) {
	if (aulaElem == nullptr || string(aulaElem->Name()) != "Aula") {
		return nullptr;
	}

	Aula* aula = new Aula();

	// ---- leggo id come attributo
	string idStr;
	const char* idAttr = aulaElem->Attribute("id");
	aula->setId(estraiIdNumerico(idAttr));

	// ---- leggo Nome come elemento
	XMLElement* nomeElem = aulaElem->FirstChildElement("Nome");
	if (nomeElem && nomeElem->GetText()) {
		aula->nome = nomeElem->GetText();
	}

	// ---- leggo Posti come elemento
	XMLElement* postiElem = aulaElem->FirstChildElement("Posti");
	if (postiElem && postiElem->GetText()) {
		aula->posti = atoi(postiElem->GetText());
	}

	// ---- leggo Tipo come elemento
	XMLElement* tipoElem = aulaElem->FirstChildElement("Tipo");
	if (tipoElem && tipoElem->GetText()) {
		string tipoStr = tipoElem->GetText();

		if (tipoStr == "AULA_STANDARD")
			aula->tipo = AULA_STANDARD;
		else if (tipoStr == "AULA_MULTIFUNZIONALE")
			aula->tipo = AULA_MULTIFUNZIONALE;
		else if (tipoStr == "AULA_DEDICATA")
			aula->tipo = AULA_DEDICATA;
	}

	return aula;
}

// ------------------------------------------------------
// Aula::toXML (TinyXML2)
// Struttura prodotta:
//
// <Aula id="204">
//   <Nome>Lab Info</Nome>
//   <Posti>30</Posti>
//   <Tipo>AULA_DEDICATA</Tipo>
//   <MaterieAmmesse>
//     <MateriaRef id="2"/>
//     ...
//   </MaterieAmmesse>
// </Aula>
// ------------------------------------------------------
XMLElement* Aula::toXML(XMLDocument& doc) const {
	XMLElement* aulaElem = doc.NewElement("Aula");
	string idStr = "AUL-"+ to_string(this->id);
	aulaElem->SetAttribute("id", idStr.c_str());

	XMLElement* nomeElem = doc.NewElement("Nome");
	nomeElem->SetText(nome.c_str());
	aulaElem->InsertEndChild(nomeElem);

	XMLElement* postiElem = doc.NewElement("Posti");
	postiElem->SetText(posti);
	aulaElem->InsertEndChild(postiElem);

	XMLElement* tipoElem = doc.NewElement("Tipo");
	tipoElem->SetText(Aula::tipoAulaToString(tipo));
	aulaElem->InsertEndChild(tipoElem);

	return aulaElem;
}

const char* Aula::tipoAulaToString(TipoAula t) {
	switch (t) {
	case AULA_STANDARD:        return "AULA_STANDARD";
	case AULA_MULTIFUNZIONALE: return "AULA_MULTIFUNZIONALE";
	case AULA_DEDICATA:        return "AULA_DEDICATA";
	default:                   return "UNKNOWN";
	}
}

string Aula::escapeXML(const string& s) {
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

string Aula::toXML() const {
	string xml;

	xml += "<Aula id=\"";
	xml += "AUL-" + to_string(id);
	xml += "\">";

	xml += "<Nome>";
	xml += escapeXML(nome);
	xml += "</Nome>";

	xml += "<Posti>";
	xml += to_string(posti);
	xml += "</Posti>";

	xml += "<Tipo>";
	xml += tipoAulaToString(tipo);
	xml += "</Tipo>";

	xml += "</Aula>";
	return xml;
}

string Aula::toString() const {
	string s;

	s += "Aula ID: ";
	s += "AUL-" + to_string(id);
	s += "\n";

	s += "  Nome: ";
	s += nome;
	s += "\n";

	s += "  Posti: ";
	s += to_string(posti);
	s += "\n";

	s += "  Tipo: ";
	s += tipoAulaToString(tipo);
	s += "\n";

	s += "  Materie ammesse: ";

	s += "\n";
	return s;
}
