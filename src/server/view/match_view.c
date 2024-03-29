/**
 * @file match_view.c
 *
 * @brief Fonctions l'affichage du match.
 *
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @date 20 Mai 2019
 */

#include "match_view.h"

#include <stdio.h>

/// Affiche la liste de matches et les états.
void afficherListeMatches(struct ListeMatches* liste_matches) {
  char* state;

  for (long i = 0; i < liste_matches->nb_matches; i++) {
    switch (liste_matches->matches[i]->etat) {
      case NOT_STARTED:
        state = "NOT STARTED";
        break;
      case STARTED:
        state = "STARTED";
        break;
      case ENDED:
        state = "ENDED";
        break;
    }
    printf("[DEBUG THREAD] %s VS %s: %s\n",
           liste_matches->matches[i]->joueur[0]->pseudo,
           liste_matches->matches[i]->joueur[1]->pseudo, state);
  }
}
