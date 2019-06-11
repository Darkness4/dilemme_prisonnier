/**
 * @dir client
 * @brief Dossier code client
 */

/**
 * @file client.c
 *
 * @brief Joueur pouvant communiquer avec d'autres clients/serveur.
 *
 * Connection via IP, Port et Pseudo.
 *
 * DNS supporté.
 *
 * Fonctionnalités :
 * - Lecture P2P Client-Serveur
 * - Ecriture P2P Client-Serveur
 *
 * Usage:
 * ```
 * ./client <ip> <port> <pseudo>
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
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "error_handler/error_handler.h"
#include "ligne/ligne.h"
#include "resolv/resolv.h"

/// Affiche l'aide.
static void printHelp(void);
/// Interrupt signal handler.
static void intHandler(int sig);
static int soc;
static pid_t pid;

/**
 * @brief Programme principal Client.
 *
 * Executez le programme avec l'argument --help pour connaitre les arguments.
 *
 * @param argc
 * @param argv
 * @return int exit(0)
 */
int main(int argc, char const *argv[]) {
  // Variables
  int ret;
  int lgEcr, lgLue;
  struct sockaddr_in *adrServ;
  signal(SIGINT, intHandler);

  if (argc < 2) {
    printf("ERREUR : Pas assez d'arguments.\n\n");
    printHelp();
  }
  if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) printHelp();
  if (argc < 4) {
    printf("ERREUR : Pas assez d'arguments.\n\n");
    printHelp();
  }

  printf("%s: creating a socket\n", argv[3]);
  soc = socket(AF_INET, SOCK_STREAM, 0);
  if (soc < 0) erreur_IO("socket");

  printf("%s: DNS resolving for %s, port %s\n", argv[3], argv[1], argv[2]);
  adrServ = resolv(argv[1], argv[2]);

  if (adrServ == NULL)
    erreur("adresse %s port %s inconnus\n", argv[1], argv[2]);

  printf("%s: adr %s, port %hu\n", argv[3],
         stringIP(ntohl(adrServ->sin_addr.s_addr)), ntohs(adrServ->sin_port));

  printf("%s: connecting the socket\n", argv[3]);
  ret = connect(soc, (struct sockaddr *)adrServ, sizeof(struct sockaddr_in));
  if (ret < 0) erreur_IO("connect");

  // Envoyer le pseudo au serveur
  lgEcr = ecrireLigne(soc, (char *)argv[3]);
  if (lgEcr == -1) erreur_IO("ecrireLigne");

  pid = fork();
  if (pid == -1) erreur_IO("fork");
  if (pid == 0) {  // processus d'écriture
    char ligne_client[BUFSIZ];
    while (1) {
      fgets(ligne_client, BUFSIZ, stdin);
      lgEcr = ecrireLigne(soc, ligne_client);
      if (lgEcr == -1) erreur_IO("ecrireLigne");
      if (strcmp(ligne_client, "/quit\n") == 0) {
        printf("QUITTING!\n");
        raise(SIGINT);
      }
    }
  } else {  // processus de lecture
    char ligne_serveur[BUFSIZ];
    while (1) {
      lgLue = lireLigne(soc, ligne_serveur);
      if (lgLue < 0)
        erreur_IO("lireLigne");
      else if (lgLue == 0)
        erreur("arret client\n");

      printf("\033[1;32m%s\033[0;0m\n", ligne_serveur);
    }
  }
  if (close(soc) == -1) erreur_IO("close socket");

  return 0;
}

static void printHelp(void) {
  printf(
      "Client pour le Dilemme du Prisonnier Multijoueur par Marc NGUYEN et\n\
Thomas LARDY en Mai 2019\n\
\n\
Usage: dilemme_prisonnier_client <ip> <port> <pseudo> \n\n\
Autres:\n\
  -h,  --help             Affiche ce dialogue\n");
  exit(0);
}

static void intHandler(int sig) {
  int lgEcr = ecrireLigne(soc, "/quit\n");
  if (lgEcr == -1) erreur_IO("ecrireLigne");
  if (close(soc) == -1) erreur_IO("close socket");
  printf("Interrupt catched: signal %i\n", WTERMSIG(sig));
  kill(pid, SIGTERM);
  exit(SIGINT);
}