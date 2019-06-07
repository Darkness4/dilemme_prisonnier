#include "resolv.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void freeResolv(void);

/*
  variable globale a visibilite restreinte a ce fichier.
*/
static struct addrinfo *static_infos = NULL;

struct sockaddr_in *resolv(const char *host, const char *service) {
  int code;
  struct addrinfo hints;

  memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  freeResolv();

  code = getaddrinfo(host, service, &hints, &static_infos);
  if (code == 0)
    return (struct sockaddr_in *)static_infos->ai_addr;
  else {
    fprintf(stderr, "Erreur: %s\n", gai_strerror(code));
    return NULL;
  }
}

char *stringIP(unsigned int entierIP) {
  struct in_addr ia;
  ia.s_addr = htonl(entierIP);
  return inet_ntoa(ia);
}

void freeResolv(void) {
  if (static_infos != NULL) {
    freeaddrinfo(static_infos);
  }
}
