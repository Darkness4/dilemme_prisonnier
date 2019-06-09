/**
 * @file match_thread.c
 *
 * @brief Fonctions permettant le fonctionnement des parties.
 *
 * Chaque match est géré par un thread.
 *
 * - Le match attend d'abord que les joueurs sont PRET2.
 * - Il envoie ensuite une notification et attend un /start
 * - Ensuite se lance la boucle de match.
 * - Chaque joueur a le choix entre /trahir et /coop.
 *
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @date 20 Mai 2019
 */

#include "match_thread.h"

#include <stdio.h>

#include "../error_handler/error_handler.h"
#include "../ligne/ligne.h"
#include "../model/datacontext.h"
#include "../view/joueur_view.h"

/// Thread d'un match
static void *_matchThread(void *val);
/// Vérifie le bon fonctionnement des états lors du match.
static void _checkJOUE(struct Match *match);
/// Vérifie le bon fonctionnement des choix lors du match.
static void _checkCHOIX(struct Match *match);
/// Envoyer du texte aux deux joueurs du Match.
static void _printTo2(struct Joueur **joueurs, char *text);

/**
 * @brief Créer tout les workers pour chaque matches.
 *
 * @param liste_matches Pointer vers la liste de matches.
 */
void creerMatchWorkers(struct ListeMatches *liste_matches) {
  for (long i = 0; i < liste_matches->nb_matches; i++) {
    if (pthread_create(&liste_matches->matches[i]->pthread_id, NULL,
                       _matchThread, liste_matches->matches[i]) != 0)
      erreur_IO("pthread_create");
  }
}

/**
 * @brief Attends la fin de chaque match.
 *
 * @param liste_matches Pointer vers la liste de matches.
 */
void joinMatchWorkers(struct ListeMatches *liste_matches) {
  // TODO: Afficher ce que ressorte les threads
  for (long i = 0; i < liste_matches->nb_matches; i++) {
    if (pthread_join(liste_matches->matches[i]->pthread_id, NULL) != 0)
      erreur_IO("pthread_join");
  }
}

static void *_matchThread(void *val) {
  struct Match *match = (struct Match *)val;
  char buf[BUFSIZ];

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
  int lgEcr =
      ecrireLigne(match->joueur[0]->canal, "Un match est disponible !\n");
  lgEcr += ecrireLigne(match->joueur[0]->canal, "CMDS: /start /quit\n");
  if (lgEcr <= -1) erreur_IO("ecrireLigne");
  lgEcr = ecrireLigne(match->joueur[1]->canal, "Un match est disponible !\n");
  lgEcr += ecrireLigne(match->joueur[1]->canal, "CMDS: /start /quit\n");
  if (lgEcr <= -1) erreur_IO("ecrireLigne");

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

  while (match->round_count < CONFIG.MAX_ROUND) {
    printf("[DEBUG] %s VS %s: Round %i start !\n", match->joueur[0]->pseudo,
           match->joueur[1]->pseudo, match->round_count);
    printf("[DEBUG] %s VS %s: Waiting for decisions...\n",
           match->joueur[0]->pseudo, match->joueur[1]->pseudo);
    _printTo2(match->joueur, "Souhaitez-vous trahir ou coopérer ?\n");
    _printTo2(match->joueur, "CMDS: /trahir /coop /quit\n");

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
      sprintf(buf, "%s a trahi %s!\n", match->joueur[0]->pseudo,
              match->joueur[1]->pseudo);
      _printTo2(match->joueur, buf);
    } else if (match->joueur[0]->choix == COOPERER &&
               match->joueur[1]->choix == TRAHIR) {
      printf("[DEBUG] %s VS %s: %s a trahi %s!\n", match->joueur[0]->pseudo,
             match->joueur[1]->pseudo, match->joueur[1]->pseudo,
             match->joueur[0]->pseudo);
      sprintf(buf, "%s a trahi %s!\n", match->joueur[1]->pseudo,
              match->joueur[0]->pseudo);
      _printTo2(match->joueur, buf);
    } else if (match->joueur[0]->choix == COOPERER &&
               match->joueur[1]->choix == COOPERER) {
      printf("[DEBUG] %s VS %s: Une belle coopération entre deux joueurs !\n",
             match->joueur[0]->pseudo, match->joueur[1]->pseudo);
      _printTo2(match->joueur, "Une belle coopération entre deux joueurs !\n");
    } else if (match->joueur[0]->choix == TRAHIR &&
               match->joueur[1]->choix == TRAHIR) {
      printf("[DEBUG] %s VS %s: Une belle trahison entre deux joueurs !\n",
             match->joueur[0]->pseudo, match->joueur[1]->pseudo);
      _printTo2(match->joueur, "Une belle trahison entre deux joueurs !\n");
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
  match->joueur[1]->etat = ATTENTE;
  _printTo2(match->joueur, "/pret pour chercher un match...\n");
  _printTo2(match->joueur, "CMDS: /pret /quit\n");

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
    match->joueur[0]->score += CONFIG.SCORE_2TRAHIR;
    match->joueur[1]->score += CONFIG.SCORE_2TRAHIR;
  } else if (match->joueur[0]->choix == COOPERER &&
             match->joueur[1]->choix == TRAHIR) {
    match->joueur[0]->score -= CONFIG.SCORE_TC;
    match->joueur[1]->score += CONFIG.SCORE_TC;
  } else if (match->joueur[0]->choix == TRAHIR &&
             match->joueur[1]->choix == COOPERER) {
    match->joueur[0]->score += CONFIG.SCORE_TC;
    match->joueur[1]->score -= CONFIG.SCORE_TC;
  } else if (match->joueur[0]->choix == COOPERER &&
             match->joueur[1]->choix == COOPERER) {
    match->joueur[0]->score += CONFIG.SCORE_2COOP;
    match->joueur[1]->score += CONFIG.SCORE_2COOP;
  } else {
    char errout[100];
    sprintf(errout, "Le match %s VS %s a reçu un choix inattendu !\n",
            match->joueur[0]->pseudo, match->joueur[1]->pseudo);
    erreur_pthread_IO(errout);
  }
}

static void _printTo2(struct Joueur **joueurs, char *text) {
  int lgEcr = ecrireLigne(joueurs[0]->canal, text);
  if (lgEcr <= -1) erreur_IO("ecrireLigne");
  lgEcr = ecrireLigne(joueurs[1]->canal, text);
  if (lgEcr <= -1) erreur_IO("ecrireLigne");
}
