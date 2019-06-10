/**
 * @file datacontext.c
 *
 * @brief Fonctions générant le contexte de données des parties.
 *
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @date 20 Mai 2019
 */

#include "datacontext.h"

#include <stdlib.h>

#include "../error_handler/error_handler.h"

/**
 * @brief Créer et initialise le contexte de données global.
 *
 * @return struct DC* Datacontext.
 */
struct DC* creerDC(void) {
  struct DC* datacontext = (struct DC*)malloc(sizeof(struct DC));
  datacontext->liste_joueurs = creerListeJoueurs();
  datacontext->client_threads = creerClientThreads();
  datacontext->liste_matches = NULL;
  return datacontext;
}
