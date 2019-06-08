#if !defined(MATCH_THREAD_H)
#define MATCH_THREAD_H

#include "../model/match.h"

void creerMatchWorkers(struct ListeMatches* liste_matches);
void joinMatchWorkers(struct ListeMatches* liste_matches);

#endif  // MATCH_THREAD_H
