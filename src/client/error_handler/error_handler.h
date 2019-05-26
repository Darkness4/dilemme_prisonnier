#if !defined(ERROR_HANDLER_H)
#define ERROR_HANDLER_H

/**
 * @brief Catch une erreur, print un message et exit failure.
 *
 * @param message Message d'erreur.
 */
void erreur_IO(const char *message);

/**
 * @brief Catch une erreur, print un message et exit pthread.
 *
 * @param message Message d'erreur.
 */
void erreur_pthread_IO(const char *message);

/**
 * @brief Print un message d'erreur et exit failure.
 *
 * @param message Message d'erreur.
 */
void erreur(const char *format, ...);

#endif  // ERROR_HANDLER_H
