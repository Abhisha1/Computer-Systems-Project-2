#include <stdbool.h>

typedef struct passwords Passwords;



Passwords* create_passwords(int passwords_to_guess);

void add_new_cracked(Passwords * pwrds, char* pword);

int remaining_passwords(Passwords *pwrds);

void free_passwords(Passwords* pwrds);

void print_passwords(Passwords* pwrds);