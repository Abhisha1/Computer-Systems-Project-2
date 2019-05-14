#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
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

void read_hash_file(FILE *file){
    if (file){
        char hashes[650];
        int n=0;
        int c;
        while((c = getc(file)) != EOF){
            n +=sprintf(hashes+n,"%02x", c);
        }
        // printf("%s",hashes);
        char **words = store_password_hashes(hashes);
        for(int i=0; i < strlen(hashes)/64; i++){
            printf("%d:    %s\n", i, words[i]);
            free(words[i]);
        };
        free(words);
        fclose(file);
    }
}
void generate_guesses(int n_guesses){
}

int main(int argc, char *argv[]){
    FILE *file;
    file = fopen("pwd4sha256", "r");
    read_hash_file(file);
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
