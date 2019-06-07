/**
 * @file mitemps.c  // TODO: Fill
 *
 * @brief Fonctions utilisées durant la mitemps des parties.
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

#include "datacontext.h"

#include <stdlib.h>

#include "../error_handler/error_handler.h"

sem_t sem_global;

struct DC* creerDC(void) {
  struct DC* datacontext = (struct DC*)malloc(sizeof(struct DC));
  datacontext->liste_joueurs = creerListeJoueurs();
  datacontext->client_threads = creerClientThreads();
  return datacontext;
}

// void detruireDC(struct DC* datacontext) {
//   detruireClientThreads(datacontext->client_threads);
//   detruireListeJoueurs(datacontext->liste_joueurs);
//   detruireListeMatch(datacontext->liste_matches);
//   free(datacontext);
// }
