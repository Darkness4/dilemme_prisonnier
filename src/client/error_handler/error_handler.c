/**
 * @file error_handler.c  // TODO: Fill
 *
 * @brief Contient les informations à propos du error_handler.
 *
 * **Description Here**
 *
 * Fonctionnalités :
 * - **Feature Here**
 *
 * Usage:
 * ```
 * **Usage Here**
 * ```
 *
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @date 20 Mai 2019
 */

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "error_handler.h"

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