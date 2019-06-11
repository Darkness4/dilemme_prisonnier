#if !defined(JOUEUR_VIEW_H)
#define JOUEUR_VIEW_H

#include "../model/joueur.h"

void afficherScoreListeJoueurs(struct ListeJoueurs* liste_joueurs);
long indicateurNiveauxJoueurs(struct Joueur* joueur1, struct Joueur* joueur2);
void afficherScoreJoueur(struct Joueur* joueur);
int cmpfonction(const void* a, const void* b);

#endif  // JOUEUR_VIEW_H
