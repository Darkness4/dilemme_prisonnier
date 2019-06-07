/**
 * @file joueur_test.c
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @brief Lancer des tests unitaires sur les manipulations de Joueur.
 * @date 19 Mai 2019
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../src/server/model/joueur.h"

/**
 * @brief Tests unitaires.
 *
 * @return int Exit 0.
 */
int main(void) {
  printf("joueur_test:\n");  // File
  printf("  Joueur:\n");     // Group
  struct Joueur* joueur = creerJoueur("pseudo");
  assert(joueur->choix == RIEN);
  assert(joueur->etat == NOT_PRET);
  assert(strcmp(joueur->pseudo, "pseudo") == 0);
  printf("    creerJoueur correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  detruireJoueur(joueur);
  printf("    detruireJoueur correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  joueur = creerJoueur("pseudo");
  struct Joueur* joueur2 = creerJoueur("pseudo2");
  printf("  ListeJoueurs:\n");  // Group
  struct ListeJoueurs* liste_joueurs = creerListeJoueurs();
  assert(liste_joueurs->nb_joueurs == 0);
  assert(liste_joueurs->HEAD == NULL);
  printf("    creerListeJoueurs correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  ajouterJoueur(liste_joueurs, joueur);
  ajouterJoueur(liste_joueurs, joueur2);
  assert(liste_joueurs->HEAD->prev == NULL);
  assert(strcmp(liste_joueurs->HEAD->pseudo, "pseudo2") == 0);
  assert(strcmp(liste_joueurs->HEAD->next->pseudo, "pseudo") == 0);
  assert(strcmp(liste_joueurs->HEAD->next->prev->pseudo, "pseudo2") == 0);
  assert(liste_joueurs->HEAD->next->next == NULL);
  printf("    ajouterJoueur correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  setEtatListeJoueurs(liste_joueurs, PRET1);
  assert(liste_joueurs->HEAD->etat == PRET1);
  assert(liste_joueurs->HEAD->next->etat == PRET1);
  printf("    setEtatListeJoueurs correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  setEtatListeJoueurs(liste_joueurs, NOT_PRET);
  assert(sontPretListeJoueurs(liste_joueurs) == 0);
  setEtatListeJoueurs(liste_joueurs, PRET1);
  assert(sontPretListeJoueurs(liste_joueurs) == 1);
  printf("    sontPretListeJoueurs correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  setScoreListeJoueurs(liste_joueurs, 10000);
  assert(liste_joueurs->HEAD->score == 10000);
  printf("    setScoreListeJoueurs correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  setChoixListeJoueurs(liste_joueurs, TRAHIR);
  assert(liste_joueurs->HEAD->choix == TRAHIR);
  printf("    setChoixListeJoueurs correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  struct ListeJoueurs* copie_liste_joueurs = clonerListeJoueurs(liste_joueurs);
  assert(strcmp(liste_joueurs->HEAD->pseudo,
                copie_liste_joueurs->HEAD->pseudo) == 0);
  assert(strcmp(liste_joueurs->HEAD->next->pseudo,
                copie_liste_joueurs->HEAD->next->pseudo) == 0);
  printf("    clonerListeJoueurs correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  struct Joueur* joueur_found = trouverJoueurParPseudo(liste_joueurs, "pseudo");
  assert(strcmp(joueur_found->pseudo, "pseudo") == 0);
  printf("    trouverJoueurParPseudo correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  char* pseudo = popJoueur(liste_joueurs);
  assert(liste_joueurs->nb_joueurs == 1);
  assert(strcmp(liste_joueurs->HEAD->pseudo, "pseudo") == 0);
  assert(strcmp(pseudo, "pseudo2") == 0);
  assert(liste_joueurs->HEAD->next == NULL);
  assert(liste_joueurs->HEAD->prev == NULL);
  printf("    popJoueur correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  pseudo = popJoueurParPseudo(liste_joueurs, "pseudo");
  assert(strcmp(pseudo, "pseudo") == 0);
  assert(liste_joueurs->nb_joueurs == 0);
  assert(liste_joueurs->HEAD == NULL);
  printf("    popJoueurParPseudo correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  detruireListeJoueurs(liste_joueurs);
  printf("    detruireListeJoueurs correctly: \x1B[32mOK\x1B[0m\n");  // Pass

  return 0;
}
