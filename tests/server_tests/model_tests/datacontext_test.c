/**
 * @file datacontext_test.c
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @brief Lancer des tests unitaires sur XXXX. TODO: Fill
 * @date 19 Mai 2019
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../src/server/model/datacontext.h"

/**
 * @brief Tests unitaires.
 *
 * @return int Exit 0.
 */
int main(void) {
  printf("datacontext_test:\n");  // File
  printf("  DataContext:\n");     // Group
  struct DC* datacontext = creerDC();
  printf("    creerDC correctly: \x1B[32mOK\x1B[0m\n");  // Pass
  // detruireDC(datacontext);
  // printf("    detruireDC correctly: \x1B[32mOK\x1B[0m\n");  // Pass
  return 0;
}
