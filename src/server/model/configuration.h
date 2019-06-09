#if !defined(CONFIGURATION_H)
#define CONFIGURATION_H

#define CONFIG_FILE "server.properties"

struct Config {
  long NB_JOUEURS_MAX;
  long SCORE_DEFAULT;
  long DURATION;
  long SCORE_TC;       // Trahir = Negatif pour pertes.
  long SCORE_2COOP;    // Negatif pour pertes.
  long SCORE_2TRAHIR;  // Negatif pour pertes.
  long MAX_ROUND;
};

struct Config lireConfig(void);

#endif  // CONFIGURATION_H
