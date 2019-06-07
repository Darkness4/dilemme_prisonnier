/**
 * @file match_thread.c  // TODO: Fill
 *
 * @brief Fonctions permettant le fonctionnement des parties.
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

#include "match_thread.h"

#include <stdio.h>

#include "../error_handler/error_handler.h"
#include "../view/joueur_view.h"

// TODO: Move to cfg
static const long SCORE_TC = 400000;        // Trahir = Negatif pour pertes.
static const long SCORE_2COOP = 100000;     // Negatif pour pertes.
static const long SCORE_2TRAHIR = -200000;  // Negatif pour pertes.
static const long MAX_ROUND = 5;

static void *_matchThread(void *val);
static void _checkJOUE(struct Match *match);
static void _checkCHOIX(struct Match *match);

void creerMatchWorkers(struct ListeMatches *liste_matches) {
  for (long i = 0; i < liste_matches->nb_matches; i++) {
    if (pthread_create(&liste_matches->matches[i]->pthread_id, NULL,
                       _matchThread, liste_matches->matches[i]) != 0)
      erreur_IO("pthread_create");
  }
}

void joinMatchWorkers(struct ListeMatches *liste_matches) {
  // TODO: Afficher ce que ressorte les threads
  for (long i = 0; i < liste_matches->nb_matches; i++) {
    if (pthread_join(liste_matches->matches[i]->pthread_id, NULL) != 0)
      erreur_IO("pthread_join");
  }
}

static void *_matchThread(void *val) {
  struct Match *match = (struct Match *)val;

  printf("[DEBUG] %s VS %s: Alive !\n", match->joueur[0]->pseudo,
         match->joueur[1]->pseudo);

  // Le producteur est désormais Match
  while (match->joueur[0]->etat != PRET2 || match->joueur[1]->etat != PRET2)
    continue;

  match->joueur[0]->etat = DOIT_ACCEPTER;
  match->joueur[1]->etat = DOIT_ACCEPTER;
  match->joueur[0]->match = match;
  match->joueur[1]->match = match;
  match->joueur[0]->id_joueur_match = 0;
  match->joueur[1]->id_joueur_match = 1;

  printf("[DEBUG] %s VS %s: Les joueurs doivent accepter !\n",
         match->joueur[0]->pseudo, match->joueur[1]->pseudo);

  // Attend que les clients ont terminées (ecrire "/start" ou "/quit")
  // Le status doit passer à JOUE à partir de SessionClient
  if (sem_wait(&match->state_sem) != 0) erreur_pthread_IO("sem_wait");
  if (sem_wait(&match->state_sem) != 0) erreur_pthread_IO("sem_wait");

  _checkJOUE(match);

  printf("[DEBUG] %s VS %s: /start received !\n", match->joueur[0]->pseudo,
         match->joueur[1]->pseudo);

  indicateurNiveauxJoueurs(match->joueur[0], match->joueur[1]);

  match->etat = STARTED;

  while (match->round_count < MAX_ROUND) {
    printf("[DEBUG] %s VS %s: Round %i start !\n", match->joueur[0]->pseudo,
           match->joueur[1]->pseudo, match->round_count);
    printf("[DEBUG] %s VS %s: Waiting for decisions...\n",
           match->joueur[0]->pseudo, match->joueur[1]->pseudo);

    // Les clients handlers sont en train de travailler...

    // Attend que les clients ont terminées (ecrire "/vote" ou "/quit")
    if (sem_wait(&match->state_sem) != 0) erreur_pthread_IO("sem_wait");
    if (sem_wait(&match->state_sem) != 0) erreur_pthread_IO("sem_wait");

    _checkJOUE(match);
    _checkCHOIX(match);
    printf("[DEBUG] %s VS %s: Choice has been succefully treated !\n",
           match->joueur[0]->pseudo, match->joueur[1]->pseudo);
    if (match->joueur[0]->choix == TRAHIR &&
        match->joueur[1]->choix == COOPERER) {
      printf("[DEBUG] %s VS %s: %s a trahi %s!\n", match->joueur[0]->pseudo,
             match->joueur[1]->pseudo, match->joueur[0]->pseudo,
             match->joueur[1]->pseudo);
    } else if (match->joueur[0]->choix == COOPERER &&
               match->joueur[1]->choix == TRAHIR) {
      printf("[DEBUG] %s VS %s: %s a trahi %s!\n", match->joueur[0]->pseudo,
             match->joueur[1]->pseudo, match->joueur[1]->pseudo,
             match->joueur[0]->pseudo);
    } else if (match->joueur[0]->choix == COOPERER &&
               match->joueur[1]->choix == COOPERER) {
      printf("[DEBUG] %s VS %s: Une belle coopération entre deux joueurs !\n",
             match->joueur[0]->pseudo, match->joueur[1]->pseudo);
    } else if (match->joueur[0]->choix == TRAHIR &&
               match->joueur[1]->choix == TRAHIR) {
      printf("[DEBUG] %s VS %s: Une belle trahison entre deux joueurs !\n",
             match->joueur[0]->pseudo, match->joueur[1]->pseudo);
    } else {
      erreur_pthread_IO(
          "Ce message d'erreur ne devrait pas s'afficher si _checkCHOIX "
          "fonctionne parfaitement.\n");
    }

    match->joueur[0]->choix = RIEN;
    match->joueur[1]->choix = RIEN;

    printf("[DEBUG] %s VS %s: Round %i ended !\n", match->joueur[0]->pseudo,
           match->joueur[1]->pseudo, match->round_count);

    afficherScoreJoueur(match->joueur[0]);
    afficherScoreJoueur(match->joueur[1]);

    match->round_count++;
  }
  printf("[DEBUG] Fin match !\n");

  match->joueur[0]->etat = ATTENTE;
  sem_post(&match->joueur[0]->notification_sem);
  match->joueur[1]->etat = ATTENTE;
  sem_post(&match->joueur[1]->notification_sem);
  match->etat = ENDED;

  // Fin de Round
  printf("[DEBUG] Thread %s VS %s is now dead !\n", match->joueur[0]->pseudo,
         match->joueur[1]->pseudo);

  sem_destroy(&match->state_sem);
  pthread_exit(NULL);
}

static void _checkJOUE(struct Match *match) {
  if (match->joueur[0]->etat == ELIMINE && match->joueur[1]->etat == JOUE) {
    printf("[DEBUG] %s est éliminé !\n", match->joueur[0]->pseudo);
    match->joueur[1]->score += match->joueur[0]->score;
    match->joueur[0]->score = 0;
    match->etat = ENDED;

    pthread_exit(NULL);

  } else if (match->joueur[0]->etat == JOUE &&
             match->joueur[1]->etat == ELIMINE) {
    printf("[DEBUG] %s est éliminé !\n", match->joueur[1]->pseudo);
    match->joueur[0]->score += match->joueur[1]->score;
    match->joueur[1]->score = 0;
    match->etat = ENDED;

    pthread_exit(NULL);

  } else if (match->joueur[0]->etat == ELIMINE &&
             match->joueur[1]->etat == ELIMINE) {
    printf("[DEBUG] %s VS %s : Ah ben bravo...\n", match->joueur[0]->pseudo,
           match->joueur[1]->pseudo);
    match->joueur[0]->score = 0;
    match->joueur[1]->score = 0;
    match->etat = ENDED;

    pthread_exit(NULL);

  } else if (match->joueur[0]->etat != JOUE && match->joueur[1]->etat != JOUE) {
    char errout[100];
    printf("[ERREUR DEBUG] %s ETAT %i\n", match->joueur[0]->pseudo,
           match->joueur[0]->etat);
    printf("[ERREUR DEBUG] %s ETAT %i\n", match->joueur[1]->pseudo,
           match->joueur[1]->etat);
    sprintf(errout, "%s ou %s possède un état impossible à traiter.\n",
            match->joueur[0]->pseudo, match->joueur[1]->pseudo);
    erreur_pthread_IO(errout);
  }
}

static void _checkCHOIX(struct Match *match) {
  if (match->joueur[0]->choix == TRAHIR && match->joueur[1]->choix == TRAHIR) {
    match->joueur[0]->score += SCORE_2TRAHIR;
    match->joueur[1]->score += SCORE_2TRAHIR;
  } else if (match->joueur[0]->choix == COOPERER &&
             match->joueur[1]->choix == TRAHIR) {
    match->joueur[0]->score -= SCORE_TC;
    match->joueur[1]->score += SCORE_TC;
  } else if (match->joueur[0]->choix == TRAHIR &&
             match->joueur[1]->choix == COOPERER) {
    match->joueur[0]->score += SCORE_TC;
    match->joueur[1]->score -= SCORE_TC;
  } else if (match->joueur[0]->choix == COOPERER &&
             match->joueur[1]->choix == COOPERER) {
    match->joueur[0]->score += SCORE_2COOP;
    match->joueur[1]->score += SCORE_2COOP;
  } else {
    char errout[100];
    sprintf(errout, "Le match %s VS %s a reçu un choix inattendu !\n",
            match->joueur[0]->pseudo, match->joueur[1]->pseudo);
    erreur_pthread_IO(errout);
  }
}
