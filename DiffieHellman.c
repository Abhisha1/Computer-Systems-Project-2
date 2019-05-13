
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
int modulo(int a, int b, int n){
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
    
    long int g,p;

    g = 15;

    p = 97;

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent * server;

    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    portno = atoi(7800);

    /* Translate host name into peer's IP address ;
     * This is name translation service by the operating system */
    server = gethostbyname(argv[1]);

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

    /* Do processing */
    char *username = "nirmalathasa"; 
    n = write(sockfd, username, strlen(username));
    n = write(sockfd, "\n", 1);
    if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(EXIT_FAILURE);
        }
    long int b = argv[2];
    int val = (g^b)%p;
    char *value = itoa(val);
    for(int i=0; i < strlen(value); i++){
        n = write(sockfd, value[i], 1);
        if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(EXIT_FAILURE);
        }
    }
    n = write(sockfd, "\n", 1);
    while(1){
        n = read(sockfd, buffer, 255);
        if(buffer =="\n"){
            buffer[n] = 0;
            break;
        }
        buffer[n] = 0;
    }
    buffer[n] = 0;
    int responseDiffie = atoi(buffer);
    printf("response is %s\n", responseDiffie);
    int sec = itoa(modulo(value, responseDiffie, p));
    for(int i=0; i < strlen(value); i++){
        n = write(sockfd, value[i], 1);
        if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(EXIT_FAILURE);
        }
    }
    n = write(sockfd, "\n", 1);
    n = read(sockfd, buffer, 255);
    printf("response message is %d\n", buffer);
    close(sockfd);

    return 0;
}