/**
 * @dir server
 * @brief Dossier code Serveur
 */

/**
 * @file server.c  // TODO: Fill
 *
 * @brief Serveur/Jeu gérant la progression d'un jeu.
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

#define FAUX 0
#define VRAI 1

void creerClientThread(struct Client_Thread** client_threads);
struct Client_Thread* chercherWorkerLibre(struct Client_Thread**);

/// Affiche l'aide
static void printHelp(void);

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
  int soc, ret, canal;
  short port;
  struct sockaddr_in adrEcoute, adrClient;
  unsigned int lgAdrClient;
  struct Client_Thread* thread_libre;
  struct DC* datacontext = creerDC();
  pthread_t partie;
  pthread_create(&partie, NULL, deroulement, datacontext);
  if (sem_init(&sem_global, 0, NB_JOUEURS_MAX) == -1) erreur_IO("sem_init");

  // Arguments positionnés
  if (argc < 2) {
    printf("ERREUR : Pas assez d'arguments.\n\n");
    printHelp();
  }
  if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) printHelp();

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
  ret = listen(soc, 23);
  if (ret < 0) erreur_IO("listen");

  while (1) {
    sem_wait(&sem_global);
    canal = accept(soc, (struct sockaddr*)&adrClient, &lgAdrClient);
    if (canal < 0) erreur_IO("accept");

    printf("Server: adr %s, port %hu\n",
           stringIP(ntohl(adrClient.sin_addr.s_addr)),
           ntohs(adrClient.sin_port));

    while ((thread_libre = chercherWorkerLibre(datacontext->client_threads)) ==
           NULL)
      usleep(1);
    thread_libre->canal = canal;
    if (lireLigne(canal, thread_libre->pseudo) < 0) erreur_IO("lireLigne");
    thread_libre->joueur = creerJoueur(thread_libre->pseudo);
    ajouterJoueur(datacontext->liste_joueurs, thread_libre->joueur);
    sem_post(&(thread_libre->sem));
  }
  if (close(soc) == -1) erreur_IO("fermeture ecoute");

  return 0;
}

/// retourne le numero du worker libre ou -1 si pas de worker libre
struct Client_Thread* chercherWorkerLibre(
    struct Client_Thread** client_threads) {
  int i = 0;
  while (client_threads[i]->libre == 0 && i < NB_JOUEURS_MAX) i++;
  if (i < NB_JOUEURS_MAX)
    return client_threads[i];
  else
    return NULL;
}

static void printHelp(void) {
  printf(  // TODO: Fill
      "Serveur pour le Dilemme du Prisonnier Multijoueur par Marc NGUYEN et\n\
Thomas LARDY en Mai 2019\n\
\n\
Usage: dilemme_prisonnier_server <port>\n\n\
Autres:\n\
  -h,  --help             Affiche ce dialogue\n");
  exit(0);
}
