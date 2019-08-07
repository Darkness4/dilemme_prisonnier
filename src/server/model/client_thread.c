/**
 * @file client_thread.c
 *
 * @brief Fonctions permettant de gérer les clients par threads.
 *
 * Capture les données transmis dans le canal de communication et répond en
 * adéquation avec le contexte de données.
 *
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @date 20 Mai 2019
 */

#include "client_thread.h"

#include <error_handler.h>
#include <ligne.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "../model/datacontext.h"
#include "../view/joueur_view.h"
#include "joueur.h"

/**
 * @brief Thread Client Handler.
 *
 * @param arg Structure Client_Thread.
 * @return void* Exit code.
 */
static void* _threadSessionClient(void* arg);
/**
 * @brief Logique du Client Handler.
 *
 * @param joueur Structure Joueur.
 */
static void _sessionClient(struct Joueur* joueur);

static void* _threadSessionClient(void* arg) {
  struct Client_Thread* client_thread = (struct Client_Thread*)arg;

  while (1) {
    if (sem_wait(&client_thread->sem) != 0) erreur_IO("sem_wait");
    client_thread->libre = 0;
    client_thread->joueur->canal = client_thread->canal;

    _sessionClient(client_thread->joueur);

    client_thread->joueur = NULL;

    sem_post(&sem_global);

    client_thread->libre = 1;
  }
  pthread_exit(NULL);
}

static void _sessionClient(struct Joueur* joueur) {
  int lgLue;
  int adversaire_id;
  char ligne_serveur[BUFSIZ];
  int lgEcr = ecrireLigne(
      joueur->canal,
      "Serveur> Bienvenue sur le jeu du Dilemme du Prisonnier Multijoueur\n");
  lgEcr += ecrireLigne(joueur->canal, "CMDS: /pret /quit\n");
  lgEcr += ecrireLigne(joueur->canal,
                       "Serveur> Entrez /pret lorsque tous les joueurs ont "
                       "rejoint le serveur\n");
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
          lgEcr +=
              ecrireLigne(joueur->canal,
                          "Serveur> Veuillez attendre les autres joueurs...\n");
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
          lgEcr = ecrireLigne(joueur->canal,
                              "Serveur> /pret pour chercher un match...\n");
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
          lgEcr = ecrireLigne(joueur->canal,
                              "Serveur> En attente de l'autre joueur...\n");
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
          if (sem_post(&joueur->match->state_sem) != 0)
            erreur_pthread_IO("sem_post");

        } else if (strcmp(ligne_serveur, "/trahir") == 0) {
          printf("[DEBUG STATE] %s est TRAHIR\n", joueur->pseudo);
          if (joueur->choix != TRAHIR) {
            enum ChoixJoueur oldchoix = joueur->choix;
            joueur->choix = TRAHIR;
            if (oldchoix != COOPERER)
              if (sem_post(&joueur->match->state_sem) != 0)
                erreur_pthread_IO("sem_post");
          }

        } else if (strcmp(ligne_serveur, "/coop") == 0) {
          printf("[DEBUG STATE] %s est COOPERER\n", joueur->pseudo);
          if (joueur->choix != COOPERER) {
            enum ChoixJoueur oldchoix = joueur->choix;
            joueur->choix = COOPERER;
            if (oldchoix != TRAHIR)
              if (sem_post(&joueur->match->state_sem) != 0)
                erreur_pthread_IO("sem_post");
          }

          lgEcr = ecrireLigne(joueur->canal,
                              "Serveur> En attente de l'autre joueur...\n");
          lgEcr += ecrireLigne(joueur->canal, "CMDS: /trahir /coop /quit\n");
          if (lgEcr <= -1) erreur_IO("ecrireLigne");

        } else {
          adversaire_id = !joueur->id_joueur_match;
          char buf[BUFSIZ + 10];
          if (snprintf(buf, BUFSIZ + 10, "%s> %s\n", joueur->pseudo,
                       ligne_serveur) <= -1)
            erreur_IO("ecrireLigne");
          lgEcr = ecrireLigne(joueur->match->joueur[adversaire_id]->canal, buf);
          if (lgEcr <= -1) erreur_IO("ecrireLigne");
        }
        break;
      case NONE:
        break;
    }

    printf("[CHAT] %s> %s\n", joueur->pseudo, ligne_serveur);
  }

  if (close(joueur->canal) == -1) erreur_IO("fermeture canal");
}

/**
 * @brief Génère et initialise les Client_Thread pour chaque Client Handler.
 *
 * @return struct Client_Thread** Tableau de Client_Thread. Max: NB_JOUEURS_MAX.
 */
struct Client_Thread** creerClientThreads(void) {
  struct Client_Thread** client_threads = (struct Client_Thread**)malloc(
      sizeof(struct Client_Thread*) * CONFIG.NB_JOUEURS_MAX);
  for (int i = 0; i < CONFIG.NB_JOUEURS_MAX; i++) {
    client_threads[i] =
        (struct Client_Thread*)malloc(sizeof(struct Client_Thread));
    if (pthread_create(&client_threads[i]->thread, NULL, _threadSessionClient,
                       client_threads[i]) != 0)
      erreur_IO("pthread_create");
    if (sem_init(&client_threads[i]->sem, 0, 0) == -1) erreur_IO("sem_init");
    client_threads[i]->libre = 1;
  }
  return client_threads;
}