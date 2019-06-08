/**
 * @file joueur_view.c  // TODO: Fill
 *
 * @brief Fonctions permettant l'affichage des données de joueur.
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

#include "joueur_view.h"
#include <stdio.h>
#include "../error_handler/error_handler.h"
#include "../ligne/ligne.h"

/// Affiche le score de tout le monde à tout le monde
void afficherScoreListeJoueurs(struct ListeJoueurs* liste_joueurs) {
  struct Joueur* pJoueur = liste_joueurs->HEAD;
  struct Joueur* pJoueur2 = liste_joueurs->HEAD;
  char ligne_client[BUFSIZ];
  while (pJoueur != NULL) {
    sprintf(ligne_client, "%s: %li\n", pJoueur->pseudo, pJoueur->score);

    // Broadcast
    pJoueur2 = liste_joueurs->HEAD;
    while (pJoueur2 != NULL) {
      ecrireLigne(pJoueur2->canal, ligne_client);
      pJoueur2 = pJoueur2->next;
    }
    pJoueur = pJoueur->next;
  }
}

void afficherScoreJoueur(struct Joueur* joueur) {
  char ligne_client[BUFSIZ];
  sprintf(ligne_client, "%s: %li\n", joueur->pseudo, joueur->score);
  ecrireLigne(joueur->canal, ligne_client);
}

long indicateurNiveauxJoueurs(struct Joueur* joueur1, struct Joueur* joueur2) {
  char ligne_client[BUFSIZ];
  int lgEcr;
  if (joueur1->score > joueur2->score) {
    sprintf(ligne_client, "%s est leader.\n", joueur1->pseudo);
    lgEcr = ecrireLigne(joueur1->canal, ligne_client);
    lgEcr += ecrireLigne(joueur2->canal, ligne_client);
    if (lgEcr <= -1) erreur_IO("ecrireLigne");

  } else if (joueur1->score < joueur2->score) {
    sprintf(ligne_client, "%s est leader.\n", joueur2->pseudo);
    lgEcr = ecrireLigne(joueur1->canal, ligne_client);
    lgEcr += ecrireLigne(joueur2->canal, ligne_client);
    if (lgEcr <= -1) erreur_IO("ecrireLigne");
  } else {
    printf("[DEBUG] %s et %s sont au même niveau.\n", joueur1->pseudo,
           joueur2->pseudo);
    lgEcr =
        ecrireLigne(joueur1->canal, "Les deux joueurs sont au même niveau.\n");
    lgEcr +=
        ecrireLigne(joueur2->canal, "Les deux joueurs sont au même niveau.\n");
    if (lgEcr <= -1) erreur_IO("ecrireLigne");
  }

  return joueur1->score - joueur2->score;
}
