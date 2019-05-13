#include <stdio.h>

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
void read_hash_file(FILE *file){
    int c;
    if (file){
        while((c = getc(file)) != EOF){
            putchar(c);
        }
        fclose(file);
    }
}


void generate_guesses(int n_guesses){
}