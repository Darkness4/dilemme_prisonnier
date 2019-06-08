/**
 * @file client_thread.c  // TODO: Fill
 *
 * @brief Contient les informations à propos du client_thread.
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

#include "client_thread.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "../error_handler/error_handler.h"
#include "../ligne/ligne.h"
#include "../model/datacontext.h"
#include "../view/joueur_view.h"
#include "joueur.h"

static void sessionClient(struct Joueur* joueur);

void* threadSessionClient(void* arg) {
  struct Client_Thread* client_thread = (struct Client_Thread*)arg;

  while (1) {
    if (sem_wait(&client_thread->sem) != 0) erreur_IO("sem_wait");
    client_thread->libre = 0;
    client_thread->joueur->canal = client_thread->canal;

    sessionClient(client_thread->joueur);

    detruireJoueur(client_thread->joueur);

    sem_post(&sem_global);

    client_thread->libre = 1;
  }
}

static void sessionClient(struct Joueur* joueur) {
  int lgLue;
  char ligne_serveur[BUFSIZ];
  int lgEcr = ecrireLigne(
      joueur->canal,
      "Bienvenue sur le jeu du Dilemme du Prisonnier Multijoueur\n");
  lgEcr += ecrireLigne(joueur->canal, "CMDS: /pret /quit\n");
  if (lgEcr <= -1) erreur_IO("ecrireLigne");
  broadcastAutreJoueurs(joueur, "s'est connecté.\n");

  while (joueur->etat != ELIMINE) {
    // Répéter aux autres joueurs
    lgLue = lireLigne(joueur->canal, ligne_serveur);
    if (lgLue < 0) erreur_IO("lireLigne");

    switch (joueur->etat) {
      case NOT_PRET:
        if (strcmp(ligne_serveur, "/quit") == 0) {
          broadcastAutreJoueurs(joueur, "/quit\n");
          printf("[DEBUG STATE] %s est ELIMINE\n", joueur->pseudo);
          joueur->etat = ELIMINE;

        } else if (strcmp(ligne_serveur, "/pret") == 0) {
          broadcastAutreJoueurs(joueur, "/pret\n");
          printf("[DEBUG STATE] %s est PRET1\n", joueur->pseudo);
          joueur->etat = PRET1;
          lgEcr = ecrireLigne(joueur->canal, "CMDS: /!pret /quit\n");
          lgEcr += ecrireLigne(joueur->canal,
                               "Veuillez attendre les autres joueurs...\n");
          if (lgEcr <= -1) erreur_IO("ecrireLigne");
        } else
          broadcastAutreJoueurs(joueur, ligne_serveur);
        break;

      case PRET1:
        if (strcmp(ligne_serveur, "/quit") == 0) {
          broadcastAutreJoueurs(joueur, "/quit\n");
          printf("[DEBUG STATE] %s est ELIMINE\n", joueur->pseudo);
          joueur->etat = ELIMINE;

        } else if (strcmp(ligne_serveur, "/!pret") == 0) {
          broadcastAutreJoueurs(joueur, "/!pret\n");
          printf("[DEBUG STATE] %s est NOT_PRET\n", joueur->pseudo);
          joueur->etat = NOT_PRET;
          lgEcr = ecrireLigne(joueur->canal, "CMDS: /pret /quit\n");
          if (lgEcr <= -1) erreur_IO("ecrireLigne");
        } else
          broadcastAutreJoueurs(joueur, ligne_serveur);
        break;

      case PRET2:
        if (strcmp(ligne_serveur, "/quit") == 0) {
          broadcastAutreJoueurs(joueur, "/quit\n");
          printf("[DEBUG STATE] %s est ELIMINE\n", joueur->pseudo);
          joueur->etat = ELIMINE;

        } else if (strcmp(ligne_serveur, "/!pret") == 0) {
          broadcastAutreJoueurs(joueur, "/!pret\n");
          printf("[DEBUG STATE] %s est ATTENTE\n", joueur->pseudo);
          joueur->etat = ATTENTE;
          lgEcr =
              ecrireLigne(joueur->canal, "/pret pour chercher un match...\n");
          lgEcr += ecrireLigne(joueur->canal, "CMDS: /pret /quit\n");
          if (lgEcr <= -1) erreur_IO("ecrireLigne");
        } else
          broadcastAutreJoueurs(joueur, ligne_serveur);
        break;

      case DOIT_ACCEPTER:
        if (strcmp(ligne_serveur, "/quit") == 0) {
          broadcastAutreJoueurs(joueur, "/quit\n");
          printf("[DEBUG STATE] %s est ELIMINE\n", joueur->pseudo);
          joueur->etat = ELIMINE;
          if (sem_post(&joueur->match->state_sem) != 0)
            erreur_pthread_IO("sem_post");

        } else if (strcmp(ligne_serveur, "/start") == 0) {
          printf("[DEBUG STATE] %s est START\n", joueur->pseudo);
          joueur->etat = JOUE;
          if (sem_post(&joueur->match->state_sem) != 0)
            erreur_pthread_IO("sem_post");
          lgEcr =
              ecrireLigne(joueur->canal, "En attente de l'autre joueur...\n");
          if (lgEcr <= -1) erreur_IO("ecrireLigne");
        } else
          broadcastAutreJoueurs(joueur, ligne_serveur);
        break;

      case ATTENTE:
        if (strcmp(ligne_serveur, "/quit") == 0) {
          broadcastAutreJoueurs(joueur, "/quit\n");
          printf("[DEBUG STATE] %s est ELIMINE\n", joueur->pseudo);
          joueur->etat = ELIMINE;

        } else if (strcmp(ligne_serveur, "/pret") == 0) {
          broadcastAutreJoueurs(joueur, "/pret\n");
          printf("[DEBUG STATE] %s est PRET2\n", joueur->pseudo);
          joueur->etat = PRET2;
          lgEcr = ecrireLigne(joueur->canal, "CMDS: /!pret /quit\n");
          if (lgEcr <= -1) erreur_IO("ecrireLigne");
        } else
          broadcastAutreJoueurs(joueur, ligne_serveur);
        break;

      case ELIMINE:
        break;

      case JOUE:
        if (strcmp(ligne_serveur, "/quit") == 0) {
          broadcastAutreJoueurs(joueur, "/quit\n");
          printf("[DEBUG STATE] %s est ELIMINE\n", joueur->pseudo);
          joueur->etat = ELIMINE;
          if (sem_destroy(&joueur->match->state_sem) != 0)
            erreur_pthread_IO("sem_destroy");

        } else if (strcmp(ligne_serveur, "/trahir") == 0) {
          printf("[DEBUG STATE] %s est TRAHIR\n", joueur->pseudo);
          if (joueur->choix != TRAHIR) {
            joueur->choix = TRAHIR;
            if (joueur->choix != COOPERER)
              if (sem_post(&joueur->match->state_sem) != 0)
                erreur_pthread_IO("sem_post");
          }

        } else if (strcmp(ligne_serveur, "/coop") == 0) {
          printf("[DEBUG STATE] %s est COOPERER\n", joueur->pseudo);
          if (joueur->choix != COOPERER) {
            joueur->choix = COOPERER;
            if (joueur->choix != TRAHIR)
              if (sem_post(&joueur->match->state_sem) != 0)
                erreur_pthread_IO("sem_post");
          }

        } else
          broadcastAutreJoueurs(joueur, ligne_serveur);
        break;
    }
  }

  printf("[CHAT] %s> %s\n", joueur->pseudo, ligne_serveur);

  if (close(joueur->canal) == -1) erreur_IO("fermeture canal");
}

struct Client_Thread** creerClientThreads(void) {
  struct Client_Thread** client_threads = (struct Client_Thread**)malloc(
      sizeof(struct Client_Thread*) * NB_JOUEURS_MAX);
  for (int i = 0; i < NB_JOUEURS_MAX; i++) {
    client_threads[i] =
        (struct Client_Thread*)malloc(sizeof(struct Client_Thread));
    if (pthread_create(&client_threads[i]->thread, NULL, threadSessionClient,
                       client_threads[i]) != 0)
      erreur_IO("pthread_create");
    if (sem_init(&client_threads[i]->sem, 0, 0) == -1) erreur_IO("sem_init");
    client_threads[i]->libre = 1;
  }
  return client_threads;
}

void detruireClientThreads(struct Client_Thread** client_threads) {
  for (int i = 0; i < NB_JOUEURS_MAX; i++) {
    // if (pthread_join(client_threads[i]->thread, NULL) != 0)
    //   erreur_IO("pthread_join");  // TODO: Afficher message de retour
    free(client_threads[i]);
  }
}