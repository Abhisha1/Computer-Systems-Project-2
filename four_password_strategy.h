#include <stdlib.h>

#include "hashtable.h"
#include "proj-2_sha256.h"
#include "passwords.h"
void brute_force_four(HashTable *ht, Passwords* solved);

int popular_character_guess_four(HashTable *ht, Passwords* solved);

int generate_guesses_four(HashTable *ht, Passwords* solved);
