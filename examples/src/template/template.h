#if !defined(TEMP_H)
#define TEMP_H

/**
 * @brief Define what this is.
 *
 */
extern const int SHARED_GLOBAL_CONST;

/**
 * @brief Define what this is.
 *
 */
extern volatile int shared_variable;

/**
 * @brief Describe here how to use the function.
 *
 * ```
 * #include "temp.h"
 *
 * int result = function(2);  // 4
 * ```
 *
 * @param var Value to be squared
 * @return int "var" squared
 */
int shared_function(int var);

#endif  // TEMP_H
