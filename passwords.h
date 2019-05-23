#include <stdbool.h>

typedef struct passwords Passwords;


//Creates a password structure to hold solved passwords
Passwords* create_passwords(int passwords_to_guess, int n_guesses);

// Adds a cracked password
void add_new_cracked(Passwords * pwrds, char* pword);

// Returns the number of passwords left to crack
int remaining_passwords(Passwords *pwrds);

// Deallocates memory
void free_passwords(Passwords* pwrds);

// Prints passwords
void print_passwords(Passwords* pwrds);


// Generates passwords using common substituions for 4 letter password
int generate_common_subs_four(Passwords* solved, HashTable *ht);

// Generates passwords using common substituions for 6 letter password
int generate_common_subs_six(Passwords* solved, HashTable *ht);

// Returns the number of guesses that can be made
int get_remaining_guesses(Passwords *pwrds);

// Decreases the number of guesses
void made_guess(Passwords *pwrds);