#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "guessing.h"
#include <signal.h>

#define TCP_PORT 9001
#define NB_CLIENTS 30
#define TIMEOUT 30

int clisocket;

void sig_handler(int signum){
    close(clisocket);
    exit(0);
}

int main(int argc, char **argv)
{
    int srv;
    struct sockaddr_in servaddr;
    srv = socket(AF_INET, SOCK_STREAM, 0);

    if (srv == 0)
    {
        perror("Socket creation error...\n");
        return -1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(TCP_PORT);
    int addrlen = sizeof(servaddr);
    if ((bind(srv, (struct sockaddr *)&servaddr, addrlen)) < 0)
    {
        perror("Socket binding error...\n");
        fflush(stderr);
        return -1;
    }
    if (listen(srv, NB_CLIENTS) < 0)
    {
        perror("Listen failure");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }
    printf("Server listen on port %d\n", TCP_PORT);
    fflush(stdout);
    while (1)
    {
        clisocket = accept(srv, (struct sockaddr *)&servaddr, (socklen_t *)&addrlen);
        if (clisocket < 0)
        {
            perror("Connection failure\n");
            fflush(stderr);
        }
        pid_t pid;
        if ((pid = fork()) == 0)
        {
            close(srv);
            signal(SIGINT,sig_handler);
            dup2(clisocket, 0);
            dup2(clisocket, 1);
            dup2(clisocket, 2);
            srand(time(0));
            game_loop();
            close(clisocket);
            exit(0);
        }
        else
        {
            close(clisocket);
            if(fork() == 0){
                sleep(TIMEOUT);
                kill(pid,SIGINT);
                exit(0);
            }
        }
    }
}