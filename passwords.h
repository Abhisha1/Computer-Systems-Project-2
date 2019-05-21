#include <stdbool.h>

typedef struct passwords Passwords;



Passwords* create_passwords(int passwords_to_guess, int n_guesses);

void add_new_cracked(Passwords * pwrds, char* pword);

int remaining_passwords(Passwords *pwrds);

void free_passwords(Passwords* pwrds);

void print_passwords(Passwords* pwrds);

int generate_common_subs_four(Passwords* solved, HashTable *ht);

int generate_common_subs_six(Passwords* solved, HashTable *ht);

int get_remaining_guesses(Passwords *pwrds);

void made_guess(Passwords *pwrds);