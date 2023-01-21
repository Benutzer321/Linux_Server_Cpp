#ifndef _MANAGER_SERVER
#define _MANAGER_SERVER 1

#define BUFF_SIZE 4096

#include "Client.cpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>


namespace ez_soc{

    int create_tcp_socket_ipv4(int Port){
        
        int soc = socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK, 0);

        if(soc == -1)
            return -1;

        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(Port);
        inet_pton(AF_INET,"0.0.0.0", &hint.sin_addr);

        if(bind(soc, (sockaddr*)&hint,sizeof(hint))){
            std::cout << "Bindingerror" << std::endl;
            return -1;
        }
        
        if(listen(soc,10)){
            std::cout << "Listening error" << std::endl;
            return -1;
        }
        return soc;
    };

    int read_socket(int socket, void* dest, size_t n){

        char buffer[BUFF_SIZE];
        ssize_t rtvallue = 0;
        size_t wbytes = 0;

        memset(buffer, 0, BUFF_SIZE);

        while (n != wbytes){

            if((n - wbytes < BUFF_SIZE))
                rtvallue = recv(socket,buffer,n - wbytes,MSG_DONTWAIT);
            else
                rtvallue = recv(socket,buffer,BUFF_SIZE,0);

            if (rtvallue < 0){
                if(errno == EAGAIN | errno == EWOULDBLOCK)
                    break;
                std::cerr << "Unexpected error on Socket" << std::endl;
                return -1;
                }
            
            if (rtvallue == 0)
                break;
            
            strncpy(((char*)dest + wbytes),buffer,rtvallue);
            wbytes += rtvallue;

            memset(buffer,0,BUFF_SIZE);
        };

        return wbytes;
    };

    int write_socket(int socket, void* msg, size_t n){

        ssize_t rtvallue = 0;
        size_t sbytes = 0;

        while (sbytes != n){
            rtvallue = send(socket,((char*)msg + sbytes),(n - sbytes),MSG_NOSIGNAL | MSG_DONTWAIT);

            if(rtvallue < 0){
                if(errno == EAGAIN | errno == EWOULDBLOCK)
                    break;

                if(errno == ENOTCONN | errno == ECONNRESET | errno == EPIPE)
                    std::cerr << "Verbindung verloren " << std::endl;
                return -1;
                }

            if(rtvallue == 0)
                break;

            sbytes += rtvallue;
        }

        return sbytes;
    }

    int get_ipv4(Client* C){

        int rvalue = getpeername(C->socket,(sockaddr*)&C->addr,& C->addrlen);

        if(rvalue < 0){
            if (errno == ENOTCONN)
                return -1;
            
            std::cerr << "Fehler bei ermitlung des Peernamens errno:" << errno << std::endl;
            return -2;}
        
        C->ip = C->addr.sin_addr;

        return rvalue;
    };

    int get_ipv4(int fd,sockaddr_in* addr, socklen_t* clientSize){

        int rvalue = getpeername(fd,(sockaddr*)addr,clientSize);

        if(rvalue < 0){
            if (errno == ENOTCONN)
                return -1;

            std::cerr << "Fehler bei ermitlung des Peernamens errno:" << errno << std::endl;
            return -2;}

        return rvalue;
    };

};


#endif