/**
 * @file joueur.c
 *
 * @brief Contient et manipule les informations à propos du joueur.
 *
 * Les structures permettent la manipulation des données du Joueur et
 * éventuellement d'une liste de Joueurs.
 *
 * Fonctionnalités :
 * - Créer et supprimer un Joueur
 * - Créer, supprimer, pop, clone, trouver dans une liste de Joueurs
 * - Check le status PRET dans la liste de Joueurs
 * - Set le Score, Choix, Etat de tous les Joueur dans la liste de Joueurs
 *
 * Usage:
 * ```
 * struct Joueur* joueur = creerJoueur("PSEUDO");
 * struct ListeJoueurs* liste_joueurs = creerListeJoueurs();
 * ajouterJoueur(liste_joueurs, joueur);
 * struct ListeJoueurs* copie_liste_joueurs = clonerListeJoueurs(liste_joueurs);
 * struct Joueur* joueur_found = trouverJoueurParPseudo(liste_joueurs,
 * "PSEUDO");
 * char* pseudo = popJoueur(liste_joueurs);
 * char* pseudo = popJoueurParPseudo(liste_joueurs, "PSEUDO");
 * detruireListeJoueurs(liste_joueurs);
 * ```
 *
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @date 20 Mai 2019
 */

#include "joueur.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error_handler/error_handler.h"
#include "../ligne/ligne.h"

/**
 * @brief Crée et initialise un struct Joueur.
 *
 * joueur->choix = RIEN;
 * joueur->etat = NOT_PRET;
 * joueur->pseudo = pseudo;
 * sem_init(&joueur->state_sem, 0, 0);
 *
 * @param pseudo Pseudonyme du Joueur.
 * @return struct Joueur* Pointer vers la struct Joueur.
 */
struct Joueur* creerJoueur(char* pseudo) {
  struct Joueur* joueur = (struct Joueur*)malloc(sizeof(struct Joueur));
  joueur->choix = RIEN;
  joueur->etat = NOT_PRET;
  joueur->pseudo = pseudo;
  return joueur;
}

/**
 * @brief Supprime le Joueur et libère la mémoire.
 *
 * @param joueur Pointer vers la struct Joueur.
 */
void detruireJoueur(struct Joueur* joueur) { free(joueur); }

/**
 * @brief Crée et initialise une liste de Joueurs.
 *
 * Liste chaînée de type LIFO.
 *
 * @return struct ListeJoueurs* Pointer vers la struct ListeJoueurs.
 */
struct ListeJoueurs* creerListeJoueurs(void) {
  struct ListeJoueurs* liste_joueurs =
      (struct ListeJoueurs*)malloc(sizeof(struct ListeJoueurs));
  liste_joueurs->nb_joueurs = 0;
  liste_joueurs->HEAD = NULL;
  return liste_joueurs;
}

/**
 * @brief Ajoute un Joueur à la liste de Joueurs.
 *
 * Ajoute à la liste tel un LIFO, c'est-à-dire :
 *
 * - NULL<-...<-Joueur<-HEAD
 * - => NULL<-...<-Joueur<-Joueur ajoutée<-HEAD
 *
 * @param liste_joueurs Pointer vers la struct ListeJoueurs.
 * @param joueur Pointer vers la struct Joueur.
 */
void ajouterJoueur(struct ListeJoueurs* liste_joueurs, struct Joueur* joueur) {
  joueur->prev = NULL;
  joueur->next = liste_joueurs->HEAD;
  if (joueur->next != NULL) joueur->next->prev = joueur;
  liste_joueurs->HEAD = joueur;
  liste_joueurs->nb_joueurs++;
}

/**
 * @brief Ejecte un Joueur tel un LIFO.
 *
 * @param liste_joueurs Pointer vers la struct ListeJoueurs.
 * @return char* Pseudonyme du Joueur éjecté.
 */
char* popJoueur(struct ListeJoueurs* liste_joueurs) {
  if (liste_joueurs->nb_joueurs < 1) erreur("Tentative de pop aucun joueur !");
  char* pseudo = liste_joueurs->HEAD->pseudo;
  struct Joueur* next = liste_joueurs->HEAD->next;
  detruireJoueur(liste_joueurs->HEAD);
  liste_joueurs->HEAD = next;
  if (next != NULL) next->prev = NULL;
  liste_joueurs->nb_joueurs--;
  return pseudo;
}

/**
 * @brief Ejecte un Joueur précis.
 *
 * NULL = Erreur.
 *
 * @param liste_joueurs Pointer vers la struct ListeJoueurs.
 * @param pseudo Pseudonyme du Joueur à éjecter
 * @return char* Pseudonyme du Joueur éjecté
 */
char* popJoueurParPseudo(struct ListeJoueurs* liste_joueurs, char* pseudo) {
  if (liste_joueurs->nb_joueurs < 1) erreur("Tentative de pop aucun joueur !");
  struct Joueur* pJoueur = liste_joueurs->HEAD;
  struct Joueur* next;
  char* pseudo_return = NULL;

  while (pJoueur != NULL) {
    if (strcmp(pJoueur->pseudo, pseudo) == 0) {
      next = pJoueur->next;
      if (pJoueur->prev == NULL)
        liste_joueurs->HEAD = next;
      else
        pJoueur->prev->next = next;
      if (next != NULL) next->prev = pJoueur->prev;
      pseudo_return = pJoueur->pseudo;
      detruireJoueur(pJoueur);
      liste_joueurs->nb_joueurs--;
      return pseudo_return;
    }
    pJoueur = pJoueur->next;
  }
  return pseudo_return;
}

/**
 * @brief Ejecte un Joueur précis par canal.
 *
 * -1 = Erreur.
 *
 * @param liste_joueurs Pointer vers la struct ListeJoueurs.
 * @param canal Canal du Joueur à éjecter
 * @return int Canal du Joueur éjecté
 */
int popJoueurParCanal(struct ListeJoueurs* liste_joueurs, int canal) {
  if (liste_joueurs->nb_joueurs < 1) erreur("Tentative de pop aucun joueur !");
  struct Joueur* pJoueur = liste_joueurs->HEAD;
  struct Joueur* next;
  int canal_return = -1;

  while (pJoueur != NULL) {
    if (pJoueur->canal == canal) {
      next = pJoueur->next;
      if (pJoueur->prev == NULL)
        liste_joueurs->HEAD = next;
      else
        pJoueur->prev->next = next;
      if (next != NULL) next->prev = pJoueur->prev;
      canal_return = pJoueur->canal;
      detruireJoueur(pJoueur);
      liste_joueurs->nb_joueurs--;
      return canal_return;
    }
    pJoueur = pJoueur->next;
  }
  return canal_return;
}

/**
 * @brief Trouve un Joueur précis.
 *
 * @param liste_joueurs Pointer vers la struct ListeJoueurs.
 * @param pseudo Pseudonyme du Joueur à trouver.
 * @return struct Joueur* Pointer vers la struct Joueur trouvé.
 */
struct Joueur* trouverJoueurParPseudo(struct ListeJoueurs* liste_joueurs,
                                      char* pseudo) {
  struct Joueur* pJoueur = liste_joueurs->HEAD;

  while (pJoueur != NULL) {
    if (strcmp(pJoueur->pseudo, pseudo) == 0) return pJoueur;
    pJoueur = pJoueur->next;
  }
  return NULL;
}

/**
 * @brief Supprime les Joueurs, la liste de Joueurs et libère la mémoire.
 *
 * @param liste_joueurs Pointer vers la struct ListeJoueurs.
 */
void detruireListeJoueurs(struct ListeJoueurs* liste_joueurs) {
  struct Joueur* pJoueur = liste_joueurs->HEAD;
  struct Joueur* next;
  if (pJoueur != NULL) {
    next = liste_joueurs->HEAD->next;
    while (next != NULL) {
      next = pJoueur->next;
      detruireJoueur(pJoueur);
      pJoueur = next;
    }
  }
  free(liste_joueurs);
}

/**
 * @brief Clone les Joueurs et la liste de Joueurs.
 *
 * Les Joueurs clonées ont de nouvelles mémoires allouées.
 * Pour trouver le Joueur original, utiliser trouverJoueurParPseudo sur la liste
 * originale.
 *
 * @param liste_joueurs Pointer vers la struct ListeJoueurs.
 * @return struct ListeJoueurs* Pointer vers la struct ListeJoueurs clonée.
 */
struct ListeJoueurs* clonerListeJoueurs(struct ListeJoueurs* liste_joueurs) {
  struct Joueur* pJoueur = liste_joueurs->HEAD;
  struct Joueur* nouveau_joueur;
  struct ListeJoueurs* copie_liste_joueurs = creerListeJoueurs();
  while (pJoueur->next != NULL) pJoueur = pJoueur->next;  // Aller à la fin
  while (pJoueur != NULL) {                               // Réplication
    nouveau_joueur = creerJoueur(pJoueur->pseudo);
    ajouterJoueur(copie_liste_joueurs, nouveau_joueur);
    pJoueur = pJoueur->prev;
  }
  return copie_liste_joueurs;
}

/**
 * @brief Transmet un texte à tous les joueurs
 *
 * @param joueur Pointeur vers la structure joueur
 * @param texte Chaine de caractère
 */

void broadcastAutreJoueurs(struct Joueur* joueur, char* texte) {
  struct Joueur* joueurDroite = joueur->next;
  struct Joueur* joueurGauche = joueur->prev;
  char texte_final[BUFSIZ];

  sprintf(texte_final, "%s> %s\n", joueur->pseudo, texte);

  while (joueurDroite != NULL) {
    ecrireLigne(joueurDroite->canal, texte_final);
    joueurDroite = joueurDroite->next;
  }
  while (joueurGauche != NULL) {
    ecrireLigne(joueurGauche->canal, texte_final);
    joueurGauche = joueurGauche->prev;
  }
  printf("[BROADCAST] Complete\n");
}

/**
 * @brief Transmet un texte à tous les joueurs
 *
 * @param joueur Pointeur vers la structure joueur
 * @param texte Chaine de caractère
 */
void broadcastJoueurs(struct ListeJoueurs* liste_joueurs, char* texte) {
  struct Joueur* pJoueur = liste_joueurs->HEAD;

  while (pJoueur != NULL) {
    ecrireLigne(pJoueur->canal, texte);
    pJoueur = pJoueur->next;
  }
}

/**
 * @brief Retourne si les Joueurs sont prêts.
 *
 * @param liste_joueurs Pointer vers la struct ListeJoueurs.
 * @return char Boolean.
 */
char sontPretListeJoueurs(struct ListeJoueurs* liste_joueurs) {
  int i = 0;
  struct Joueur* pJoueur = liste_joueurs->HEAD;

  while (pJoueur != NULL) {
    if (pJoueur->etat != PRET1 && pJoueur->etat != PRET2)
      return 0;
    else
      i++;
    pJoueur = pJoueur->next;
  }
  if (i > 1)
    return 1;
  else
    return 0;
}

/**
 * @brief Set le score de tous les Joueurs de la liste de Joueurs.
 *
 * @param liste_joueurs Pointer vers la struct ListeJoueurs.
 * @param score Score des Joueurs.
 */
void setScoreListeJoueurs(struct ListeJoueurs* liste_joueurs, long score) {
  struct Joueur* pJoueur = liste_joueurs->HEAD;

  while (pJoueur != NULL) {
    pJoueur->score = score;
    pJoueur = pJoueur->next;
  }
}

/**
 * @brief Set l'état de tous les Joueurs de la liste de Joueurs.
 *
 * @param liste_joueurs Pointer vers la struct ListeJoueurs.
 * @param etat Etat des Joueurs.
 */
void setEtatListeJoueurs(struct ListeJoueurs* liste_joueurs,
                         enum EtatJoueur etat) {
  struct Joueur* pJoueur = liste_joueurs->HEAD;

  while (pJoueur != NULL) {
    pJoueur->etat = etat;
    pJoueur = pJoueur->next;
  }
}

/**
 * @brief Set le choix de tous les Joueurs de la liste de Joueurs.
 *
 * @param liste_joueurs Pointer vers la struct ListeJoueurs.
 * @param choix Choix des Joueurs.
 */
void setChoixListeJoueurs(struct ListeJoueurs* liste_joueurs,
                          enum ChoixJoueur choix) {
  struct Joueur* pJoueur = liste_joueurs->HEAD;

  while (pJoueur != NULL) {
    pJoueur->choix = choix;
    pJoueur = pJoueur->next;
  }
}
