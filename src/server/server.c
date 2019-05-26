/**
 * @dir server
 * @brief Dossier code Serveur
 */

/**
 * @file server.c  // TODO: Fill
 *
 * @brief Serveur/Jeu gérant la progression d'un jeu.
 *
 * **Description Here**
 *
 * Fonctionnalités :
 * - **Feature Here**
 *
 * Usage:
 * ```
 * **Usage Here**
 * ```
 *
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @date 20 Mai 2019
 */

#include <stdio.h>
#include <stdlib.h>

static void printHelp(void);

/**
 * @brief Description How2use.  // TODO: Fill
 *
 * Executez le programme avec l'argument --help pour connaitre les arguments.
 *
 * @param argc
 * @param argv
 * @return int exit(0)
 */
int main(int argc, char const *argv[]) {
  // Defauts
  /* Code */  // TODO: Fill

  // Arguments positionnés
  // long x, y;  // TODO: Fill
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
  /* Code */  // TODO: Fill

  // Traitement
  /* Code */  // TODO: Fill

  // Output
  /* Code */  // TODO: Fill

  return 0;
}

/**
 * @brief Affiche l'aide.
 *
 */
static void printHelp(void) {
  printf(  // TODO: Fill
      "{TITRE} par Marc NGUYEN et Thomas LARDY en {DATE}\n\
\n\
Usage: {EXECUTABLE} <x> <y> <cote> [options...]\n\n\
{CATEGORIE} Options:\n\
  -c,  --commande         {DESCRIPTION}                            [défaut: 100]\n\
\n\
Autres:\n\
  -h,  --help             Affiche ce dialogue\n");
  exit(0);
}
