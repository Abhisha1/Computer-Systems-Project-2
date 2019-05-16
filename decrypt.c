#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


#include "hashtable.h"
#include "proj-2_sha256.h"
char** store_password_hashes(char* hashes){
    char **password_hashes = (char**)malloc((strlen(hashes)/64)*sizeof(char*));
    assert(password_hashes);
    memset(password_hashes,0,(strlen(hashes)/64));
    for(int i=1; i <= strlen(hashes)/64; i++){
        password_hashes[i-1] = malloc(sizeof(char)*(64+1));
        assert(password_hashes[i-1]);
        memset(password_hashes[i-1],0,strlen(password_hashes[i-1]));
        snprintf(password_hashes[i-1], 65, "%s",hashes + (i-1)*64);
    }
    return password_hashes;
}
char *sha256_byteToHexString(BYTE data[]) {
	char *hexC = "0123456789abcdef";
	char *hexS = malloc(65);
	if (!hexS) return NULL;
	for(BYTE i; i<32; i++) {
		hexS[i*2]   = hexC[data[i]>>4];
		hexS[i*2+1] = hexC[data[i]&0xF];
	}
	hexS[64] = 0;
	return hexS;
}
void brute_force_four(HashTable *ht){
    printf("brute force\n");
    char brute_guess[4];
    SHA256_CTX ctx;
    int hash;
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
                    sha256_update(&ctx, brute_guess, strlen(brute_guess));
                    BYTE guess[32];
                    sha256_final(&ctx, guess);
                    char* hex_guess = sha256_byteToHexString(guess);
                    // printf("%s\n", brute_guess);
                    if ((hash = hash_table_get(ht, hex_guess))>0){
                        printf("%s %d\n", brute_guess, hash);
                        printf("remaining passwords are : %d\n", remaining_hashes(ht));
                    }
                    if (remaining_hashes(ht) == 0){
                        break;
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
void brute_force_six(HashTable *ht){
    char brute_guess[6];
    SHA256_CTX ctx;
    int hash;
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
                            sha256_update(&ctx, brute_guess, strlen(brute_guess));
                            BYTE guess[32];
                            sha256_final(&ctx, guess);
                            char* hex_guess = sha256_byteToHexString(guess);
                            if ((hash = hash_table_get(ht, hex_guess))>0){
                                printf("%s %d\n", brute_guess, hash);
                                printf("remaining passwords are : %d\n", remaining_hashes(ht));
                            }
                            if (remaining_hashes(ht) == 0){
                                break;
                            }
                        }
                    }
                }
            }
            if(j== 127){
                j= 33;
            }
            else if(j== 64){
                break;
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
void generate_guesses_four(int n_guesses, HashTable *ht){
    FILE* file = fopen("proj-2_common_passwords.txt", "r");
    char line[20];
    SHA256_CTX ctx;
    int hash;
    while (fgets(line, sizeof(line), file) && remaining_hashes(ht) > 0){
        line[4] = '\0';
        // printf("%s\n", line);
        sha256_init(&ctx);
        sha256_update(&ctx, line, strlen(line));
        BYTE guess[32];
        sha256_final(&ctx, guess);
        char* hex_guess = sha256_byteToHexString(guess);
        if ((hash = hash_table_get(ht, hex_guess))>0){
            printf("%s %d\n", line, hash);
            printf("remaining passwords are : %d\n", remaining_hashes(ht));
        }
    }
    printf("finished file");
    fclose(file);
}
void generate_guesses_six(int n_guesses, HashTable *ht){
    FILE* file = fopen("proj-2_common_passwords.txt", "r");
    char line[20];
    SHA256_CTX ctx;
    int hash;
    printf("start generate");
    while (fgets(line, sizeof(line), file) && remaining_hashes(ht) > 0){
        line[6] = '\0';
        // printf("%s\n", line);
        sha256_init(&ctx);
        sha256_update(&ctx, line, strlen(line));
        BYTE guess[32];
        sha256_final(&ctx, guess);
        char* hex_guess = sha256_byteToHexString(guess);
        if ((hash = hash_table_get(ht, hex_guess))>0){
            printf("%s %d\n", line, hash);
            printf("remaining passwords are : %d\n", remaining_hashes(ht));
        }
    }
    printf("finished file");
    fclose(file);
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
        generate_guesses_four(10, password_hashes);
        brute_force_four(password_hashes);
        for(int i=0; i < strlen(hashes)/64; i++){
            printf("%d:    %s\n", i, words[i]);
            free(words[i]);
        };
        free(words);
        free_hash_table(password_hashes);
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
    // file = fopen("pwd4sha256", "r");
    // read_hash_file_four(file);
    file = fopen("pwd6sha256", "r");
    read_hash_file_six(file);
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
