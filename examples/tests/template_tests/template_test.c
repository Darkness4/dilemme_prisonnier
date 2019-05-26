/**
 * @file template_test.c
 * @author Marc NGUYEN
 * @author Thomas LARDY
 * @brief Lancer des tests unitaires sur les fonctions principales.
 * @date 19 Mai 2019
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../src/template/template.h"

/**
 * @brief Tests unitaires.
 *
 * @return int Exit 0.
 */
int main(void) {
  printf("template_test:\n");  // File
  // Pre-script Here
  printf("  function:\n");  // Group
  // Script Here
  assert(shared_function(2) == 4);
  printf("    Function return 2*2 correctly: \x1B[32mOK\x1B[0m\n");  // Pass
  assert(shared_function(3) == 6);
  printf("    Function return 3*2 correctly: \x1B[32mOK\x1B[0m\n");  // Pass
  return 0;
}
