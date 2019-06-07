/**
 * @file preparation.c  // TODO: Fill
 *
 * @brief Fonctions permettant la préparation des parties.
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

#include "preparation.h"

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "../../../src/server/view/match_view.h"
#include "../error_handler/error_handler.h"
#include "../model/datacontext.h"
#include "match_thread.h"
#include "mitemps.h"

static const long SCORE_DEFAULT = 1000000;  // TODO: Move to cfg
static const char DURATION = 10;

void* preparation(void *val) {
  struct DC* datacontext = (struct DC*)val;
  while (1) {
    while (!sontPretListeJoueurs(datacontext->liste_joueurs)) {
      while (!sontPretListeJoueurs(datacontext->liste_joueurs)) continue;

      datacontext->liste_matches = creerListeMatch(datacontext->liste_joueurs);
      setScoreListeJoueurs(datacontext->liste_joueurs, SCORE_DEFAULT);
      for (char i = 0; i < DURATION; i++) {  // Wait 10s
        if (!sontPretListeJoueurs(datacontext->liste_joueurs)) {
          detruireListeMatch(datacontext->liste_matches);
          break;  // Fail
        }
        char text[BUFSIZ];
        sprintf(text, "Serveur> Attendre %i\n", DURATION - i);
        broadcastJoueurs(datacontext->liste_joueurs, text);
        printf("[DEBUG] Attendre %i\n", DURATION - i);
        sleep(1);
      }
    }
    setEtatListeJoueurs(datacontext->liste_joueurs, ATTENTE);
    creerMatchWorkers(datacontext->liste_matches);
    afficherListeMatches(datacontext->liste_matches);
    mitemps(datacontext);
  }
  pthread_exit(NULL);
}