#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"

int Client_SendDummyCommand()
{
    int bRet = 0;
    int sockfd, res;
    struct sockaddr_in server_addr = {0};
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("[client] socket created %d\n", sockfd);
    if (sockfd != -1) 
    {
        // Define server address
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(CONTROL_PORT);

        // Convert IPv4 and IPv6 addresses from text to binary form
        inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

        // Connect to the server
        printf("[clinet] connecting\n");
        res = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
        printf("[client] connect res %d\n", res);
        if (res == 0)
        {
            strcpy(buffer, "HELLO:\r\n");

            // Send message to the server
            res = send(sockfd, buffer, strlen(buffer), 0);
            printf("[client] send HELLO, send len %d\n",res);

            bRet = 1;

        }

        // Close the socket
        close(sockfd);
        printf("[client] socket closed\n");
    }

    return bRet;
}



int main(void)
{
   while(1)
   {

       Client_SendDummyCommand();
   }


   return 0;
}
