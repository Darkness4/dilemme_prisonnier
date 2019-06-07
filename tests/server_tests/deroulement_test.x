/**
 * @file deroulement_test.c
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @brief Lancer des tests unitaires sur le Jeu.
 * @date 19 Mai 2019
 *
 * Cas de tests:
 * - Un jeu complet
 * - Un jeu avec abandon
 * - Un jeu avec 3 joueurs complet
 * - 2 jeux avec deux joueurs
 *
 */

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../../src/server/deroulement/preparation.h"
#include "../../src/server/error_handler/error_handler.h"

static void* _testThread1(void* val);
static void* _testThread2(void* val);
static void* _testThread3(void* val);

/**
 * @brief Tests unitaires.
 *
 * @return int Exit 0.
 */
int main(void) {
  printf("deroulement_test:\n");  // File
  printf("  Parties:\n");         // Group
  struct DC* datacontext = creerDC();
  struct Joueur* joueur1 = creerJoueur("marc");
  struct Joueur* joueur2 = creerJoueur("thomas");
  pthread_t simulation_thread;
  ajouterJoueur(datacontext->liste_joueurs, joueur1);
  ajouterJoueur(datacontext->liste_joueurs, joueur2);
  printf("Deux joueurs ont été rajoutté !\n");
  if (pthread_create(&simulation_thread, NULL, _testThread1, datacontext) != 0)
    erreur_IO("pthread_create");
  printf("pthread_create !\n");
  preparation(datacontext);
  printf("Les matches se sont terminées !\n");
  pthread_join(simulation_thread, NULL);
  if (joueur1->score == 0 && joueur2->score == 0)
    printf("    5 rounds trahir correctly: \x1B[32mOK\x1B[0m\n");
  else
    printf("    5 rounds trahir correctly: \x1B[31mFAIL\x1B[0m\n");

  detruireDC(datacontext);
  datacontext = creerDC();
  joueur1 = creerJoueur("marc2");
  joueur2 = creerJoueur("thomas2");
  pthread_t simulation_thread2;
  ajouterJoueur(datacontext->liste_joueurs, joueur1);
  ajouterJoueur(datacontext->liste_joueurs, joueur2);
  printf("Deux joueurs ont été rajoutté !\n");
  if (pthread_create(&simulation_thread2, NULL, _testThread2, datacontext) != 0)
    erreur_IO("pthread_create");
  printf("pthread_create !\n");
  preparation(datacontext);
  printf("Les matches se sont terminées !\n");
  pthread_join(simulation_thread2, NULL);
  if (joueur1->score == 0 && joueur2->score == 0)
    printf("    preparation with /quit correctly: \x1B[32mOK\x1B[0m\n");
  else
    printf("    preparation with /quit correctly: \x1B[31mFAIL\x1B[0m\n");

  detruireDC(datacontext);
  datacontext = creerDC();
  joueur1 = creerJoueur("marc3");
  joueur2 = creerJoueur("thomas3");
  struct Joueur* joueur3 = creerJoueur("florent3");
  pthread_t simulation_thread3;
  ajouterJoueur(datacontext->liste_joueurs, joueur1);
  ajouterJoueur(datacontext->liste_joueurs, joueur2);
  ajouterJoueur(datacontext->liste_joueurs, joueur3);
  printf("Deux joueurs ont été rajoutté !\n");
  if (pthread_create(&simulation_thread3, NULL, _testThread3, datacontext) != 0)
    erreur_IO("pthread_create");
  printf("pthread_create !\n");
  preparation(datacontext);
  printf("Les matches se sont terminées !\n");
  pthread_join(simulation_thread3, NULL);
  if (joueur1->score == -1000000 && joueur2->score == -1000000 &&
      joueur3->score == -1000000)
    printf("    preparation with 3 players correctly: \x1B[32mOK\x1B[0m\n");
  else
    printf("    preparation with 3 players correctly: \x1B[31mFAIL\x1B[0m\n");

  return 0;
}

static void* _testThread1(void* val) {
  struct DC* datacontext = (struct DC*)val;
  printf("2 secondes d'attentes !\n");
  sleep(2);
  printf("fin de l'attente\n");
  setEtatListeJoueurs(datacontext->liste_joueurs, PRET1);
  printf("PRET !\n");
  printf("10 secondes d'attentes !\n");
  sleep(11);
  printf("fin de l'attente\n");
  // Les matchs sont prêts !

  // mitemps
  // /pret
  setEtatListeJoueurs(datacontext->liste_joueurs, PRET2);
  printf("PRET !\n");
  // /start
  setEtatListeJoueurs(datacontext->liste_joueurs, JOUE);
  if (sem_post(&datacontext->liste_matches->matches[0]->state_sem) != 0)
    erreur_pthread_IO("sem_post");
  if (sem_post(&datacontext->liste_matches->matches[0]->state_sem) != 0)
    erreur_pthread_IO("sem_post");
  printf("JOUE !\n");

  // 5 rounds
  for (char i = 0; i < 5; i++) {
    printf("Round %i !\n", i);
    printf("SYNC !\n");

    // /vote
    setChoixListeJoueurs(datacontext->liste_joueurs, TRAHIR);
    if (sem_post(&datacontext->liste_matches->matches[0]->state_sem) != 0)
      erreur_pthread_IO("sem_post");
    if (sem_post(&datacontext->liste_matches->matches[0]->state_sem) != 0)
      erreur_pthread_IO("sem_post");
    printf("TRAHIR !\n");
    printf("Fin round %i !\n", i);
  }

  printf("Fin thread simulation !\n");
  pthread_exit(NULL);
}

static void* _testThread2(void* val) {
  struct DC* datacontext = (struct DC*)val;
  printf("2 secondes d'attentes !\n");
  sleep(2);
  printf("fin de l'attente\n");
  setEtatListeJoueurs(datacontext->liste_joueurs, PRET1);
  printf("PRET !\n");
  printf("10 secondes d'attentes !\n");
  sleep(11);
  printf("fin de l'attente\n");
  // Les matchs sont prêts !

  // mitemps
  // /pret
  setEtatListeJoueurs(datacontext->liste_joueurs, PRET2);
  printf("PRET !\n");
  // /start
  setEtatListeJoueurs(datacontext->liste_joueurs, JOUE);
  if (sem_post(&datacontext->liste_matches->matches[0]->state_sem) != 0)
    erreur_pthread_IO("sem_post");
  if (sem_post(&datacontext->liste_matches->matches[0]->state_sem) != 0)
    erreur_pthread_IO("sem_post");
  printf("JOUE !\n");

  // 2 rounds
  for (char i = 0; i < 2; i++) {
    printf("Round %i !\n", i);

    // /vote
    setChoixListeJoueurs(datacontext->liste_joueurs, TRAHIR);
    if (sem_post(&datacontext->liste_matches->matches[0]->state_sem) != 0)
      erreur_pthread_IO("sem_post");
    if (sem_post(&datacontext->liste_matches->matches[0]->state_sem) != 0)
      erreur_pthread_IO("sem_post");
    printf("TRAHIR !\n");
    printf("Fin round %i !\n", i);
  }

  // 3e round
  printf("Round 3 !\n");
  printf("SYNC !\n");

  // double /quit
  setEtatListeJoueurs(datacontext->liste_joueurs, ELIMINE);
  if (sem_post(&datacontext->liste_matches->matches[0]->state_sem) != 0)
    erreur_pthread_IO("sem_post");
  if (sem_post(&datacontext->liste_matches->matches[0]->state_sem) != 0)
    erreur_pthread_IO("sem_post");
  printf("QUIT !\n");
  printf("Fin round 3 !\n");

  printf("Fin thread simulation !\n");
  pthread_exit(NULL);
}

static void* _testThread3(void* val) {
  struct DC* datacontext = (struct DC*)val;
  struct Joueur* joueur1 =
      trouverJoueurParPseudo(datacontext->liste_joueurs, "marc3");
  struct Joueur* joueur2 =
      trouverJoueurParPseudo(datacontext->liste_joueurs, "thomas3");
  struct Joueur* joueur3 =
      trouverJoueurParPseudo(datacontext->liste_joueurs, "florent3");
  printf("2 secondes d'attentes !\n");
  sleep(2);
  printf("fin de l'attente\n");
  setEtatListeJoueurs(datacontext->liste_joueurs, PRET1);
  printf("PRET !\n");
  printf("10 secondes d'attentes !\n");
  sleep(11);
  printf("fin de l'attente\n");
  // Les matchs sont prêts !

  // mitemps
  // /pret
  joueur1->etat = PRET2;
  printf("Joueur 1 PRET !\n");
  sleep(1);
  joueur2->etat = PRET2;
  printf("Joueur 2 PRET !\n");  // => joueur1 et joueur2 DOIT_ACCEPTER
  sleep(1);
  joueur3->etat = PRET2;
  printf("Joueur 3 PRET !\n");
  sleep(1);

  // Joueur1 VS Joueur2
  // /start
  if (sem_post(&joueur1->match->state_sem) != 0) erreur_pthread_IO("sem_post");
  printf("Joueur 1: /start\n");
  if (sem_post(&joueur2->match->state_sem) != 0) erreur_pthread_IO("sem_post");
  printf("Joueur 2: /start\n");

  // 5 rounds
  for (char i = 0; i < 5; i++) {
    printf("Round %i !\n", i);

    // /vote
    setChoixListeJoueurs(datacontext->liste_joueurs, TRAHIR);
    if (sem_post(&joueur1->match->state_sem) != 0)
      erreur_pthread_IO("sem_post");
    if (sem_post(&joueur2->match->state_sem) != 0)
      erreur_pthread_IO("sem_post");
    printf("TRAHIR !\n");
    printf("Fin round %i !\n", i);
  }

  joueur2->etat = PRET2;
  printf("Joueur 2 PRET !\n");  // Thread J2 VS J3 Active
  sleep(1);
  joueur1->etat = PRET2;
  printf("Joueur 1 PRET !\n");  // Thread J1 VS J3 NOT Active
  sleep(1);
  // Joueur2 VS Joueur3

  if (sem_post(&joueur2->match->state_sem) != 0) erreur_pthread_IO("sem_post");
  if (sem_post(&joueur3->match->state_sem) != 0) erreur_pthread_IO("sem_post");
  printf("Joueur 2 VS Joueur 3 JOUE !\n");

  // 5 rounds
  for (char i = 0; i < 5; i++) {
    printf("Round %i !\n", i);

    // /vote
    setChoixListeJoueurs(datacontext->liste_joueurs, TRAHIR);
    if (sem_post(&joueur2->match->state_sem) != 0)
      erreur_pthread_IO("sem_post");
    if (sem_post(&joueur3->match->state_sem) != 0)
      erreur_pthread_IO("sem_post");
    printf("TRAHIR !\n");
    printf("Fin round %i !\n", i);
  }

  joueur2->etat = PRET2;
  printf("Joueur 2 PRET !\n");  // Do nothing...
  sleep(1);
  joueur3->etat = PRET2;
  printf("Joueur 3 PRET !\n");  // Thread J1 VS J3 Active
  sleep(1);
  // Joueur2 VS Joueur3

  if (sem_post(&joueur1->match->state_sem) != 0) erreur_pthread_IO("sem_post");
  if (sem_post(&joueur3->match->state_sem) != 0) erreur_pthread_IO("sem_post");
  printf("Joueur 1 VS Joueur 3 JOUE !\n");

  // 5 rounds
  for (char i = 0; i < 5; i++) {
    printf("Round %i !\n", i);

    // /vote
    setChoixListeJoueurs(datacontext->liste_joueurs, TRAHIR);
    if (sem_post(&joueur1->match->state_sem) != 0)
      erreur_pthread_IO("sem_post");
    if (sem_post(&joueur3->match->state_sem) != 0)
      erreur_pthread_IO("sem_post");
    printf("TRAHIR !\n");
    printf("Fin round %i !\n", i);
  }

  printf("Fin thread simulation !\n");
  pthread_exit(NULL);
}
