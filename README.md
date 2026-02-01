# orario-scolastico
Il sistema ha come obiettivo la consultazione e la modifica di un orario scolastico, tenendo conto di:  classi, docenti, aule, discipline.

## Memorizzazione dei dati in formato XML

Per la condivisione dei dati verrà utilizzato il formato XML. Per evitare duplicazioni e ridondanze si dovranno seguire le seguenti regole:

* **1 file per entità** (normalizzate)
* Convenzione ID per ogni entità:
> `MAT-…`, `DOC-…`, `AUL-…`, `CLA-…`, `STU-…`, `LEZ-…`, `ORA-…`, `PRO-…`

---

## 0) Manifest

### `dataset.xml`

```xml
<OrarioDataset version="1.0">
  <Files>
    <File type="Materie"  path="materie.xml"/>
    <File type="Docenti"  path="docenti.xml"/>
    <File type="Classi"   path="classi.xml"/>
    <File type="Studenti" path="studenti.xml"/>
    <File type="Aule"     path="aule.xml"/>
    <File type="Lezioni"  path="lezioni.xml"/>
    <File type="Orari"    path="orari.xml"/>
    <File type="Proposte" path="proposte.xml"/>
  </Files>
</OrarioDataset>
```

---

## 1) Materie

### `materie.xml`

```xml
<Materie>
  <Materia id="MAT-001">
    <Nome>Matematica</Nome>
  </Materia>

  <Materia id="MAT-002">
    <Nome>Informatica</Nome>
  </Materia>

  <Materia id="MAT-003">
    <Nome>Italiano</Nome>
  </Materia>
</Materie>
```

> Nota: per evitare duplicazioni, **non** viene inserita la lista dei docenti che insegnano la materia.
> La relazione Docente↔Materia è dichiarata in `docenti.xml` (e/o derivabile da `lezioni.xml`).

---

## 2) Docenti

### `docenti.xml`

```xml
<Docenti>
  <Docente id="DOC-010">
    <Nome>Luca</Nome>
    <Cognome>Rossi</Cognome>
    <Username>lrossi</Username>
    <Password>pwd123</Password>

    <Tipo>DOCENTE_CLASSICO</Tipo>

    <OreLezione>18</OreLezione>
    <OreADisposizione>2</OreADisposizione>
    <OreTotaliMax>24</OreTotaliMax>

    <MaterieInsegnate>
      <MateriaRef id="MAT-001"/>
      <MateriaRef id="MAT-002"/>
    </MaterieInsegnate>
  </Docente>

  <Docente id="DOC-011">
    <Nome>Sara</Nome>
    <Cognome>Bianchi</Cognome>

    <Username>sbianchi</Username>
    <Password>scuola2026</Password>

    <Tipo>DOCENTE_LABORATORIO</Tipo>

    <OreLezione>12</OreLezione>
    <OreADisposizione>6</OreADisposizione>
    <OreTotaliMax>24</OreTotaliMax>

    <MaterieInsegnate>
      <MateriaRef id="MAT-002"/>
    </MaterieInsegnate>
  </Docente>
</Docenti>
```

---

## 3) Classi

### `classi.xml`

```xml
<Classi>
  <Classe id="CLA-3A">
    <Codice>3A</Codice>
    <NumeroStudenti>25</NumeroStudenti>

    <LezioniAssegnate>
      <LezioneRef id="LEZ-100"/>
      <LezioneRef id="LEZ-101"/>
    </LezioniAssegnate>
  </Classe>

  <Classe id="CLA-5B">
    <Codice>5B</Codice>
    <NumeroStudenti>22</NumeroStudenti>

    <LezioniAssegnate>
      <LezioneRef id="LEZ-102"/>
    </LezioniAssegnate>
  </Classe>
</Classi>
```

---

## 4) Studenti

### `studenti.xml`

```xml
<Studenti>
  <Studente id="STU-050">
    <Nome>Marco</Nome>
    <Cognome>Verdi</Cognome>

    <Username>mverdi</Username>
    <Password>pwd</Password>

    <Tipo>STUDENTE_RAPPRESENTANTE</Tipo>

    <ClasseRef id="CLA-3A"/>
  </Studente>

  <Studente id="STU-051">
    <Nome>Giulia</Nome>
    <Cognome>Neri</Cognome>

    <Username>gneri</Username>
    <Password>pwd2</Password>

    <Tipo>STUDENTE_NORMALE</Tipo>

    <ClasseRef id="CLA-5B"/>
  </Studente>
</Studenti>
```

---

## 5) Aule

### `aule.xml`

```xml
<Aule>
  <Aula id="AUL-101">
    <Nome>Aula 1</Nome>
    <Posti>25</Posti>
    <Tipo>AULA_STANDARD</Tipo>

    <MaterieAmmesse>
      <MateriaRef id="MAT-001"/>
      <MateriaRef id="MAT-003"/>
    </MaterieAmmesse>
  </Aula>

  <Aula id="AUL-204">
    <Nome>Lab Info</Nome>
    <Posti>30</Posti>
    <Tipo>AULA_DEDICATA</Tipo>

    <MaterieAmmesse>
      <MateriaRef id="MAT-002"/>
    </MaterieAmmesse>
  </Aula>
</Aule>
```

---

## 6) Lezioni

### `lezioni.xml`

```xml
<Lezioni>
  <Lezione id="LEZ-100">
    <Giorno>LUNEDI</Giorno>
    <Ora>1</Ora>
    <MateriaRef id="MAT-001"/>
    <AulaRef id="AUL-101"/>
    <ClasseRef id="CLA-3A"/>
    <Docenti>
      <DocenteRef id="DOC-010"/>
    </Docenti>
  </Lezione>

  <Lezione id="LEZ-101">
    <Giorno>LUNEDI</Giorno>
    <Ora>2</Ora>
    <MateriaRef id="MAT-002"/>
    <AulaRef id="AUL-204"/>
    <ClasseRef id="CLA-3A"/>
    <Docenti>
      <DocenteRef id="DOC-010"/>
      <DocenteRef id="DOC-011"/>
    </Docenti>
  </Lezione>

  <Lezione id="LEZ-102">
    <Giorno>MARTEDI</Giorno>
    <Ora>3</Ora>
    <MateriaRef id="MAT-002"/>
    <AulaRef id="AUL-204"/>
    <ClasseRef id="CLA-5B"/>
    <Docenti>
      <DocenteRef id="DOC-011"/>
    </Docenti>
  </Lezione>
</Lezioni>
```

---

## 7) Orari

### `orari.xml`

```xml
<Orari>
  <Orario id="OR-001">
    <Nome>Orario provvisorio 2025/26</Nome>

    <Lezioni>
      <LezioneRef id="LEZ-100"/>
      <LezioneRef id="LEZ-101"/>
      <LezioneRef id="LEZ-102"/>
    </Lezioni>

    <ProposteInCoda>
      <PropostaRef id="PRO-001"/>
      <PropostaRef id="PRO-002"/>
      <PropostaRef id="PRO-003"/>
    </ProposteInCoda>
  </Orario>
</Orari>
```

---

## 8) Proposte (coda + stato)

### `proposte.xml`

```xml
<Proposte>
  <Coda>
    <Proposta id="PRO-001">
      <Tipo>SCAMBIO</Tipo>
      <Stato>NESSUN_COMMENTO</Stato>

      <UtenteRef type="Studente" id="STU-050"/>
      <Descrizione>Scambio per uscita didattica</Descrizione>

      <CoppieOre>
        <Coppia>
          <Ora giorno="LUNEDI" ora="1"/>
          <Ora giorno="MARTEDI" ora="3"/>
        </Coppia>
      </CoppieOre>
    </Proposta>

    <Proposta id="PRO-002">
      <Tipo>PRIMA_VA_IN_SECONDA</Tipo>
      <Stato>COMMENTO_POSITIVO</Stato>

      <UtenteRef type="Docente" id="DOC-010"/>

      <Descrizione>Spostare per assemblea</Descrizione>
      <Commento>Ok se aula disponibile.</Commento>

      <CoppieOre>
        <Coppia>
          <Ora giorno="LUNEDI" ora="2"/>
          <Ora giorno="VENERDI" ora="4"/>
        </Coppia>
        <Coppia>
          <Ora giorno="VENERDI" ora="4"/>
          <Ora giorno="MERCOLEDI" ora="1"/>
        </Coppia>
        <Coppia>
          <Ora giorno="MERCOLEDI" ora="1"/>
          <Ora giorno="LUNEDI" ora="2"/>
        </Coppia>
      </CoppieOre>
    </Proposta>

    <Proposta id="PRO-003">
      <Tipo>SCAMBIO</Tipo>
      <Stato>RIFIUTATA</Stato>

      <UtenteRef type="Studente" id="STU-051"/>

      <Descrizione>Richiesta non compatibile con verifiche</Descrizione>
      <Commento>Conflitto con docente.</Commento>

      <CoppieOre>
        <Coppia>
          <Ora giorno="MARTEDI" ora="2"/>
          <Ora giorno="MERCOLEDI" ora="2"/>
        </Coppia>
      </CoppieOre>
    </Proposta>
  </Coda>
</Proposte>
```

---

# Regole operative 

Per evitare incoerenze:

1. **Fonte unica per le relazioni “forti”**

   * Le informazioni su *chi/dove/quando* stanno sempre in `lezioni.xml`.

2. **ID non numerici puri**

   * prefissi (`DOC-`, `LEZ-`) per ridurre errori e semplificare il debug.

3. **Coda FIFO = ordine nel file**

   * in `proposte.xml` l’ordine delle entità `<Proposta>` dentro `<Coda>` è FIFO.

---

