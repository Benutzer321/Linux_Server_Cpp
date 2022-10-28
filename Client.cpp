#ifndef _CLIENT_SERVER
#define _CLIENT_SERVER 1


#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <netdb.h>

struct Client
{

    int* socketptr;
    int socket;

    size_t spot;

    size_t port;

    in_addr ip;

    sockaddr_in addr;
    socklen_t addrlen;

    sockaddr_in client;
    socklen_t clientSize;

    Client():
        socketptr(nullptr),
        socket(-1),
        spot(-1),
        port(-1),
        clientSize(sizeof(client)),
        addrlen(sizeof(addr)){};
};


#endif
