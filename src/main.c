/**
 * @dir src
 * @brief Dossier code source
 */

/**
 * @file main.c
 *
 * @brief XXXXX
 *
 * DescXXXXX
 *
 * Fonctionnalités :
 * - XXXXX
 *
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @date 20 Mai 2019
 */

#include <stdio.h>
#include <stdlib.h>

void printHelp(void);

/**
 * @brief Execute la simulation d'une propagation, et sort des statistiques.
 *
 * Executez le programme avec l'argument --help pour connaitre les arguments.
 *
 * @param argc
 * @param argv
 * @return int exit(0)
 */
int main(int argc, char const *argv[]) {
    // Defauts
    /* Code */

    // Arguments positionnés
    // long x, y;
    // unsigned long cote;
    // if (argc < 2) {
    //     printf("ERREUR : Pas assez d'arguments.\n\n");
    //     printHelp();
    // }
    // if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
    //     printHelp();
    // if (argc < 4) {
    //     printf("ERREUR : Pas assez d'arguments.\n\n");
    //     printHelp();
    // }
    // sscanf(argv[1], "%li", &x);
    // sscanf(argv[2], "%li", &y);
    // sscanf(argv[3], "%lu", &cote);

    // Arguments nommés (incrémenter i_défaut si arguments positionné)
    // for (int i = 4; i < argc; i++) {
    //     if (!strcmp(argv[i], "-b") || !strcmp(argv[i], "--mort"))
    //         sscanf(argv[i + 1], "%lf", &beta);
    // }

    // Init
    /* Code */

    // Traitement
    /* Code */

    // Output
    /* Code */

    return 0;
}

/**
 * @brief Affiche l'aide.
 *
 */
void printHelp(void) {
    printf(
        "Projet Semestre 6, Propagation d’une épidémie dans une population par Marc NGUYEN et Thomas LARDY en Mar-Apr 2019\n\
\n\
Usage: ProjetS6-MarcNGUYEN-ThomasLARDY <x> <y> <cote> [options...]\n\n\
Population Options:\n\
  -t,  --tours            tours max de la simulation               [défaut: 100]\n\
\n\
Simulation Options Générales:\n\
  -b,  --mort             [0, 1] proba de mourir par la maladie    [défaut: 0.5]\n\
  -g,  --immunise         [0, 1] proba d'être immunise             [défaut: 0.1]\n\
  -la, --malade --incube  [0, 1] proba de contamination            [défaut: 1.0]\n\
\n\
Output Options:\n\
  -od, --data             nom de données brutes               [défaut: data.txt]\n\
  -og, --graph            nom du graphique               [défaut: graphique.txt]\n\
  -ot, --tableau          nom du tableau de bord   [défaut: tableau de bord.txt]\n\
  -li, --limite           limite de char/ligne du graphique ASCII   [défaut: 80]\n\
  -ha, --hauteur          hauteur du graphique ASCII                [défaut: 20]\n\
\n\
Extension Incubation:\n\
  -di, --duree-incube     durée d'une incubation                     [défaut: 4]\n\
\n\
Extension Vaccin et Quarantaine:\n\
  -q,  --quarantaine      [0, 1] proba d'une quarantaine           [défaut: 0.1]\n\
  -dq, --duree-quarantaine  durée d'une quarantaine                 [défaut: 20]\n\
       --cordon           taille du cordon sanitaire                 [défaut: 5]\n\
  -v,  --vaccin           [0, 1] proba de développer un vaccin   [défaut: 0.001]\n\
\n\
Autres:\n\
  -h,  --help             Affiche ce dialogue\n");
    exit(0);
}