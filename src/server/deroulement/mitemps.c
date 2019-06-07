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

#include "mitemps.h"

#include <stdlib.h>

#include "match_thread.h"
#include "preparation.h"
#include "../view/joueur_view.h"

void mitemps(struct DC* datacontext) {
  // Chat Global + Commands /quit /pret
  setEtatListeJoueurs(datacontext->liste_joueurs, ATTENTE);
  joinMatchWorkers(datacontext->liste_matches);

  // Output
  afficherScoreListeJoueurs(datacontext->liste_joueurs);
  setEtatListeJoueurs(datacontext->liste_joueurs, NOT_PRET);
}