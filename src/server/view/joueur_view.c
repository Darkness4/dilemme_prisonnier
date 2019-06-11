/**
 * @file joueur_view.c
 *
 * @brief Fonctions permettant l'affichage des données de joueur.
 *
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @date 20 Mai 2019
 */

#include "joueur_view.h"

#include <stdio.h>
#include <stdlib.h>

#include "../error_handler/error_handler.h"
#include "../ligne/ligne.h"

/// Affiche le score de tout le monde à tout le monde
void afficherScoreListeJoueurs(struct ListeJoueurs* liste_joueurs) {
  struct Joueur* pJoueur = liste_joueurs->HEAD;
  unsigned char nb_joueurs = liste_joueurs->nb_joueurs;
  char ligne_client[BUFSIZ];
  struct Joueur* liste_tri[nb_joueurs];

  for (int i = 0; i < nb_joueurs; i++) {
    liste_tri[i] = pJoueur;
    pJoueur = pJoueur->next;
  }

  qsort(liste_tri, nb_joueurs, sizeof(struct Joueur*), cmpfonction);
  for (int i = 0; i < nb_joueurs; i++) {
    sprintf(ligne_client, "%s: %li\n", liste_tri[nb_joueurs - 1 - i]->pseudo,
            liste_tri[nb_joueurs - 1 - i]->score);
    // Broadcast
    broadcastJoueurs(liste_joueurs, ligne_client);
  }
}

int cmpfonction(const void* a, const void* b) {
  struct Joueur* joueur1 = (struct Joueur*)a;
  struct Joueur* joueur2 = (struct Joueur*)b;
  return ((joueur1->score) - (joueur2->score));
}

/// Affiche son propre score à lui-même.
void afficherScoreJoueur(struct Joueur* joueur) {
  char ligne_client[BUFSIZ];
  sprintf(ligne_client, "%s: %li\n", joueur->pseudo, joueur->score);
  ecrireLigne(joueur->canal, ligne_client);
}

/// Indique si l'un des joueurs est supérieur à l'autre et vice versa.
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
    lgEcr = ecrireLigne(joueur1->canal,
                        "Serveur> Les deux joueurs sont au même niveau.\n");
    lgEcr += ecrireLigne(joueur2->canal,
                         "Serveur> Les deux joueurs sont au même niveau.\n");
    if (lgEcr <= -1) erreur_IO("ecrireLigne");
  }

  return joueur1->score - joueur2->score;
}
