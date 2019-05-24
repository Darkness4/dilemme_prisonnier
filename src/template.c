/**
 * @file template.c
 *
 * @brief XXXXX
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

#include "template.h"
#define BUFFER_SIZE_LAMBDA 40  // No pointer, no memory. A MACRO.

const int SHARED_GLOBAL = 32;

static const int PRIVATE_GLOBAL = 32;

volatile int shared_variable = 32;

static volatile int _private_variable = 32;  // Useful for getters/setters.

enum PinStateType { PIN_OFF, PIN_ON };

/**
 * @brief Describe here how to use the function.
 *
 * ```
 * #include "temp.h"
 *
 * int result = function(2);  // 4
 * ```
 *
 * @param var Value to be multiplied by 2
 * @return int "var" multiplied by 2
 */
static int local_function(int var);

/**
 * @brief Describe here how the function works.
 *
 * The variable is multiplied by two using [local_function(2)].
 *
 * @param var Value to be multiplied by 2
 * @return int "var" multiplied by 2
 */
int shared_function(int var) { return local_function(var); }

/**
 * @brief Describe here how the function works.
 *
 * The variable is multiplied by two using [var] * 2
 *
 * @param var Value to be multiplied by 2
 * @return int "var" multiplied by 2
 */
int local_function(int var) { return var * 2; }