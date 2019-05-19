# Algorithmique et Programmation II

## Projet : Propagation d'une épidémie dans un population avec un structure de graphe

Par Marc NGUYEN et Thomas LARDY.

- [Algorithmique et Programmation II](#algorithmique-et-programmation-ii)
  - [Projet : Propagation d'une épidémie dans un population avec un structure de graphe](#projet--propagation-dune-épidémie-dans-un-population-avec-un-structure-de-graphe)
  - [Fonctionnalités](#fonctionnalités)
  - [Build](#build)
  - [Usage](#usage)
    - [Linux](#linux)
    - [Output](#output)
  - [Structure du programme](#structure-du-programme)
    - [src/](#src)
      - [main.c](#mainc)
      - [population.*](#population)
      - [statistique.*](#statistique)
      - [jouer_un_tour.*](#jouer_un_tour)
    - [tests/](#tests)
      - [jouer_un_tour_test.c](#jouer_un_tour_testc)
      - [population_test.c](#population_testc)
      - [statistique_test.c](#statistique_testc)
    - [docs/](#docs)
    - [Racine ./](#racine-)

## Fonctionnalités

- Simulateur de propagation d'une épidémie
- Période d'incubation
- Vaccination
- Quarantaine
- Graphique et tableau de bord

## Build

```bash
cd ProjetS6-MarcNGUYEN-ThomasLARDY
make -j4  # ou make executable -j4
          # Si tests: make tests -j4
          # Si all: make all -j4
          # Si debug: make debug -j4
          # Egalement, make clean (objs) ou make remove (objs + bin)
```

## Usage

### Linux

Executer le fichier binaire dans le dossier bin.

Par défaut, les paramètres sont les suivant :

```c
unsigned long hauteur = 20;
unsigned long limite = 80;
unsigned duree_incube = 4;
double beta = 0.5;                        // MALADE -> MORT
double gamma = 0.1;                       // MALADE -> IMMUNISE
double lambda = 1.0;                      // SAIN -> MALADE
double chance_quarantaine = 0.1;
double chance_decouverte_vaccin = 0.001;  // MALADE -> VACINE par découverte
char* file_graph = "graphique.txt";
char* file_data = "data.txt";
char* file_tableau = "tableau de bord.txt";
unsigned long tour_max = 100;
int duree_quarantaine = 20;  // Très grand au tps d'incub
int cordon_sanitaire = 5;   // Généralement égal au temps d'incubation
```

Choisissez le patient zero avec x et y, et définissez la taille de la grille
avec cote :

```sh
# Il est conseillé de ne pas travailler dans ./bin/, 
# puisque make remove supprime le dossier
./bin/ProjetS6-MarcNGUYEN-ThomasLARDY <x> <y> <cote> [options...]
```

Pour obtenir de l'aide:

```sh
./bin/ProjetS6-MarcNGUYEN-ThomasLARDY -h
```

```txt
Projet Semestre 6, Propagation d’une épidémie dans une population par Marc NGUYEN et Thomas LARDY en Mar-Apr 2019

Usage: ProjetS6-MarcNGUYEN-ThomasLARDY <x> <y> <cote> [options...]

Population Options:
  -t,  --tours            tours max de la simulation               [défaut: 500]

Simulation Options Générales:
  -b,  --mort             [0, 1] proba de mourir par la maladie    [défaut: 0.5]
  -g,  --immunise         [0, 1] proba d'être immunise             [défaut: 0.1]
  -la, --malade           [0, 1] proba de contamination            [défaut: 1.0]

Output Options:
  -od, --data             nom de données brutes               [défaut: data.txt]
  -og, --graph            nom du graphique               [défaut: graphique.txt]
  -ot, --tableau          nom du tableau de bord   [défaut: tableau de bord.txt]
  -li, --limite           limite de char/ligne du graphique ASCII   [défaut: 80]
  -ha, --hauteur          hauteur du graphique ASCII                [défaut: 20]

Extension Incubation:
  -di, --duree-incube     durée d'une incubation [défaut: 4]

Extension Vaccin et Quarantaine:
  -q,  --quarantaine      [0, 1] proba d'une quarantaine           [défaut: 0.1]
  -dq, --duree-quarantaine  durée d'une quarantaine                 [défaut: 20]
       --cordon           taille du cordon sanitaire                 [défaut: 5]
  -v,  --vaccin           [0, 1] proba de développer un vaccin   [défaut: 0.001]

Autres:
  -h,  --help             Affiche ce dialogue
```

### Output

La sortie se fait sur STDOUT, data.txt, graphique.txt et tableau de bord.txt.

```bash
❯ ./bin/ProjetS6-MarcNGUYEN-ThomasLARDY 0 0 10
----------
o.........
..........
..........
..........
..........
..........
..........
..........
..........
..........
----------

[...]

----------
   $$ *
     *
      $ $$
  $$$$$$$$
 $$$$$$$$$
$$$$$$$$$$
$$$$$$$$$$
$$$$$$$$$$
$$$$$$$$$$
$$$$$$$$$$
----------

Graphique (colonne = tour, charactère = %état)
...$$$$$$$$$$$$$$$$$$$
....$$$$$$$$$$$$$$$$$$
.....$$$$$$$$$$$$$$$$$
.....$$$$$$$$$$$$$$$$$
......$$$$$$$$$$$$$$$$
......$$$$$$$$$$$$$$$$
.......$$$$$$$$$$$$$$$
.......$$$$$$$$$$$$$$$
........$$$$$$$$$$$$$$
........$$$$$$$$$$$$$$
.........$$$$$$$$$$$$$
.........$$$$$$$$$$$$$
..........$$$$$$$$$$$$
........oooouoo $$$$$$
.......ouuuu
......uuuuuu
....uuuuuu
....uuuu
..uu u
.
Exportation...OK
Tableau de bord
-----------------------------------------------------------------
| Tours |   SAIN   |   MORT   |   IMMU   |   VACC   |   Total   |
| ----- | -------- | -------- | -------- | -------- | --------- |
|    23 |        0 |       26 |        2 |       72 |       100 |
|       |   0.00 % |  26.00 % |   2.00 % |  72.00 % |     100 % |
|---------------------------------------------------------------|
| Vit. IMMU max | Vit. INCUB max | Vit. MAL max | Vit. MORT max |
| ------------- | -------------- | ------------ | ------------- |
|             1 |              5 |            3 |             5 |
-----------------------------------------------------------------
```

Le graphe se lit suivant cela:

- IMMUNISE = '*'. Une étoile, parce qu'il est immortel.
- VACCINE = '$'. Un dollar, parce qu'il a payé un vaccin
- SAIN = '.'. Un ., un homme en parfaite santé
- INCUBE = 'u'. Un o incomplet
- MALADE = 'o'. Malade, une bulle qui va pas tarder à éclater
- MORT = ' '. Vide

Le graphique représente le ratio d'un état par rapport aux tours.

Des couleurs sont également associés (Terminal seulement):

- JAUNE : Immusé
- ROUGE : Malade
- VIOLET : Incubé
- VERT : Quarantaine

## Structure du programme

### src/

*Code Source.*

#### main.c

Fichier principal.

#### population.*

Manipulateur de population.

Fonctionnalités :

- Créer
- Cloner
- Afficher
- Faire le patient zero
- Obtenir la taille de la population

#### statistique.*

Gère le stockage de donnée et la récupération de métrique.

Contient:

- Créer une base de donnée
- Ajouter à une base de données
- Exporter une base de données
- Dériver les valeurs de la base de données
- Obtenir des Statistique
- Exporter et afficher un graphique
- Exporter et afficher un tableau de bord

#### jouer_un_tour.*

Lancer un tour.

Fonctionnalités :

- Détecteur de zombie.
- jouerTour lancer un tour.
- Mettre en quarantaine

### tests/

*Tests unitaires.*

#### jouer_un_tour_test.c

- zombiePresent :
  - Faux s'il n'y a pas de MALADE/INCUBE
  - Vrai s'il y a un MALADE/INCUBE
- mettreEnQuarantaine :
  - Le rayon de la quarantaine correspond aux paramètres

#### population_test.c

- creerPopulation:
  - La population créée a la même cote que le paramètre.
  - La grille est saine.
- clonerGrillePopulation:
  - Les pointers grille ne sont pas les même que l'original.
  - Les pointers Personne ne sont pas les même que l'original.
  - Les états sont les mêmes.
- afficherGrillePopulation:
  - (Manuel) Affiche bien le graphique.
- patient_zero:
  - L'état du patient zero est MALADE.
- getTaillePopulation:
  - La taille obtenue correspond à la taille de la population.

#### statistique_test.c

- creerData:
  - tours = 0.
  - La liste de statistiques est définie.
- appendData:
  - tours = 1
  - liste_statistiques[0] est définie.
  - liste_statistiques[0] correspond aux paramètres.
- appendData 2 (MALADE+1, SAIN-1):
  - tours = 2
  - liste_statistiques[1] est définie.
  - liste_statistiques[1] correspond aux paramètres.
  - liste_statistiques[1] ne pointe pas sur les même que liste_statistiques[0].
- deriver:
  - population_totale ne change pas.
  - tours = 1.
  - La dérivée correspond à la différence entre t+1 et t.
- exporter:
  - Exporter data fonctionne.
  - Exporter data_derivee fonctionne.
  - data == data_test.txt
  - data_derivee == data_derivee_test.txt
  - (Manuel) graphique avec hauteur = population et limite = tours
  - (Manuel) graphique avec hauteur = population et limite = tours/2
  - (Manuel) graphique avec hauteur = population et limite = 90

### docs/

Dossier avec la consigne en PDF.

[Documentation Doxygen](http://marc.nguyen.gitlab.emse.fr/projets6-marcnguyen-thomaslardy/)

### Racine ./

- .gitignore : Ignorer ces fichiers pour les futurs commits.
- .gitlab-ci.yml : Intégration Continue sur Gitlab, contient cross-compilation, tests et déploiment de la documentation Doxygen.
- Doxyfile : Paramètres de la documentation Doxygen.
- Makefile : Ensemble de commandes pour générer des fichiers ayant des dépendances.
