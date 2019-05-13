
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
    printf("rnning main\n");
    int g,p;

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
    char privKey[5];
    // memset(privKey,0,0);
    memcpy(privKey, argv[2], 2);
    privKey[2] = '\0';
    printf("privKey is %s\n\n", privKey);
    // char hex[10];
    // snprintf(hex, 2, "0x%s",privKey);
    // printf("hex is %s\n\n", hex);
    int b = (int)strtol(privKey, NULL, 16);
    printf("b is %d\n\n", b);
    portno = 7800;

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
    printf("created scoket\n");
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
    printf("send username\n");
    char *username = "nirmalathasa"; 
    n = write(sockfd, username, strlen(username));
    printf("n  is %d\n", n);
    n = write(sockfd, "\n", 1);
    printf("n  is %d\n", n);
    if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(EXIT_FAILURE);
        }
    printf("g: %d, b: %d, p: %d\n", g,b,p);
    // long int numerator = (int)pow(g,b);
    // printf("numerator: %ld\n", numerator);
    // int val = numerator%p;
    int val = modulo(g,b,p);
    printf("my key is is %d\n", val);
    char value[100];
    sprintf(value,"%d", val);
    for(int i=0; i < strlen(value); i++){
        printf("value  is %s\n", value);
        n = write(sockfd, value, 1);
        if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(EXIT_FAILURE);
        }
    }
    n = write(sockfd, "\n", 1);
    printf("n is %d\n", n);
    while(1){
        n = read(sockfd, buffer, 255);
        printf("n is %d\n", n);
        if(!strcmp(buffer,"\n")){
            buffer[n] = 0;
            break;
        }
        buffer[n] = 0;
    }
    buffer[n] = 0;
    int responseDiffie = atoi(buffer);
    printf("response is %d\n", responseDiffie);
    char sec[100];
    sprintf(sec,"%d",modulo(val, responseDiffie, p));
    printf("final is %s\n", sec);
    for(int i=0; i < strlen(value); i++){
        n = write(sockfd, value, 1);
        if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(EXIT_FAILURE);
        }
    }
    n = write(sockfd, "\n", 1);
    n = read(sockfd, buffer, 255);
    printf("response message is %s\n", buffer);
    close(sockfd);

    return 0;
}