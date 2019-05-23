#include <stdlib.h>

#include "hashtable.h"
#include "proj-2_sha256.h"
#include "passwords.h"
// Tries all character combinations and tries to guess
void brute_force_four(HashTable *ht, Passwords* solved);

// Checks a file in order of popular characters from the distribution of common passwords
int popular_character_guess_four(HashTable *ht, Passwords* solved);

// Genereates guesses using common passwords file
int generate_guesses_four(HashTable *ht, Passwords* solved);
