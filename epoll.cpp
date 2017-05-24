#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>

using namespace std;

#define MAXLINE 5
#define OPEN_MAX 100
#define LISTENQ 20
// #define SERV_PORT 5000
// #define INFTIM 1000

void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0)
    {
        perror("fcntl(sock, F_GETFl)");
        exit(1);
    }
    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0)
    {
        perror("fcntl(sock, F_SETFL, opts)");
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    int i, maxi, listenfd, connfd, sockfd, epfd, nfds, portnumber;
    size_t n;
    char line[MAXLINE];
    socklen_t clilen;

    if (2 == argc)
    {
        if ((portnumber = atoi(argv[1])) < 0)
        {
            fprintf(stderr, "Usage: %s portnumber\n", argv[0]);
            return 1;
        } 
    }
    else
    {
        fprintf(stderr, "Usage: %s portnumber\n", argv[0]);
        return 1;
    }

    epfd = epoll_create(OPEN_MAX);
    struct epoll_event ev, events[20];
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    listenfd = socket(AF_INET,SOCK_STREAM, 0);
    setnonblocking(listenfd);

    ev.data.fd = listenfd;
    ev.events = EPOLLIN|EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    char* local_addr = (char*)"127.0.0.1";
    inet_aton(local_addr, &(serveraddr.sin_addr));
    serveraddr.sin_port = htons(portnumber);

    bind(listenfd, (sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);

    maxi = 0;
    for (;;)
    {
        nfds = epoll_wait(epfd, events, 20, 500);

        for (i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == listenfd)
            {
                connfd = accept(listenfd, (sockaddr*)&clientaddr, &clilen);
                if (connfd < 0)
                {
                    perror("connfd < 0");
                    exit(1);
                }

                char* str = inet_ntoa(clientaddr.sin_addr);
                cout << "accept a connection from " << str << endl;

                ev.data.fd = connfd;
                ev.events = EPOLLIN|EPOLLET;

                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
            }
            else if (events[i].events & EPOLLIN)
            {
                cout << "EPOLLIN" << endl;

                if ((sockfd = events[i].data.fd) < 0)
                    continue;
                if ((n = read(sockfd, line, MAXLINE)) < 0)
                {
                    if (errno == ECONNRESET)
                    {
                        close(sockfd);
                        events[i].data.fd = -1;
                    }
                    else
                    {
                        cout << "read error"<< endl;
                    }
                }
                else if (n == 0)
                {
                    close(sockfd);
                    events[i].data.fd = -1;
                }

                line[n-1] = '\0';
                cout << "read " << line << endl;

                ev.data.fd = sockfd;
                ev.events = EPOLLOUT | EPOLLET;

                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
            else if (events[i].events & EPOLLOUT)
            {
                sockfd = events[i].data.fd;
                write(sockfd, line, n);

                ev.data.fd = sockfd;
                ev.events=EPOLLIN|EPOLLET;

                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
        }
    }

    return 0;
}