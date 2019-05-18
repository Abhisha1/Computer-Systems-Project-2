#include <stdlib.h>

#include "hashtable.h"
#include "passwords.h"
void brute_force_six(HashTable *ht, Passwords *solved,int n_guesses);
int popular_character_guess_six(HashTable *ht, Passwords *solved,int n_guesses);
int generate_guesses_six(int n_guesses, HashTable *ht, Passwords *solved);