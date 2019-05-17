#include <stdlib.h>

#include "hashtable.h"
void brute_force_six(HashTable *ht);
void generate_guesses_six(int n_guesses, HashTable *ht);
int popular_character_guess_six(HashTable *ht);
char *sha256_byteToHexString(BYTE data[]);