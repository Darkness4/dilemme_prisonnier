/**
 * @file match_test.c
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @brief Lancer des tests unitaires sur XXXX. TODO: Fill
 * @date 19 Mai 2019
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../src/server/model/match.h"
#include "../../../src/server/view/match_view.h"

/**
 * @brief Tests unitaires.
 *
 * @return int Exit 0.
 */
int main(void) {
  printf("match_test:\n");  // File
  printf("  Match:\n");     // Group
  struct Joueur* joueur1 = creerJoueur("pseudo");
  struct Joueur* joueur2 = creerJoueur("pseudo2");
  struct Match* match = creerMatch(joueur1, joueur2);
  assert(match->etat == NOT_STARTED);
  assert(match->joueur[0] == joueur1);
  assert(match->joueur[1] == joueur2);
  printf("    creerMatch correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  detruireMatch(match);
  printf("    detruireMatch correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  printf("  ListeMatches:\n");  // Group
  struct Joueur* joueur3 = creerJoueur("pseudo3");
  struct Joueur* joueur4 = creerJoueur("pseudo4");
  struct Joueur* joueur5 = creerJoueur("pseudo5");
  struct ListeJoueurs* liste_joueurs = creerListeJoueurs();
  ajouterJoueur(liste_joueurs, joueur1);
  ajouterJoueur(liste_joueurs, joueur2);
  ajouterJoueur(liste_joueurs, joueur3);
  ajouterJoueur(liste_joueurs, joueur4);
  ajouterJoueur(liste_joueurs, joueur5);
  struct ListeMatches* liste_matches = creerListeMatch(liste_joueurs);
  printf("    \x1B[33m-- MANUAL VIEW --\x1B[0m\n");
  afficherListeMatches(liste_matches);
  printf("    \x1B[33m--  END  VIEW  --\x1B[0m\n");
  printf("    creerListeMatch correctly: \x1B[33mMANUAL\x1B[0m\n");  // Pass
  detruireListeMatch(liste_matches);
  printf("    detruireListeMatch correctly: \x1B[32mOK\x1B[0m\n");  // Pass
  return 0;
}
