/**
 * @file error_handler.c
 *
 * @brief Raise des EXIT_FAILURE en plus de rajouter des messages.
 *
 * Fonctionnalités :
 * - erreur_IO: exit(1) avec perror
 * - erreur_pthread_IO: pthread_exit(1) avec perror
 * - erreur: exit(1) avec print
 *
 * @author P Lalevée ?
 * @date 2012 ?
 */

#include "error_handler.h"

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void erreur_IO(const char *message) {
  perror(message);
  exit(EXIT_FAILURE);
}

void erreur_pthread_IO(const char *message) {
  perror(message);
  pthread_exit(NULL);
}

void erreur(const char *format, ...) {
  va_list liste_arg;
  fflush(stdout);
  fprintf(stderr, "[ERREUR] ");
  va_start(liste_arg, format);
  vfprintf(stderr, format, liste_arg);
  va_end(liste_arg);
  fflush(stderr);
  exit(EXIT_FAILURE);
}
