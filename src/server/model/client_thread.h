#if !defined(CLIENT_THREAD_H)
#define CLIENT_THREAD_H

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#include "datacontext.h"
#include "joueur.h"

struct Client_Thread {
  pthread_t thread;
  char libre;
  int canal;
  sem_t sem;
  struct Joueur* joueur;
  char pseudo[BUFSIZ];
};

void* threadSessionClient(void* arg);
struct Client_Thread** creerClientThreads(void);
void detruireClientThreads(struct Client_Thread** client_threads);

#endif  // CLIENT_THREAD_H
