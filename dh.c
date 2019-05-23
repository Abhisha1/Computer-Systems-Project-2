
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <math.h>
#include <unistd.h>
int modulo(int a, int b, int n){
    // adapted from Wikipedia pow explanation
    long long x=1, y=a; 
    while (b > 0) {
        if (b%2 == 1) {
            x = (x*y) % n; // multiplying with base
        }
        y = (y*y) % n; // squaring the base
        b /= 2;
    }
    return x % n;
}

int main(int argc, char *argv[]){
    int g,p;

    g = 15;

    p = 97;

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent * server;

    char buffer[256];

    int b = atoi(argv[1]);
    portno = 7800;

    /* Translate host name into peer's IP address ;
     * This is name translation service by the operating system */
    server = gethostbyname("172.26.37.44");

    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(EXIT_FAILURE);
    }

    /* Building data structures for socket */

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    bcopy(server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(portno);

    /* Create TCP socket -- active open
     * Preliminary steps: Setup: creation of active open socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        exit(EXIT_FAILURE);
    }

    /* Sends username*/
    char *username = "nirmalathasa"; 
    n = write(sockfd, username, strlen(username));
    n = write(sockfd, "\n", 1);
    if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(EXIT_FAILURE);
        }
    /* Sends secret*/
    int client_secret = modulo(g,b,p);
    char secret[100];
    sprintf(secret,"%d", client_secret);
    for(int i=0; i < strlen(secret); i++){
        n = write(sockfd, &secret[i], 1);
        if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(EXIT_FAILURE);
        }
    }
    n = write(sockfd, "\n", 1);
    /* Recieves secret*/
    n = read(sockfd, buffer, 255);
    if(!strncmp(buffer,"\n",1)){
        buffer[n] = 0;
    }
    buffer[n] = 0;
    /* Applies b to sent message*/
    int responseDiffie = atoi(buffer);
    char shared_value[100];
    sprintf(shared_value,"%d",modulo(responseDiffie, b, p));
     for(int i=0; i < strlen(shared_value); i++){
        n = write(sockfd, &shared_value[i], 1);
        if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(EXIT_FAILURE);
        }
    }
    n = write(sockfd, "\n", 1);
    n = read(sockfd, buffer, 255);
    printf("%s\n", buffer);
    close(sockfd);

    return 0;
}