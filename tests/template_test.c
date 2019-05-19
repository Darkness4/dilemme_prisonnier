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

/**
 * @brief Main pour tourner les tests unitaires.
 *
 * @return int Exit 0.
 */
int main(void) {
    printf("jouer_un_tour_test:\n"); // File
    // Pre-script Here
    printf("  zombiePresent:\n"); // Group
    // Script Here
    // assert(!zombiePresent(population));
    printf("    False: \x1B[32mOK\x1B[0m\n"); // Pass
    return 0;
}
