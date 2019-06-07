/**
 * @file template_test.c
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @brief Lancer des tests unitaires sur XXXX. TODO: Fill
 * @date 19 Mai 2019
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../src/server/view/joueur_view.h"

/**
 * @brief Tests unitaires.
 *
 * @return int Exit 0.
 */
int main(void) {
  printf("joueur_view_test:\n");  // File
  printf("  Score:\n");           // Group
  struct Joueur* joueur = creerJoueur("pseudo");
  struct ListeJoueurs* liste_joueurs = creerListeJoueurs();
  ajouterJoueur(liste_joueurs, joueur);
  joueur->score = 10000;
  printf("    \x1B[33m-- MANUAL VIEW --\x1B[0m\n");
  afficherScoreListeJoueurs(liste_joueurs);
  printf("    \x1B[33m--  END  VIEW  --\x1B[0m\n");
  printf(
      "    afficherScoreListeJoueurs correctly: \x1B[33mMANUAL\x1B[0m\n");  // Pass
  return 0;
}
