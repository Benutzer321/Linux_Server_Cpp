#ifndef _HANDLER_SERVER
#define _HANDLER_SERVER 1

#include "Client.cpp"
#include "Managed_Socket.cpp"

#include <unistd.h>
#include <iostream>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <queue>
#include <mutex>
#include <condition_variable>

const static size_t MAX_CLIENTS = 255;


class Handler{
private:

    pollfd fd_arr[MAX_CLIENTS];
    Client c_arr[MAX_CLIENTS];

    size_t top_fd = 0;

    std::queue<Client*> Q;

    std::mutex p_mutex;
    std::condition_variable p_condition;

    bool close_socket = false;

private:

void push(Client* C){

    std::unique_lock<std::mutex> lock(p_mutex);
    Q.push(C);
    p_condition.notify_all();

}

public:

    size_t Port;

public:

    Handler(int socket, size_t _Port):top_fd(1),Port(_Port){

        fd_arr[0] = {socket, POLLIN, 0};

        Client c = Client();
        c.port = _Port;

        for(size_t i = 1; i < MAX_CLIENTS; i++){
            fd_arr[i] = {-1, POLLIN, 0};
            c.spot = i;
            c_arr[i] = c;
        };
    };

    void main(){

        while (!close_socket){

            int ready = poll(fd_arr,top_fd,10000);

            if(ready < 0)
                break;;

            if(ready == 0)
                continue;

            if(fd_arr[0].revents){
                if(accept_socket() < 0)
                    break;
                ready--;}


            for(int i = 1; ready ; i++){
                if(fd_arr[i].revents){
                    push(&c_arr[i]);
                    fd_arr[i].fd = -1;
                    ready--;
                };
            };

        };

        for(int i = 0; i < MAX_CLIENTS; i++){
            if (fd_arr[i].fd == -1)
                continue;
            close(fd_arr[i].fd);
        };

        std::cout << "EXIT" << std::endl;
        
    };
    
    int accept_socket(){

        if (top_fd >= MAX_CLIENTS)
            return 0;

        int rvallue = accept(fd_arr[0].fd, (sockaddr*)& c_arr[top_fd].addr, &c_arr[top_fd].clientSize);
        
        if(rvallue < 0){
            std::cerr << "Fehler beim Akzeptieren des Clients errno:" << errno << std::endl;
            return 0;
        };

        fd_arr[top_fd].fd = rvallue;
        c_arr[top_fd].socket = rvallue;
        
        rvallue = ez_soc::get_ipv4(&c_arr[top_fd]);
        if(rvallue < 0){
            if(rvallue == -1){
                std::cerr << "Verbindung verloren" << std::endl;
                return 0;}

            std::cerr << "Fehler beim ermitteln der IP addresse des Clients errno:" << errno << std::endl;
            return -1;}
        
        std::cout <<std::endl << "Client mit der IP:" << inet_ntoa(c_arr[top_fd].addr.sin_addr) << " hat sich verbunden" <<std::endl <<std::endl;
        top_fd ++;

        return 0;
    }

    Client* get_client(){

        std::unique_lock<std::mutex> lock(p_mutex);
        while (Q.empty()){
            p_condition.wait(lock);
            if(close_socket){
                //kp ob des funktioniert
                exit(0);
            }
        };

        Client* value(std::move(Q.front()));
        Q.pop();
        return value;
    }

    void return_client(Client * C){
        *C->socketptr = C->socket;
    }

    void drop_client(Client * C){

        if(C->spot == top_fd - 1){
            for(size_t i = 1; i < MAX_CLIENTS + 1; i++)
                if(fd_arr[top_fd - i].fd != -1){
                    top_fd = top_fd - i + 1;
                    break;
                    };
            };

        fd_arr[C->spot].fd -1;
        close(C->socket);
        C->socket = -1;

        };
    
    void kill(){

        close_socket = true;

        p_condition.notify_all(); 
        
        close(fd_arr[0].fd);  
        fd_arr[0].fd = -1;
    }
};

#endif