#if !defined(CLIENT_THREAD_H)
#define CLIENT_THREAD_H

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#include "datacontext.h"
#include "joueur.h"

/// Contexte de données d'un thread Client.
struct Client_Thread {
  /// ID du thread
  pthread_t thread;
  /// Etat du Worker
  char libre;
  /// Canal de communication
  int canal;
  /// Protection de données
  sem_t sem;
  /// Contexte de données d'un Joueur
  struct Joueur* joueur;
  /// Le pseudo du joueur.
  char pseudo[BUFSIZ];
};

struct Client_Thread** creerClientThreads(void);

#endif  // CLIENT_THREAD_H
