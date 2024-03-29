#if !defined(JOUEUR_H)
#define JOUEUR_H

/// Etat d'un Joueur
enum EtatJoueur {
  NOT_PRET,
  PRET1,
  JOUE,
  DOIT_ACCEPTER,
  ATTENTE,
  PRET2,
  ELIMINE,
  NONE,
};

// Choix d'un Joueur
enum ChoixJoueur {
  RIEN,
  COOPERER,
  TRAHIR,
};

/// Données relative au Joueur.
struct Joueur {
  /// Status du Joueur.
  enum EtatJoueur etat;
  /// Pseudo unique du Joueur
  char* pseudo;
  /// Représente le nombre de points/montant/points de vie du Joueur.
  long score;
  /// Choix du Joueur durant la partie.
  enum ChoixJoueur choix;
  /// Joueur suivant.
  struct Joueur* next;
  /// Joueur precedent.
  struct Joueur* prev;
  /// Match du joueur participant.
  struct Match* match;
  /// Indice du joueur dans la struct Match.
  int id_joueur_match;
  /// Canal de communication.
  int canal;
};

/// Liste de joueurs connectés
struct ListeJoueurs {
  /// Nombre de joueurs connectés.
  unsigned char nb_joueurs;
  /// Dernier joueur connecté.
  struct Joueur* HEAD;
};

/// Crée et initialise un struct Joueur.
struct Joueur* creerJoueur(char* pseudo);
/// Supprime le Joueur et libère la mémoire.
void detruireJoueur(struct Joueur* joueur);

/// Crée et initialise une liste de Joueurs.
struct ListeJoueurs* creerListeJoueurs(void);
/// Supprime les Joueurs, la liste de Joueurs et libère la mémoire.
void detruireListeJoueurs(struct ListeJoueurs* liste_joueurs);
/// Ajoute un Joueur à la liste de Joueurs.
void ajouterJoueur(struct ListeJoueurs* liste_joueurs, struct Joueur* joueur);
/// Ejecte un Joueur tel un LIFO.
char* popJoueur(struct ListeJoueurs* liste_joueurs);
/// Ejecte un Joueur précis.
char* popJoueurParPseudo(struct ListeJoueurs* liste_joueurs, char* pseudo);
/// Ejecte un Joueur précis par canal.
int popJoueurParCanal(struct ListeJoueurs* liste_joueurs, int canal);
/// Ejecte un Joueur par Etat.
enum EtatJoueur popJoueurParEtat(struct ListeJoueurs* liste_joueurs,
                                 enum EtatJoueur etat);
/// Clone les Joueurs et la liste de Joueurs.
struct ListeJoueurs* clonerListeJoueurs(struct ListeJoueurs* liste_joueurs);
/// Envoie du texte dans le canal des joueur dans la liste de joueurs
void broadcastAutreJoueurs(struct Joueur* joueur, char* texte);
/// Envoie du texte à tout le monde
void broadcastJoueurs(struct ListeJoueurs* liste_joueurs, char* texte);

/// Retourne si les Joueurs sont prêts.
char sontPretListeJoueurs(struct ListeJoueurs* liste_joueurs);
/// Trouve un Joueur précis.
struct Joueur* trouverJoueurParPseudo(struct ListeJoueurs* liste_joueurs,
                                      char* pseudo);

/// Set le score de tous les Joueurs de la liste de Joueurs.
void setScoreListeJoueurs(struct ListeJoueurs* liste_joueurs, long score);
/// Set l'état de tous les Joueurs de la liste de Joueurs.
void setEtatListeJoueurs(struct ListeJoueurs* liste_joueurs,
                         enum EtatJoueur etat);
/// Set le choix de tous les Joueurs de la liste de Joueurs.
void setChoixListeJoueurs(struct ListeJoueurs* liste_joueurs,
                          enum ChoixJoueur choix);

#endif  // JOUEUR_H
