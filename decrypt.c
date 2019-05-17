#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


#include "hashtable.h"
#include "four_password_strategy.h"
#include "six_password_strategy.h"
#include "passwords.h"
// adapted from https://github.com/RemyNoulin/sha256
char** store_password_hashes(char* hashes){
    char **password_hashes = (char**)malloc((strlen(hashes)/64)*sizeof(char*));
    assert(password_hashes);
    memset(password_hashes,0,(strlen(hashes)/64));
    for(int i=1; i <= strlen(hashes)/64; i++){
        password_hashes[i-1] = calloc((64+1), sizeof(char));
        assert(password_hashes[i-1]);
        snprintf(password_hashes[i-1], 65, "%s",hashes + (i-1)*64);
    }
    return password_hashes;
}

void read_hash_file_four(FILE *file){
    if (file){
        char hashes[650];
        int n=0;
        int c;
        while((c = getc(file)) != EOF){
            n +=sprintf(hashes+n,"%02x", c);
        }
        // printf("%s",hashes);
        char **words = store_password_hashes(hashes);

        HashTable *password_hashes = new_hash_table(10);
        for(int i=0; i < strlen(hashes)/64; i++){
            printf("%d:    %s\n", i, words[i]);
            hash_table_put(password_hashes, words[i], (i+1));
        };
        Passwords *cracked_passwords = create_passwords(10);
        generate_guesses_four(10, password_hashes, cracked_passwords);
        popular_character_guess_four(password_hashes, cracked_passwords);
        brute_force_four(password_hashes, cracked_passwords);
        print_passwords(cracked_passwords);
        for(int i=0; i < strlen(hashes)/64; i++){
            printf("%d:    %s\n", i, words[i]);
            free(words[i]);
        };
        free(words);
        free_hash_table(password_hashes);
        free_passwords(cracked_passwords);
        fclose(file);
    }
}


void read_hash_file_six(FILE *file){
    if (file){
        char hashes[650];
        int n=0;
        int c;
        while((c = getc(file)) != EOF){
            n +=sprintf(hashes+n,"%02x", c);
        }
        // printf("%s",hashes);
        char **words = store_password_hashes(hashes);

        HashTable *password_hashes = new_hash_table(20);
        for(int i=0; i < strlen(hashes)/64; i++){
            printf("%d:    %s\n", i, words[i]);
            hash_table_put(password_hashes, words[i], (i+1));
        };
        generate_guesses_six(20, password_hashes);
        popular_character_guess_six(password_hashes);
        brute_force_six(password_hashes);
        for(int i=0; i < strlen(hashes)/64; i++){
            printf("%d:    %s\n", i, words[i]);
            free(words[i]);
        };
        free(words);
        free_hash_table(password_hashes);
        fclose(file);
    }
}

int main(int argc, char *argv[]){
    FILE *file;
    file = fopen("pwd4sha256", "r");
    read_hash_file_four(file);
    // file = fopen("pwd6sha256", "r");
    // read_hash_file_six(file);
    if (argc == 1){
        //generate guesses and test against SHA256 hashes
    }
    else if(argc == 2){
        int n_guesses = argv[argc-1];
        // have n amount of guesses
    }
    else if(argc == 3){
        // read file and do stuff
    }
}
