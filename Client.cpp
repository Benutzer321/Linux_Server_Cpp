#ifndef _CLIENT_SERVER
#define _CLIENT_SERVER 1


#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <cstring>
#include <netdb.h>
#include <unordered_map>

#define MAX_CLIENTS 255

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

class Clients{

private:
    std::unordered_map<std::string,Client> ClientMap;
    pollfd FDArr[MAX_CLIENTS];
    size_t client_n;

private:

public:
    pollfd* get_fd(){}


public:


};







#endif
