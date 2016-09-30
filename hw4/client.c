/* Cassidy Brown - cmb195
 * OS HW4 - 10/3/16
 * Server-client method
 * adapted from provided client.c example 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <math.h>

double shubert(double x1, double x2);

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    double x1, x2, y, min = 0;

    /* Arbitrary port number */
    portno = 8020;

    /* Socket stuff */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname("127.0.0.1");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Connect to server */
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    /* Do half the shubert computations */
    for (x1 = 0.5; x1 <= 2.0; x1 += 0.5) {
        for (x2 = -2; x2 <= 2; x2 += 0.5) {
            y = shubert(x1, x2);
            printf("%.2f ", y);
            if (y < min)
                min = y;
        }
        printf("\n");
    }

    /* Convert min to a string for writing */
    sprintf(buffer, "%f", min);

    /* Write to server */
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");

    close(sockfd);
    return 0;
}

/* Shubert function, as provided */
double shubert(double x1, double x2) {
    double sum1 = 0; 
    double sum2 = 0;
    int i;
    for (i = 1; i <= 5; i++) {
        sum1 += i * cos((i + 1) * x1 + i);
        sum2 += i * cos((i + 1) * x2 + i);
    }
    return sum1 * sum2;
}