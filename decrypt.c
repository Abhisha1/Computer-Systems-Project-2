#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


#include "hashtable.h"
#include "four_password_strategy.h"
#include "six_password_strategy.h"
#include "passwords.h"
#include "sha256-helper.h"
char** store_password_hashes(char* hashes){
    /**
     *  Stores the string of password hashes into an array
     * */
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

int read_hash_file_four(char *file_name, int n_guesses){
     /**
     *  Reads a file_name containing passwords of length 4 and makes n_guesses
     *  Run with -1 when there is no specified number of guesses
     * */
    FILE *file;
    file = fopen(file_name, "r");
    if (file){
        char hashes[4000];
        memset(hashes, 0, 4000);
        int n=0;
        int c;
        while((c = getc(file)) != EOF){
            n +=sprintf(hashes+n,"%02x", c);
        }
        // printf("%s",hashes);
        char **words = store_password_hashes(hashes);

        HashTable *password_hashes = new_hash_table(strlen(hashes)/64);
        for(int i=0; i < strlen(hashes)/64; i++){
            hash_table_put(password_hashes, words[i], (i+1));
        };
        
        Passwords *cracked_passwords = create_passwords(strlen(hashes)/64, n_guesses);
        // Tries cracking passwords based on common passwords
        generate_guesses_four(password_hashes, cracked_passwords);
        // Tries cracking passwords based on highly frequent character passwords
        popular_character_guess_four(password_hashes, cracked_passwords);
        // Checks all combinations of four passwords
        brute_force_four(password_hashes, cracked_passwords);
        n_guesses = get_remaining_guesses(cracked_passwords);
        for(int i=0; i < strlen(hashes)/64; i++){
            free(words[i]);
        };
        free(words);
        free_hash_table(password_hashes);
        free_passwords(cracked_passwords);
        fclose(file);
    }
    return n_guesses;
}


void read_hash_file_six(char *file_name, int n_guesses){
    /**
     *  Reads a file_name containing passwords of length 6 and makes n_guesses
     *  Run with -1 when there is no specified number of guesses
     * */
    FILE *file;
    file = fopen(file_name, "r");
    if (file){
        char hashes[4000];
        memset(hashes, 0, 4000);
        int n=0;
        int c;
        while((c = getc(file)) != EOF){
            n +=sprintf(hashes+n,"%02x", c);
        }
        char **words = store_password_hashes(hashes);

        //creates hashtable of # of passwords
        HashTable *password_hashes = new_hash_table(strlen(hashes)/64);
        for(int i=0; i < strlen(hashes)/64; i++){
            hash_table_put(password_hashes, words[i], (i+10+1));
        };
        Passwords *cracked_passwords = create_passwords(strlen(hashes)/64, n_guesses);
        // Tries cracking passwords based on common passwords
        generate_guesses_six("proj-2_common_passwords.txt", password_hashes, cracked_passwords);
         // Tries cracking passwords based on highly frequent character passwords
        popular_character_guess_six(password_hashes, cracked_passwords);
        // Checks all combinations of four passwords
        brute_force_six(password_hashes, cracked_passwords);
        for(int i=0; i < strlen(hashes)/64; i++){
            free(words[i]);
        };
        free(words);
        free_hash_table(password_hashes);
        fclose(file);
    }
}

void check_hashed_passwords(char *password_list, int n_guesses,char *file_name){
    /**
     *  Reads a file_name containing hashed passwords of assumed length 6 and a list of plaint text passwords which it is checked against
     * */
    FILE *file;
    file = fopen(file_name, "r");
    if (file){
        char hashes[4000];
        memset(hashes, 0, 4000);
        int n=0;
        int c;
        while((c = getc(file)) != EOF){
            n +=sprintf(hashes+n,"%02x", c);
        }
        char **words = store_password_hashes(hashes);

        //creates hashtable of # of passwords
        HashTable *password_hashes = new_hash_table(strlen(hashes)/64);
        for(int i=0; i < strlen(hashes)/64; i++){
            hash_table_put(password_hashes, words[i], (i+1));
        };
        // Passwords to check against hashes
        FILE* pwrd_file = fopen(password_list, "r");
        char line[100000];
        memset(line, 0, 100000);
        SHA256_CTX ctx;
        int hash;
        while (fgets(line, sizeof(line), pwrd_file) && remaining_hashes(password_hashes) > 0){
            line[6] = '\0';
            
            sha256_init(&ctx);
            sha256_update(&ctx, (BYTE*)line, strlen(line));
            BYTE guess[32];
            sha256_final(&ctx, guess);
            char* hex_guess = sha256_byteToHexString(guess);
            // Print matching passwords
            if ((hash = hash_table_get(password_hashes, hex_guess))>0){
                printf("%s %d\n", line, hash);
            }
            free(hex_guess);
        }
        fclose(pwrd_file);
        free(words);
        free_hash_table(password_hashes);
    }
    fclose(file);
}

int main(int argc, char *argv[]){
    if (argc == 1){
        //generate guesses and test against SHA256 hashes
        read_hash_file_four("pwd4sha256", -1);
        read_hash_file_six("pwd6sha256", -1);
    }
    else if(argc == 2){
        int n_guesses = atoi(argv[argc-1]);
        
        // have n amount of guesses
        n_guesses = read_hash_file_four("pwd4sha256", n_guesses);
        if (n_guesses>0){
            read_hash_file_six("pwd6sha256", n_guesses);
        }
    }
    else if(argc == 3){
        // Checks the hash of passwords with provided password file
        check_hashed_passwords(argv[1], -1,argv[2]);
    }
}