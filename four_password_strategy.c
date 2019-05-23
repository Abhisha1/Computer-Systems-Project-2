#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "hashtable.h"
#include "proj-2_sha256.h"
#include "four_password_strategy.h"
#include "sha256-helper.h"
#include "passwords.h"
void brute_force_four(HashTable *ht, Passwords* solved){
    // Checks every password combination and guesses
    char brute_guess[4];
    SHA256_CTX ctx;
    int hash;
    // Ensures all passwords not already guessed or haven't guessed all guesses
    if (remaining_hashes(ht) == 0 || get_remaining_guesses(solved) == 0){
        return;
    }
    // Tries every combination of characters, starting from A
    for(int i=65;i < 127; i++){
        brute_guess[0] = (char)i;
        for(int j=33;j < 127; j++){
            brute_guess[1] = (char)j;
            for(int k=33;k < 127; k++){
                brute_guess[2] = (char)k;
                for(int l=33;l < 127; l++){
                    brute_guess[3] = (char)l;
                    brute_guess[4] = '\0';
                    sha256_init(&ctx);
                    sha256_update(&ctx, (BYTE*)brute_guess, strlen(brute_guess));
                    BYTE guess[32];
                    sha256_final(&ctx, guess);
                    char* hex_guess = sha256_byteToHexString(guess);
                    if ((hash = hash_table_get(ht, hex_guess))>0){
                        printf("%s %d\n", brute_guess, hash);
                        add_new_cracked(solved, brute_guess);
                        // Tries generating guesses with common substituions for guessed password
                        generate_common_subs_four(solved,ht);
                        if (get_remaining_guesses(solved) == 0 || remaining_hashes(ht) == 0){
                            return;
                        }
                    }
                    free(hex_guess);
                    made_guess(solved);
                    if (remaining_hashes(ht) == 0 || get_remaining_guesses(solved) == 0){
                        return;
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

int popular_character_guess_four(HashTable *ht, Passwords* solved){
    // Gusses passwords using common password file frequency
    char brute_guess[4];
    SHA256_CTX ctx;
    int hash;
    char line[20];
    FILE *file = fopen("common_password_frequency.txt", "r");
    char frequent_characters[60];
    int index = 0;
    // Ensures all passwords havent been guessed
    if (remaining_hashes(ht) == 0 || get_remaining_guesses(solved) == 0){
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
                    brute_guess[4] = '\0';
                    sha256_init(&ctx);
                    sha256_update(&ctx, (BYTE*)brute_guess, strlen(brute_guess));
                    BYTE guess[32];
                    sha256_final(&ctx, guess);
                    char* hex_guess = sha256_byteToHexString(guess);
                    // Checks if guess matched password hash
                    if ((hash = hash_table_get(ht, hex_guess))>0){
                        printf("%s %d\n", brute_guess, hash);
                        add_new_cracked(solved, brute_guess);
                        //Finds common substituion guesses
                        generate_common_subs_four(solved,ht);
                        if (get_remaining_guesses(solved) == 0 || remaining_hashes(ht) == 0){
                            return 0;
                        }
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
    return get_remaining_guesses(solved);
}

int generate_guesses_four(HashTable *ht, Passwords* solved){
    // Guesses common passwords from common password file
    FILE* file = fopen("proj-2_common_passwords.txt", "r");
    char line[20];
    SHA256_CTX ctx;
    int hash;
    while (fgets(line, sizeof(line), file) && remaining_hashes(ht) > 0 && get_remaining_guesses(solved) != 0){
        line[4] = '\0';
        sha256_init(&ctx);
        sha256_update(&ctx, (BYTE*)line, strlen(line));
        BYTE guess[32];
        sha256_final(&ctx, guess);
        char* hex_guess = sha256_byteToHexString(guess);
        // Checks for matching password
        if ((hash = hash_table_get(ht, hex_guess))>0){
            printf("%s %d\n", line, hash);
            add_new_cracked(solved, line);
            // Checks for common substituion with guessed password
            generate_common_subs_four(solved,ht);
            if (get_remaining_guesses(solved) == 0 || remaining_hashes(ht) == 0){
                return 0;
            }
        }
        made_guess(solved);
        free(hex_guess);
    }
    fclose(file);
    return get_remaining_guesses(solved);
}