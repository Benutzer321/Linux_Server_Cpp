#ifndef SOCK_HANDLER
#define SOCK_HANDLER

#include "Client.cpp"
#include "Managed_Socket.cpp"

#include <iostream>
#include <poll.h>
#include <sys/types.h>
#include <queue>
#include <unistd.h>
#include <vector>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace ez_sock{

struct Client_Info{

    int sock;
    sockaddr_in addr;
    socklen_t clientSize = sizeof(client);
    unsigned int ipv4;

};

    class fd_stack {

    private:

        int BLOCKSIZE = 50;

        int* Arr;

        size_t min, max, size;

        int* Verteilung;

    private:

        void resize_arr(int n){

            if(!n%BLOCKSIZE)
                n = (n/BLOCKSIZE + 1) * BLOCKSIZE;

            int* tmp = new int[size + n];

            memcpy(tmp,Arr,sizeof(int)*size);
            delete[] Arr;
            Arr = (int*)tmp;

            memset(&Arr[size+1],-1,n);

            tmp = new int[(size+n)/BLOCKSIZE];

            memcpy(tmp,Verteilung,sizeof(int)*((size+n)/BLOCKSIZE));
            delete[] Verteilung;

            Verteilung = tmp;

            memset(&Verteilung[size/BLOCKSIZE + 1],BLOCKSIZE,n/BLOCKSIZE);

            size += n;
        }
    public:

    public:

        const int* data(){return Arr;}

        size_t length(){return max;}

        int put(int val){

            int position;
            Arr[min] = val;
            position = min;

            if(min == max){
                min++;
                max++;
            }else{
                for(int j = min + 1; j < max; j++) {
                    if (Arr[j] == -1) {
                        min = j;
                        return position;
                    }
                }
                min = max;
            }
            if(max == size){
                resize_arr(BLOCKSIZE);
            }
            return position;
        }

        int pop(int pos){

            if(min > pos)
                min = pos;
            if(max == pos + 1)
                max--;
            if(pos > size)
                return -1;

            int tmp = Arr[pos];
            Arr[pos] = -1;
            return tmp;
        };

        fd_stack():min(0),max(0),size(BLOCKSIZE){

            Arr = new int[size];
            memset(Arr,-1,size*sizeof(int));
            Verteilung = new int [size/BLOCKSIZE];
            memset(Verteilung,BLOCKSIZE,size/BLOCKSIZE*sizeof(int));

        }
    };

#define MAX_TIMEOUT 1000

class Sock_Handler{

private:

    volatile bool Monitoring;

    std::queue<Client_Info> C_in;

    pollfd* FD_Arr;

    Client_Info* C_Arr;

    size_t min, max, size;

    size_t* Verteilung;

    std::queue<Client*> Q;

private:



    bool Manage_Arr(Client_Info C){

        if(max >= size)
            resize_arr(50);

        while(!C_in.empty()){

        }



        return true;

    }

    int accept(int fd){

        sockaddr_in addr;
        socklen_t clientSize = sizeof(client);

        int rvallue = accept(fd, (sockaddr*)& addr, &clientSize);

        if(rvallue < 0){
            std::cerr << "Fehler beim Akzeptieren des Clients errno:" << errno << std::endl;
            return -1;
        };

        if(rvallue < 0){
            if(rvallue == -1){
                std::cerr << "Verbindung verloren" << std::endl;
                return 0;
            }

            std::cerr << "Fehler beim ermitteln der IP addresse des Clients errno:" << errno << std::endl;
            return -1;}

        return rvallue;
    };

    Client_Info identify(int fd){

        sockaddr_in addr;
        socklen_t clientSize = sizeof(client);

        if (ez_soc::get_ipv4(fd,&addr,&clientSize) < 0)
            return {NULL,NULL,NULL,NULL};

        return {fd,addr,clientSize, addr.sin_addr};

    }

    void monitor(){

        Monitoring = True;

        int ready = 0;

        while (Monitoring){

            ready = ppoll(FD_Arr, max, -1, sigmask);

            if(ready <= 0)
                if (ready < 0)
                    break;
                else
                    continue;

            if(FD_Arr[0].revents){
                ready--;
                identify(accept(FD_Arr[0].fd));
            }

            for(int i = 1, j = 0; ready != 0 && i > max; i++)
                if(FD_Arr[i].revents) {
                    ready--;
                    identify(FD_Arr[i].fd);
                }
        }
    }

public:

    static void* LAUNCH(void* Handler){
        Sock_Handler * H = (Sock_Handler*)Handler;
        H->monitor();
    }

    Sock_Handler(int sock, size_t fd_n = 200):Monitoring(false),min(0),max(0),size(50){

        FD_Arr = new pollfd[50];
        C_Arr = new Client_Info[50];
        Verteilung = new size_t [1];
        size = 50;

        FD_Arr[0] = {sock, POLLIN, 0};
        min++;
        max++;

        for(int i = 1; i != size; i++)
            FD_Arr[i] = {-1,POLLIN, 0};



    };


public:
};
};

#endif