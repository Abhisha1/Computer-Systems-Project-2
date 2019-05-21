#include <stdlib.h>

#include "hashtable.h"
#include "passwords.h"
void brute_force_six(HashTable *ht, Passwords *solved);
int popular_character_guess_six(HashTable *ht, Passwords *solved);
int generate_guesses_six(char* file_name, HashTable *ht, Passwords *solved);