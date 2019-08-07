/**
 * @file configuration.c
 *
 * @brief Gère les configurations du serveur
 *
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @date 20 Mai 2019
 */
#include "configuration.h"

#include <error_handler.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct Config lireConfig(void) {
  struct Config config = {
      // Default value
      .NB_JOUEURS_MAX = 24, .SCORE_DEFAULT = 1000000, .DURATION = 5,
      .SCORE_TC = 400000,   .SCORE_2COOP = 100000,    .SCORE_2TRAHIR = 200000,
      .MAX_ROUND = 5,
  };
  char properties[BUFSIZ];
  char value[BUFSIZ];
  char line[BUFSIZ];
  FILE *configFile;
  const int fd = open(CONFIG_FILE, O_RDONLY, 0400);
  if (fd == -1) erreur_IO("open: server.properties");
  configFile = fdopen(fd, "r");
  if (configFile == NULL) erreur_IO("fopen: server.properties");

  while (fscanf(configFile, "%256[^\n]\n", line) != EOF) {
    if (sscanf(line, "%32[^=]=%32s", properties, value) == 2) {
      printf("%s: %s\n", properties, value);
      if (strcmp(properties, "max-players") == 0) {
        long max_players;
        sscanf(value, "%li", &max_players);
        if (max_players < 2)
          erreur("Configuration incorrecte! Illegal: max-players < 2.\n");
        config.NB_JOUEURS_MAX = max_players;
      } else if (strcmp(properties, "waiting-time") == 0) {
        long waiting_time;
        sscanf(value, "%li", &waiting_time);
        if (waiting_time < 0)
          erreur("Configuration incorrecte! Illegal: waiting-time < 0.\n");
        config.DURATION = waiting_time;
      } else if (strcmp(properties, "score-default") == 0) {
        long score_default;
        sscanf(value, "%li", &score_default);
        config.SCORE_DEFAULT = score_default;
      } else if (strcmp(properties, "score-trahir-coop") == 0) {
        long score_trahir_coop;
        sscanf(value, "%li", &score_trahir_coop);
        config.SCORE_TC = score_trahir_coop;
      } else if (strcmp(properties, "score-double-trahir") == 0) {
        long score_double_trahir;
        sscanf(value, "%li", &score_double_trahir);
        config.SCORE_2TRAHIR = score_double_trahir;
      } else if (strcmp(properties, "score-double-coop") == 0) {
        long score_double_coop;
        sscanf(value, "%li", &score_double_coop);
        config.SCORE_2COOP = score_double_coop;
      } else if (strcmp(properties, "max-rounds") == 0) {
        long max_rounds;
        sscanf(value, "%li", &max_rounds);
        if (max_rounds < 1)
          erreur("Configuration incorrecte! Illegal: max-rounds < 1.\n");
        config.MAX_ROUND = max_rounds;
      }
    } else {
      printf("Ignored line : %s\n", line);
    }
  }

  printf("Configuration loaded.\n");

  if (fclose(configFile) != 0) erreur_IO("fclose");

  return config;
}