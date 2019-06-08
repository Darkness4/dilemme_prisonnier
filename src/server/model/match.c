/**
 * @file match.c
 *
 * @brief Gère la logique des matches.
 *
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @date 20 Mai 2019
 */

#include "match.h"

#include <stdlib.h>

#include "../error_handler/error_handler.h"

/// nombre!
static long _factorielle(long nombre);
/// Créer un Match entre deux Joueur.
static struct Match* _creerMatch(struct Joueur* joueur1,
                                 struct Joueur* joueur2);
/// Supprime un Match et libère la mémoire.
static void _detruireMatch(struct Match* match);

static struct Match* _creerMatch(struct Joueur* joueur1,
                                 struct Joueur* joueur2) {
  struct Match* match = (struct Match*)malloc(sizeof(struct Match));
  match->etat = NOT_STARTED;
  match->joueur[0] = joueur1;
  match->joueur[1] = joueur2;
  match->round_count = 0;
  if (sem_init(&match->state_sem, 0, 0) == -1)  // Mode "Trigger"
    erreur_IO("sem_init");
  return match;
}

static void _detruireMatch(struct Match* match) {
  if (sem_destroy(&match->state_sem) == -1) erreur_IO("sem_destroy");
  free(match);
}

/**
 * @brief Créer une liste de matches en fonction d'une liste de joueurs
 *
 * Premièrement, on reserve Comb(nb_joueurs, 2) matches.
 * Ensuite, on clone la liste de joueurs.
 * On utilise pop la liste de joueurs clonée afin de parcourir la liste tel une
 * combinaison.
 *
 * Schématiquement, ça donne : ABCD[E]
 *
 * ```txt
 * ^^^^
 * ||||
 * E
 *
 * ABC[D]
 * ^^^
 * |||
 * D
 *
 * AB[C]
 * ^^
 * ||
 * C
 *
 * A[B]
 * ^
 * |
 * B
 * ```
 *
 * Chaque flèche représente un match.
 *
 * @param liste_joueurs Pointer vers la struct ListeJoueurs.
 * @return struct ListeMatches* Pointer vers la struct ListeMatches.
 */
struct ListeMatches* creerListeMatch(struct ListeJoueurs* liste_joueurs) {
  struct ListeMatches* liste_matches =
      (struct ListeMatches*)malloc(sizeof(struct ListeMatches*));

  long nb_matches = _factorielle(liste_joueurs->nb_joueurs) /
                    (_factorielle(liste_joueurs->nb_joueurs - 2) * 2);
  struct Match** matches =
      (struct Match**)malloc(sizeof(struct Match*) * nb_matches);

  struct ListeJoueurs* copie_liste_joueurs = clonerListeJoueurs(liste_joueurs);
  struct Joueur *joueur1, *joueur2 = NULL;
  struct Joueur* pJoueur;
  char* pseudo = NULL;
  long match = 0;
  while (copie_liste_joueurs->HEAD != NULL) {
    pseudo = popJoueur(copie_liste_joueurs);
    joueur1 = trouverJoueurParPseudo(liste_joueurs, pseudo);
    pJoueur = copie_liste_joueurs->HEAD;
    while (pJoueur != NULL) {
      joueur2 = trouverJoueurParPseudo(liste_joueurs, pJoueur->pseudo);
      matches[match] = _creerMatch(joueur1, joueur2);
      match++;

      pJoueur = pJoueur->next;
    }
  }
  liste_matches->nb_matches = nb_matches;
  liste_matches->matches = matches;
  return liste_matches;
}

/**
 * @brief Supprime ListeMatches et libère la mémoire.
 *
 * @param liste_matches Pointer vers la struct ListeMatches.
 */
void detruireListeMatch(struct ListeMatches* liste_matches) {
  for (long i = 0; i < liste_matches->nb_matches; i++) {
    _detruireMatch(liste_matches->matches[i]);
  }
  free(liste_matches);
}

static long _factorielle(long nombre) {
  long c, fact = 1;
  for (c = 1; c <= nombre; c++) fact = fact * c;
  return fact;
}
