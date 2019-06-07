#if !defined(MATCH_THREAD_H)
#define MATCH_THREAD_H

#include "../model/match.h"

/**
 * @brief Affiche l'état du Match
 *
 *
 */
void afficherEtatMatch(void);

/**
 * @brief Affiche l'état du Jeu
 *
 */
void afficherEtatJeu(void);

void creerMatchWorkers(struct ListeMatches* liste_matches);
void joinMatchWorkers(struct ListeMatches* liste_matches);

#endif  // MATCH_THREAD_H
