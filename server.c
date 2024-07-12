#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "common.h"


typedef struct
{
    int sockClient;

}CLIENT_PARAM;

void* Server_client_handler(void* param);


int main(int argc, char* argv[])
{
    int sockSrv, sockClient;
    struct sockaddr_in saServer = { 0 }, saClient = { 0 };
    sockSrv = socket(AF_INET, SOCK_STREAM, 0);

    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    saServer.sin_port = htons(CONTROL_PORT);

    listen(sockSrv, SOMAXCONN);

    printf("[server] Waiting for incoming connections at port %d...\n", ntohs(saServer.sin_port));

    while (1)
    {
        socklen_t len = sizeof(struct sockaddr_in);
        sockClient = accept(sockSrv, (struct sockaddr*)&saClient, &len);
        if (sockClient >= 0)
        {
            printf("[server] accepted connection from client\n");

            CLIENT_PARAM* pParam = (CLIENT_PARAM*)malloc(sizeof(CLIENT_PARAM));
            if (pParam)
            {
                printf("[server] allocated memory for CLIENT_PARAM\n");

                pParam->sockClient = sockClient;
                pthread_t thread_id = 0;

                if (pthread_create(&thread_id, NULL, Server_client_handler, (void*)pParam) < 0)
                {
                    printf("[server] could not create connection thread\n");
                    close(sockClient);
                    free(pParam);
                }

                pthread_detach(thread_id);
            }
        }
        else
        {
            printf("[server] accept error, fd = %d\n", sockClient);
        }
    }

    return 0;
}


void* Server_client_handler(void* param)
{

    printf("[Server_client_handler] begin");

    CLIENT_PARAM* pParam = (CLIENT_PARAM*)param;
    if (pParam == NULL)
        return NULL;

    int sockClient = pParam->sockClient;

    int nRead, nCmdIdx = 0;
    char szRead[255];
    char szCmdBuff[20480];
    szCmdBuff[0] = 0;

    while ((nRead = recv(sockClient, szRead, sizeof(szRead), 0)) > 0)
    {
        for (int i = 0; i < nRead; i++)
        {
            char c = szRead[i];

            switch (c)
            {
            case 0x0A:
                break;
            case 0x0D:
                szCmdBuff[nCmdIdx] = 0;
                nCmdIdx = 0;
                printf("[Server_client_handler] received command %s\n", szCmdBuff);
                break;
            default:
                szCmdBuff[nCmdIdx] = c;
                nCmdIdx++;

                if (nCmdIdx >= (int)(sizeof(szCmdBuff) - 1))
                    nCmdIdx = 0;
            }
        }
    }
    printf("[Server_client_handler] end. client closed socket\n");
    close(sockClient);
    free(pParam);
    return NULL;
}
