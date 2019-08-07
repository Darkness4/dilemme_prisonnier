#if !defined(CONFIGURATION_H)
#define CONFIGURATION_H

#define CONFIG_FILE "server.properties"

/// Structure de configuration
struct Config {
  /// Nombre de joueurs pouvant participer à un match
  long NB_JOUEURS_MAX;
  /// Score de départ
  long SCORE_DEFAULT;
  /// Temps d'attente
  long DURATION;
  /// Score Trahir/Coop
  long SCORE_TC;       // Trahir = Negatif pour pertes.
  /// Score Coop/Coop
  long SCORE_2COOP;    // Negatif pour pertes.
  /// Score Trahir/Trahir
  long SCORE_2TRAHIR;  // Negatif pour pertes.
  /// Round per Match
  long MAX_ROUND;
};

struct Config lireConfig(void);

#endif  // CONFIGURATION_H
