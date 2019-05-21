

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hashtable.h"
#include "proj-2_sha256.h"
#include "sha256-helper.h"
#include "passwords.h"

struct passwords {
	char **cracked; //array of words
	int passwords_left;
    int current_size;
    int n_guesses;
};

Passwords* create_passwords(int passwords_to_guess, int n_guesses){
    Passwords* passwords = malloc(sizeof *passwords);
    assert(passwords);
    passwords->cracked = malloc(sizeof(char*)*passwords_to_guess);
    assert(passwords->cracked);
    passwords->passwords_left = passwords_to_guess;
    passwords->current_size = 0;
    passwords->n_guesses = n_guesses;
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

void made_guess(Passwords* pwrds){
    pwrds->n_guesses--;
}

int get_remaining_guesses(Passwords* pwrds){
    return pwrds->n_guesses;
}

int generate_common_subs_four(Passwords* solved, HashTable *ht){
    if (remaining_hashes(ht) == 0 || get_remaining_guesses(solved) == 0){
        return 0;
    }
    char **subs;
    subs = calloc(4,sizeof(char*));
    assert(subs);
    char *characters = calloc(4, sizeof(char));
    assert(characters);
    char *word = solved->cracked[solved->current_size-1];
    char line[20];
    int *subs_size = calloc(4, sizeof(int));
    assert(subs_size);
    char *token;
    const char s[2] = " ";
    for(int i=0; i < 4; i++){
        subs[i] = malloc(10);
        assert(subs[i]);
        subs_size[i] = 0;
        FILE *file = fopen("common_subs.txt", "r");
        while (fgets(line, sizeof(line), file)){
            if(line[0] == word[i]){
                characters[i] = line[0];
                subs[i][subs_size[i]] = line[0];
                subs_size[i]++;
                token = strtok(strstr(line, s), s);
                while(token != NULL && strlen(token) == 1){
                    subs[i][subs_size[i]] = (char)*token;
                    subs_size[i]++;
                    token = strtok(NULL, s);
                }
            }
        }fclose(file);
    }
    char sub_guess[4];
    int hash;
    SHA256_CTX ctx;
    for(int i=0; i<subs_size[0]; i++){
        sub_guess[0] = subs[0][i];
        for(int j=0; j<subs_size[1]; j++){
            sub_guess[1] = subs[1][j];
            for(int k=0; k<subs_size[2]; k++){
                sub_guess[2] = subs[2][k];
                for(int l=0; l<subs_size[3]; l++){
                    sub_guess[3] = subs[3][l];
                    sub_guess[4] = '\0';
                    sha256_init(&ctx);
                    sha256_update(&ctx, (BYTE*)sub_guess, strlen(sub_guess));
                    BYTE guess[32];
                    sha256_final(&ctx, guess);
                    char* hex_guess = sha256_byteToHexString(guess);
                    // printf("%s\n", sub_guess);
                    if ((hash = hash_table_get(ht, hex_guess))>0){
                        printf("%s %d\n", sub_guess, hash);
                        add_new_cracked(solved, sub_guess);
                        printf("remaining passwords are : %d\n", remaining_hashes(ht));
                    }
                    free(hex_guess);
                    made_guess(solved);
                    if (remaining_hashes(ht) == 0 || get_remaining_guesses(solved) == 0){
                        return 0;
                    }
                }
            }
        }
    }



    for(int i=0; i < 4; i++){
        free(subs[i]);
    }
    free(characters);
    free(subs);
    free(subs_size);
    return get_remaining_guesses(solved);
}


int generate_common_subs_six(Passwords* solved, HashTable *ht){
    if (remaining_hashes(ht) == 0 || get_remaining_guesses(solved) == 0){
        return 0;
    }
    char **subs;
    subs = calloc(6,sizeof(char*));
    assert(subs);
    char *characters = calloc(6, sizeof(char));
    assert(characters);
    char *word = solved->cracked[solved->current_size-1];
    char line[20];
    int *subs_size = calloc(6, sizeof(int));
    assert(subs_size);
    char *token;
    const char s[2] = " ";
    for(int i=0; i < 6; i++){
        subs[i] = malloc(10);
        assert(subs[i]);
        subs_size[i] = 0;
        FILE *file = fopen("common_subs.txt", "r");
        while (fgets(line, sizeof(line), file)){
            if(line[0] == word[i]){
                characters[i] = line[0];
                subs[i][subs_size[i]] = line[0];
                subs_size[i]++;
                token = strtok(strstr(line, s), s);
                while(token != NULL && strlen(token) == 1){
                    subs[i][subs_size[i]] = (char)*token;
                    subs_size[i]++;
                    token = strtok(NULL, s);
                }
            }
        }fclose(file);
    }
    char sub_guess[6];
    int hash;
    SHA256_CTX ctx;
    for(int i=0; i<subs_size[0]; i++){
        sub_guess[0] = subs[0][i];
        for(int j=0; j<subs_size[1]; j++){
            sub_guess[1] = subs[1][j];
            for(int k=0; k<subs_size[2]; k++){
                sub_guess[2] = subs[2][k];
                for(int l=0; l<subs_size[3]; l++){
                    sub_guess[3] = subs[3][l];
                    for(int m=0;m < subs_size[4]; m++){
                        sub_guess[4] = (char)subs[4][m];
                        for(int n=0;n < subs_size[5]; n++){
                            sub_guess[5] = (char)subs[5][n];
                            sub_guess[6] = '\0';
                            sha256_init(&ctx);
                            sha256_update(&ctx, (BYTE*)sub_guess, strlen(sub_guess));
                            BYTE guess[32];
                            sha256_final(&ctx, guess);
                            char* hex_guess = sha256_byteToHexString(guess);
                            // printf("%s\n", sub_guess);
                            if ((hash = hash_table_get(ht, hex_guess))>0){
                                printf("%s %d\n", sub_guess, hash);
                                add_new_cracked(solved, sub_guess);
                                printf("remaining passwords are : %d\n", remaining_hashes(ht));
                            }
                            free(hex_guess);
                            made_guess(solved);
                            if (remaining_hashes(ht) == 0 || get_remaining_guesses(solved) == 0){
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
    for(int i=0; i < 4; i++){
        free(subs[i]);
    }
    free(characters);
    free(subs);
    free(subs_size);
    return get_remaining_guesses(solved);
}