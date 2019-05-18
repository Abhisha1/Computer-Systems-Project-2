#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


#include "hashtable.h"
#include "four_password_strategy.h"
#include "six_password_strategy.h"
#include "passwords.h"

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

void read_hash_file_four(char *file_name, int n_guesses){
    FILE *file;
    file = fopen(file_name, "r");
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
        n_guesses = generate_guesses_four(n_guesses, password_hashes, cracked_passwords);
        n_guesses = popular_character_guess_four(password_hashes, cracked_passwords, n_guesses);
        brute_force_four(password_hashes, cracked_passwords, n_guesses);
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


void read_hash_file_six(char *file_name, int n_guesses){
    FILE *file;
    file = fopen(file_name, "r");
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
        Passwords *cracked_passwords = create_passwords(20);
        n_guesses = generate_guesses_six(n_guesses, password_hashes, cracked_passwords);
        n_guesses = popular_character_guess_six(password_hashes, cracked_passwords, n_guesses);
        brute_force_six(password_hashes, cracked_passwords,n_guesses);
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
    if (argc == 1){
        //generate guesses and test against SHA256 hashes
        read_hash_file_four("pwd4sha256", -1);
        read_hash_file_six("pwd6sha256", -1);
    }
    else if(argc == 2){
        int n_guesses = atoi(argv[argc-1]);
        printf("remaining passwords are : %d\n", n_guesses);
        // have n amount of guesses
        read_hash_file_four("pwd4sha256", n_guesses);
        read_hash_file_six("pwd6sha256", n_guesses);
    }
    else if(argc == 3){
        // read file and do stuff
    }
}