/* Cassidy Brown - cmb195
 * OS HW4 - 10/3/16
 * Server-client method
 * adapted from provided server.c example 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>

double shubert(double x1, double x2);

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    double x1, x2, y, min, cli_min = 0;
     
    /* Open socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));

    /* Arbitrary port number */
    portno = 8020;

    /* Socket stuff*/
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    /* Listen for client */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, 
                (struct sockaddr *) &cli_addr, 
                &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");
    bzero(buffer,256);

    /* Read from client */
    n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    /* Convert recieved value to double */
    cli_min = atof(buffer);

    /* Close sockets */
    close(newsockfd);
    close(sockfd);


    /* Do half the shubert computations */
    for (x1 = -2; x1 <= 0; x1 += 0.5) {
        for (x2 = -2; x2 <= 2; x2 += 0.5) {
            y = shubert(x1, x2);
            printf("%.2f ", y);
            if (y < min)
                min = y;
        }
        printf("\n");
    }

    /* Compare server's min with client's min */
    if(cli_min < min){
        min = cli_min;
    }

    /* Print net minimum */
    printf("min = %.2f\n", min);

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