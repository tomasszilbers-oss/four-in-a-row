# Four in a row (Četri rindā)
Grafiska Windows spēle, kas izstrādāta C++ valodā, izmantojot WinAPI un GDI+. Spēle ietver animētus žetonus, uzvaras noteikšanu, statistikas uzskaiti un ērtu lietotājam saprotamu lietotāja saskarni.
Atbalstīti režīmi: 2 spēlētāji (PvP) un pret datoru (PvE).

---

## Projekta ideja
Projekta mērķis ir izveidot klasisko spēli Connect 4 (Četras rindā) kā pilnvērtīgu Windows lietotni ar modernu renderēšanu un plūstošu animāciju. Spēlētājs ar peli izvēlas kolonnu, žetons “iekrīt” līdz brīvajai vietai, un programma nosaka uzvarētāju vai neizšķirtu.

---

## Lietotāji
- Bērni;
- Ģimenes (vecāki ar bērniem);
- Pusaudži;
- Draugu kompānijas;
- Ikdienas spēlētāji, kas vēlas ātru un vienkāršu spēli (pret cilvēku vai datoru).

## Spēles režīmi
- 2 spēlētāji (PvP) — cilvēks pret cilvēku;
- 1 spēlētājs pret datoru (PvE) — cilvēks pret MI.

---

## Spēles noteikumi
Spēles laukums sastāv no 7 kolonnām un 6 rindām. Spēle paredzēta diviem spēlētājiem vai vienam spēlētājam pret datoru. Žetoni tiek iemesti pārmaiņus.

- žetons vienmēr iekrīt līdz apakšai;
- mērķis — izvietot četrus žetonus rindā:
  - horizontāli,
  - vertikāli,
  - diagonāli (↗ vai ↘);
- spēles gaita:
  1.	Spēlētājs ar peli izvēlas kolonnu.
  2.	Žetons animācijas veidā “iekrīt” vajadzīgajā rindā.
  3.	Tiek pārbaudīts, vai izveidojusies uzvaras līnija.
  4.	Ja kādam spēlētājam ir četri žetoni rindā - uzvara.
  5.	Ja viss laukums ir aizpildīts - neizšķirts.
  Spēle tiek restartēta ar jebkuru peles klikšķi pēc uzvaras vai neizšķirta.

### Spēles noslēgums
- viens spēlētājs iegūst četras rindā — uzvara;
- nav brīvu vietu — neizšķirts;
- pēc spēles ekrāna parādīšanas jebkurš klikšķis restartē spēli.

---

## Funkcionalitāte
- spēles režīma izvēle: PvP vai PvE;
- datora pretinieks (MI) PvE režīmā;
- 7×6 spēles laukums ar grafiskām flīzēm;
- animēta žetona krišana (WM_TIMER);
- uzvaras noteikšana četros virzienos;
- neizšķirta noteikšana;
- peles bultiņas indikators virs kolonnas;
- uzvaras (spēlētājs 1 / spēlētājs 2 vai cilvēks / dators) un neizšķirta ekrāni;
- divkāršā buferizācija — bez mirgošanas;
- spēlētāju kopējās statistikas uzskaite un saglabāšana CSV datubāzē.

---

## Tehnoloģijas
- Platforma: Windows 10/11 (WinAPI + GDI+).
- C++17
- WinAPI
- GDI+
- Double Buffering
- CSV datubāze
- CLion IDE
- CMake
- Git + GitHub

---

## Projekta struktūra
```text
four-in-a-row/
├── src/
│   └── four-in-a-row.cpp
│
├── assets/
│   ├── 4row_board.png
│   ├── 4row_red.png
│   ├── 4row_black.png
│   ├── 4row_arrow.png
│   ├── 4row_humanwinner.png
│   ├── 4row_computerwinner.png
│   └── 4row_tie.png
│
├── stats/
│   └── stats.csv
│
├── tests/
│   └── TestRules.cpp
│
├── docs/
│   ├── diagramma.png
│   └── plans.docx
│
└── README.md
```


---

## Statistikas datubāze (CSV)

Spēlētāju kopējā statistika tiek saglabāta failā stats/stats.csv.
Katram spēlētājam CSV failā ir viena rinda, kas tiek atjaunināta pēc katras spēles.

### CSV kolonnas
| Kolonna | Apraksts |
|--------|----------|
| name | spēlētāja vārds |
| wins | uzvaru skaits |
| losses | zaudējumu skaits |
| ties | neizšķirtu skaits |
| last_played | pēdējās spēles datums un laiks |

### CSV piemērs
```csv
name,wins,losses,ties,last_played
Tomass,5,2,1,2026-01-30 14:22:01
Anna,1,3,0,2026-01-30 14:30:44
```

---

## Instalēšana un palaišana ar CLion

### Nepieciešamās komponentes
CLion lieto toolchains, tādēļ nepieciešams:

- MinGW (ieteicams) - ja izmanto CLion Windows vidē;  
- vai Microsoft Visual C++ Build Tools (ja CLion konfigurēts MSVC izmantošanai).

Tāpat nepieciešams:

- CMake (CLion jau ietver).  
- GDI+ ir daļa no Windows SDK, nekas papildus nav jāinstalē.

---

### Projekta atvēršana CLion
1. Palaid CLion  
2. Izvēlies Open…  
3. Norādi projekta direktoriju Connect4/  
4. Pārliecinies, ka CLion atrod toolchain (MinGW vai MSVC)

---

### Programmas palaišana
1. CLion automātiski ģenerēs CMake konfigurāciju  
2. Nospied Build (Ctrl + F9)  
3. Nospied Run (Shift + F10)

Ja assets neielādējas, pārliecinies, ka CLion executable working directory ir projekta sakne.

---

## Autors
**Tomass Zilbers**  
Projekts izstrādāts individuāli.
