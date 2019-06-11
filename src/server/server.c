/**
 * @dir server
 * @brief Dossier code Serveur
 */

/**
 * @file server.c
 *
 * @brief Serveur/Jeu gérant la progression d'un jeu.
 *
 * Jeu du dilemme du prisonnier multijoueur.
 *
 * Etats :
 *
 * - Connection
 * - NOT_PRET automatiquement
 * - PRET1 via /pret
 * - ATTENTE automatiquement
 * - PRET2 via /pret
 * - DOIT_ACCEPTER automatiquement
 * - JOUE via /start
 * - ATTENTE automatiquement
 * - ELIMINE via /quit
 * - Deconnection
 *
 * Usage:
 * ```
 * dilemme_prisonnier_server <port>
 * ```
 *
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @date 20 Mai 2019
 */

#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "deroulement/deroulement.h"
#include "error_handler/error_handler.h"
#include "ligne/ligne.h"
#include "model/client_thread.h"
#include "model/datacontext.h"
#include "resolv/resolv.h"

static struct Client_Thread* _chercherWorkerLibre(
    struct Client_Thread** client_threads);

/// Affiche l'aide
static void _printHelp(void);

/**
 * @brief Programme principal Serveur.
 *
 * Executez le programme avec l'argument --help pour connaitre les arguments.
 *
 * @param argc
 * @param argv
 * @return int exit(0)
 */
int main(int argc, char const* argv[]) {
  CONFIG = lireConfig();
  int soc, ret, canal;
  short port;
  struct sockaddr_in adrEcoute, adrClient;
  unsigned int lgAdrClient;
  struct Client_Thread* thread_libre;
  struct DC* datacontext = creerDC();
  pthread_t partie;
  pthread_create(&partie, NULL, deroulement, datacontext);
  if (sem_init(&sem_global, 0, CONFIG.NB_JOUEURS_MAX) == -1)
    erreur_IO("sem_init");

  // Arguments positionnés
  if (argc < 2) {
    printf("ERREUR : Pas assez d'arguments.\n\n");
    _printHelp();
  }
  if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) _printHelp();

  port = (short)atoi(argv[1]);

  printf("Server: creating a socket\n");
  soc = socket(AF_INET, SOCK_STREAM, 0);  // creation socket
  if (soc < 0) erreur_IO("socket");

  adrEcoute.sin_family = AF_INET;
  adrEcoute.sin_addr.s_addr = INADDR_ANY;
  adrEcoute.sin_port = htons(port);
  printf("Server: binding to INADDR_ANY address on port %d\n", port);
  ret = bind(soc, (struct sockaddr*)&adrEcoute, sizeof(adrEcoute));
  if (ret < 0) erreur_IO("bind");

  printf("Server: listening to socket\n");
  ret = listen(soc, CONFIG.NB_JOUEURS_MAX - 1);
  if (ret < 0) erreur_IO("listen");

  while (1) {
    printf("Server: accepting a connection\n");
    canal = accept(soc, (struct sockaddr*)&adrClient, &lgAdrClient);
    if (canal < 0) erreur_IO("accept");

    printf("Server: adr %s, port %hu\n",
           stringIP(ntohl(adrClient.sin_addr.s_addr)),
           ntohs(adrClient.sin_port));

    // Enlever les joueurs éliminé
    while (popJoueurParEtat(datacontext->liste_joueurs, ELIMINE) != NONE)
      continue;

    // Refuse la connection si jeu en cours
    if (datacontext->liste_matches != NULL) {
      printf("Server: Connection refused. Game is running.\n");
      ecrireLigne(canal,
                  "Serveur> Connexion refusée. Une partie est en cours.\n");
      close(canal);
      continue;
    }

    // Refuse la connection si pseudo existant
    char pseudo[BUFSIZ];
    if (lireLigne(canal, pseudo) < 0) erreur_IO("lireLigne");
    if (trouverJoueurParPseudo(datacontext->liste_joueurs, pseudo) != NULL) {
      printf("Server: Connection refused. Pseudo aleady in use.\n");
      ecrireLigne(
          canal,
          "Serveur> Connexion refusée. Veuillez choisir un autre pseudo.\n");
      close(canal);
      continue;  // Skip ahead
    }

    if (sem_wait(&sem_global) != 0) erreur_IO("sem_wait");
    thread_libre = _chercherWorkerLibre(datacontext->client_threads);
    thread_libre->canal = canal;
    strcpy(thread_libre->pseudo, pseudo);
    thread_libre->joueur = creerJoueur(thread_libre->pseudo);
    ajouterJoueur(datacontext->liste_joueurs, thread_libre->joueur);
    if (sem_post(&thread_libre->sem) != 0) erreur_IO("sem_post");
  }
  if (close(soc) == -1) erreur_IO("fermeture ecoute");

  return 0;
}

/// Retourne le contexte de données du woker libre. Sinon NULL.
static struct Client_Thread* _chercherWorkerLibre(
    struct Client_Thread** client_threads) {
  for (int i = 0; i < CONFIG.NB_JOUEURS_MAX; i++) {
    if (client_threads[i]->libre == 1) return client_threads[i];
  }
  return NULL;
}

static void _printHelp(void) {
  printf(
      "Serveur pour le Dilemme du Prisonnier Multijoueur par Marc NGUYEN et\n\
Thomas LARDY en Mai 2019\n\
\n\
Usage: dilemme_prisonnier_server <port>\n\n\
Autres:\n\
  -h,  --help             Affiche ce dialogue\n");
  exit(0);
}
