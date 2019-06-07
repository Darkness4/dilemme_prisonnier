#if !defined(DATACONTEXT_H)
#define DATACONTEXT_H

#include "client_thread.h"
#include "joueur.h"
#include "match.h"

extern sem_t sem_global;

struct DC {
  struct ListeJoueurs* liste_joueurs;
  struct ListeMatches* liste_matches;
  struct Client_Thread** client_threads;
};

struct DC* creerDC(void);
void detruireDC(struct DC* datacontext);

#endif  // DATACONTEXT_H
