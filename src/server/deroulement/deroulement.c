/**
 * @file deroulement.c
 *
 * @brief Fonctions permettant le déroulement des parties.
 *
 * Ci-gît le deroulement des parties. Doit être lancé en thread.
 *
 * - Le déroulement se lance dès que tout le monde est PRET1
 * - Ensuite, la liste de matches est généré avec leur thread.
 * - On attend la fin des threads.
 * - Et on finit par affiche le score.
 *
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @date 20 Mai 2019
 */

#include "deroulement.h"

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "../error_handler/error_handler.h"
#include "../model/datacontext.h"
#include "../view/joueur_view.h"
#include "../view/match_view.h"
#include "match_thread.h"

/**
 * @brief Deroulement de chaque partie.
 *
 * @param val DC datacontext.
 * @return void* Exit code.
 */
void* deroulement(void* val) {
  struct DC* datacontext = (struct DC*)val;
  while (1) {
    while (!sontPretListeJoueurs(datacontext->liste_joueurs)) {
      while (!sontPretListeJoueurs(datacontext->liste_joueurs)) continue;

      datacontext->liste_matches = creerListeMatch(datacontext->liste_joueurs);
      setScoreListeJoueurs(datacontext->liste_joueurs, CONFIG.SCORE_DEFAULT);
      for (char i = 0; i < CONFIG.DURATION; i++) {  // Wait 10s
        if (!sontPretListeJoueurs(datacontext->liste_joueurs)) {
          detruireListeMatch(datacontext->liste_matches);
          break;  // Fail
        }
        char text[BUFSIZ];
        sprintf(text, "Serveur> Attendre %li\n", CONFIG.DURATION - i);
        broadcastJoueurs(datacontext->liste_joueurs, text);
        printf("[DEBUG] Attendre %li\n", CONFIG.DURATION - i);
        sleep(1);
      }
    }
    setEtatListeJoueurs(datacontext->liste_joueurs, ATTENTE);
    broadcastJoueurs(datacontext->liste_joueurs,
                     "/pret pour chercher un match...\n");
    broadcastJoueurs(datacontext->liste_joueurs, "CMDS: /pret /quit\n");
    creerMatchWorkers(datacontext->liste_matches);
    afficherListeMatches(datacontext->liste_matches);

    // Traitement
    joinMatchWorkers(datacontext->liste_matches);

    // Output
    broadcastJoueurs(datacontext->liste_joueurs,
                     "Le jeu est terminé. Merci d'y avoir participé !\n");
    afficherScoreListeJoueurs(datacontext->liste_joueurs);
    setEtatListeJoueurs(datacontext->liste_joueurs, NOT_PRET);
  }
  pthread_exit(NULL);
}