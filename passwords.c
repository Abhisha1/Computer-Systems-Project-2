

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "passwords.h"

struct passwords {
	char **cracked; //array of words
	int passwords_left;
    int current_size;
};

Passwords* create_passwords(int passwords_to_guess){
    Passwords* passwords = malloc(sizeof *passwords);
    assert(passwords);
    passwords->cracked = malloc(sizeof(char*)*passwords_to_guess);
    assert(passwords->cracked);
    passwords->passwords_left = passwords_to_guess;
    passwords->current_size = 0;
    return passwords;
}

void add_new_cracked(Passwords * pwrds, char* pword){
    pwrds->cracked[pwrds->current_size] = calloc(6, sizeof(char*));
    assert(pwrds->cracked[pwrds->current_size]);
    memcpy(pwrds->cracked[pwrds->current_size], pword, strlen(pword));
    pwrds->current_size++;
    pwrds->passwords_left--;
}

int remaining_passwords(Passwords *pwrds){
    return pwrds->passwords_left;
}

void free_passwords(Passwords* pwrds){
    for(int i=0; i < pwrds->current_size; i++){
        free(pwrds->cracked[i]);
    }
    free(pwrds->cracked);
    free(pwrds);
}

void print_passwords(Passwords* pwrds){
    for(int i=0; i< pwrds->current_size; i++){
        printf("%s\n", pwrds->cracked[i]);
    }
}