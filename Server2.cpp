#ifndef _SERVER_SERVER
#define _SERVER_SERVER 1

#include "Handler.cpp"

#include "Managed_Socket.cpp"

#include "Worker.cpp"

#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
 
#include <pthread.h>


#define MAX_SOCKETS 2

class Server{

    private:

    size_t n_socket = 0;
    
    Handler* socket_handler[MAX_SOCKETS];
    Worker* socket_worker[MAX_SOCKETS];

    bool close_socket[MAX_SOCKETS];
    
    pthread_t p_socket[MAX_SOCKETS];
    pthread_t* p_worker[MAX_SOCKETS];


    private:

        static void* Start_Handler(void* Argument){
            Handler * Handl = (Handler*)Argument;

            std::cout << "Socket auf Port:" << Handl->Port << " nimt nun Verbindungen an" <<std::endl;

            Handl->main();
            return Argument;
        }

        static void* Start_Worker(void* Argument){
            Worker W = Worker((Handler*)Argument);
            W.main();
            return Argument;
        }

    public:

    public:

    Server(){};

    ~Server(){

        for(int i = 0; i < n_socket; i++)
            socket_handler[i]->kill();

    };

    void add_Port(size_t Port, size_t Worker){
        
        int soc = ez_soc::create_tcp_socket_ipv4(Port);

        if(soc == -1)
            return;

        socket_handler[n_socket] = new Handler(soc, Port);
        
        pthread_create(&p_socket[n_socket],NULL,Start_Handler,socket_handler[n_socket]);

        p_worker[n_socket] = new pthread_t[Worker - 1];
        for(int i = 0; i < Worker; i++)
            pthread_create(&p_worker[n_socket][i],NULL,Start_Worker, socket_handler[n_socket]);
        
        n_socket ++;

    };
};

int main(){
    
    Server s;

    s.add_Port(42069,2);

    std::cin.get();
}

#endif