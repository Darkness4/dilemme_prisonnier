#if !defined(MATCH_H)
#define MATCH_H

#include <pthread.h>
#include <semaphore.h>

#include "joueur.h"

enum EtatMatch {
  NOT_STARTED,
  STARTED,
  ENDED,
};

struct Match {
  struct Joueur* joueur[2];
  enum EtatMatch etat;
  sem_t state_sem;
  char round_count;
  pthread_t pthread_id;
};

struct ListeMatches {
  struct Match** matches;
  long nb_matches;
};

struct Match* creerMatch(struct Joueur* joueur1, struct Joueur* joueur2);
void detruireMatch(struct Match* match);

/**
 * @brief Créer la liste de matches que le serveur devra lancer.
 */
struct ListeMatches* creerListeMatch(struct ListeJoueurs* liste_joueurs);
void detruireListeMatch(struct ListeMatches* liste_matches);

#endif  // MATCH_H
