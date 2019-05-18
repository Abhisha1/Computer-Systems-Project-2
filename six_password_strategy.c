#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "hashtable.h"
#include "proj-2_sha256.h"
#include "six_password_strategy.h"
#include "sha256-helper.h"
#include "passwords.h"

void brute_force_six(HashTable *ht, Passwords *solved, int n_guesses){
    char brute_guess[6];
    SHA256_CTX ctx;
    int hash;
    if (remaining_hashes(ht) == 0 || n_guesses == 0){
        return;
    }
    for(int i=65;i < 127; i++){
        brute_guess[0] = (char)i;
        for(int j=65;j < 127; j++){
            brute_guess[1] = (char)j;
            for(int k=33;k < 127; k++){
                brute_guess[2] = (char)k;
                for(int l=33;l < 127; l++){
                    brute_guess[3] = (char)l;
                    for(int m=33;m < 127; m++){
                        brute_guess[4] = (char)m;
                        for(int n=33;n < 127; n++){
                            brute_guess[5] = (char)n;
                            brute_guess[6] = '\0';
                            sha256_init(&ctx);
                            sha256_update(&ctx, (BYTE*)brute_guess, strlen(brute_guess));
                            BYTE guess[32];
                            sha256_final(&ctx, guess);
                            char* hex_guess = sha256_byteToHexString(guess);
                            if ((hash = hash_table_get(ht, hex_guess))>0){
                                printf("%s %d\n", brute_guess, hash);
                                add_new_cracked(solved, brute_guess);
                                printf("remaining passwords are : %d\n", remaining_hashes(ht));
                                n_guesses = generate_common_subs_six(solved, n_guesses, ht);
                            }
                            free(hex_guess);
                            n_guesses--;
                            if (remaining_hashes(ht) == 0 || n_guesses == 0){
                                return;
                            }
                        }
                    }
                }
            }
        }
        if(i == 127){
            i = 33;
        }
        else if(i == 64){
            break;
        }
    }
}
int popular_character_guess_six(HashTable *ht, Passwords *solved,int n_guesses){
    printf("popular character guess strategy\n");
    char brute_guess[4];
    SHA256_CTX ctx;
    int hash;
    char line[20];
    FILE *file = fopen("common_password_frequency.txt", "r");
    char frequent_characters[60];
    int index = 0;
    if (remaining_hashes(ht) == 0 || n_guesses == 0){
        fclose(file);
        return 0;
    }
    while (fgets(line, sizeof(line), file)){
        line[1] = '\0';
        frequent_characters[index] = line[0];
        index++;
    }
    for(int i = 0;i < index; i++){
        brute_guess[0] = frequent_characters[i];
        for(int j = 0;j < index; j++){
            brute_guess[1] = frequent_characters[j];
            for(int k = 0;k < index; k++){
                brute_guess[2] = frequent_characters[k];
                for(int l = 0;l < index; l++){
                    brute_guess[3] = frequent_characters[l];
                    for(int m = 0;m < index; m++){
                        brute_guess[4] = frequent_characters[m];
                        for(int n = 0;n < index; n++){
                            brute_guess[5] = frequent_characters[n];
                            brute_guess[6] = '\0';
                            sha256_init(&ctx);
                            sha256_update(&ctx, (BYTE*)brute_guess, strlen(brute_guess));
                            BYTE guess[32];
                            sha256_final(&ctx, guess);
                            char* hex_guess = sha256_byteToHexString(guess);
                            // printf("%s\n", brute_guess);
                            if ((hash = hash_table_get(ht, hex_guess))>0){
                                printf("%s %d\n", brute_guess, hash);
                                add_new_cracked(solved, brute_guess);
                                printf("remaining passwords are : %d\n", remaining_hashes(ht));
                                n_guesses = generate_common_subs_six(solved,n_guesses,ht);
                            }
                            free(hex_guess);
                            n_guesses--;
                            if (remaining_hashes(ht) == 0 || n_guesses == 0){
                                return n_guesses;
                            }
                        }
                    }
                }
            }
        }
    }return n_guesses;
}





int generate_guesses_six(int n_guesses, HashTable *ht, Passwords *solved){
    FILE* file = fopen("proj-2_common_passwords.txt", "r");
    char line[20];
    SHA256_CTX ctx;
    int hash;
    printf("start generate");
    while (fgets(line, sizeof(line), file) && remaining_hashes(ht) > 0 && n_guesses != 0){
        line[6] = '\0';
        // printf("%s\n", line);
        sha256_init(&ctx);
        sha256_update(&ctx, (BYTE*)line, strlen(line));
        BYTE guess[32];
        sha256_final(&ctx, guess);
        char* hex_guess = sha256_byteToHexString(guess);
        if ((hash = hash_table_get(ht, hex_guess))>0){
            printf("%s %d\n", line, hash);
            add_new_cracked(solved, line);
            printf("remaining passwords are : %d\n", remaining_hashes(ht));
            n_guesses = generate_common_subs_six(solved,n_guesses,ht);
        }
        n_guesses--;
        free(hex_guess);
    }
    printf("finished file");
    fclose(file);
    return n_guesses;
}