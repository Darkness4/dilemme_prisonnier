#if !defined(MATCH_H)
#define MATCH_H

#include <pthread.h>
#include <semaphore.h>

#include "joueur.h"

/// Etat d'un Match
enum EtatMatch {
  NOT_STARTED,
  STARTED,
  ENDED,
};

/// Contexte de données d'un match
struct Match {
  /// Joueur[0] vs Joueur[1].
  struct Joueur* joueur[2];
  /// Etat du Match
  enum EtatMatch etat;
  /// Semaphore pour gérer les états.
  sem_t state_sem;
  /// Suivi du Round.
  char round_count;
  /// Thread d'un Match.
  pthread_t pthread_id;
};

/// Liste de matches
struct ListeMatches {
  /// Liste de matches
  struct Match** matches;
  /// Nombre de matches
  long nb_matches;
};

/// Créer une liste de matches en fonction d'une liste de joueurs.
struct ListeMatches* creerListeMatch(struct ListeJoueurs* liste_joueurs);
/// Supprime ListeMatches et libère la mémoire.
void detruireListeMatch(struct ListeMatches* liste_matches);

#endif  // MATCH_H
