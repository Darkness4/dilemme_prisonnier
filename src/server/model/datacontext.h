#if !defined(DATACONTEXT_H)
#define DATACONTEXT_H

#include "client_thread.h"
#include "configuration.h"
#include "match.h"

/// Sémaphore du thread principal.
sem_t sem_global;

/// Configuration file
struct Config CONFIG;

/// Contexte de donnnées global.
struct DC {
  /// Liste de contexte de données de chaque Joueur.
  struct ListeJoueurs* liste_joueurs;
  /// Liste de contexte de données de chaque Match.
  struct ListeMatches* liste_matches;
  /// Liste de contexte de données de chaque thread Client.
  struct Client_Thread** client_threads;
};

struct DC* creerDC(void);

#endif  // DATACONTEXT_H
