#ifndef _WORKER_SERVER
#define _WORKER_SERVER 1


#include "Client.cpp"
#include "Handler.cpp"
#include "Managed_Socket.cpp"
#include "HTTP_Manager.cpp"

#include <bitset>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unordered_map>
#include <string>
#include <cstring>

#define MAX_MESSAGE_SIZE 8192

class Worker{

private:

    Handler* H;
    Client* C;

    ssize_t rBytes, sBytes;

    char Buffer[MAX_MESSAGE_SIZE];

public:
    Worker(Handler* _H): H(_H){};

    ~Worker(){};

    void main(){
        std::cout << "Arbeiter am Start" << std::endl;

        while (true){
            
            rBytes = 0;
            sBytes = 0;
            memset(Buffer,0,MAX_MESSAGE_SIZE);

            C = H->get_client();

            rBytes = ez_soc::read_socket(C->socket,Buffer,MAX_MESSAGE_SIZE);

            if(rBytes == -1){
                std::cerr << "Reading error \n";
                H->drop_client(C);
                continue;
                }

            if(rBytes == 0){
                H->drop_client(C);
                continue;
            }
            if(rBytes == MAX_MESSAGE_SIZE){
                if(!HTTP::H_HTTP::check_completion(Buffer,rBytes)){
                    std::string return_msg = "HTTP/1.1 431 Request Header Fields Too Large\n";
                    sBytes = ez_soc::write_socket(C->socket,(char*)return_msg.c_str(), return_msg.size());
                }
            }
            
            HTTP::H_HTTP Http_handler(Buffer,rBytes);
            
            if(Http_handler.error())
                std::cout << std::bitset<16> (Http_handler.error()) <<std::endl;
            
            Http_handler.print();
            
            //temporär
            sBytes =  ez_soc::write_socket(C->socket,Buffer,rBytes);

            H->drop_client(C);
        }
    std::cout << "Arbeiter meldet sich ab" << std::endl;
    }
};









#endif