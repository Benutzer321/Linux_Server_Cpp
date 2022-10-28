#ifndef _HTTP_SERVER
#define _HTTP_SERVER 1


#include <unordered_map>
#include <string>
#include <cstring>
#include <iostream>
#include <string>


// GET /tttt HTTP/1.1  /19
// Host: localhost:42069 /22
// User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:104.0) Gecko/20100101 Firefox/104.0
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
// Accept-Language: de,en-US;q=0.7,en;q=0.3
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive
// Upgrade-Insecure-Requests: 1
// Sec-Fetch-Dest: document
// Sec-Fetch-Mode: navigate
// Sec-Fetch-Site: none
// Sec-Fetch-User: ?1 */





namespace ez_http {

    class Header{
        size_t size_in_Bytes;
        void* value;
    };


    class H_HTTP{

        private:

            bool p_error;

            std::string msg, method, path, request;

            std::unordered_map<std::string, std::string> cookies;
            
        private:

            bool check_completion(){
                
                if(msg.rfind("\r\n\r\n") != std::string::npos)
                    return true;
                return false;
            }

            bool get_path(){

                size_t tmp = msg.find(' '), tmp2 = msg.find(' ',tmp + 1), tmp3 = msg.find('\n');

                if(tmp == std::string::npos | tmp2 == std::string::npos | tmp3 == std::string::npos)
                    return false;
                if(tmp >= tmp3 | tmp2 >= tmp3)
                    return false;
                
                method = msg.substr(0,tmp);
                path = msg.substr(tmp + 1, tmp2 - tmp);
                request = msg.substr(0, tmp3);

                return true;
                }

            bool get_header(){

                size_t tmp1 = msg.find('\n') + 1, tmp2 = 0, tmp3 = 0;

                while (true){

                    tmp2 = msg.find(':', tmp1);
                    if(tmp2 == std::string::npos)
                        break;

                    tmp3 = msg.find('\n', tmp2);
                    if(tmp3 == std::string::npos)
                        break;

                    cookies.insert({msg.substr(tmp1, tmp2 - tmp1 ), msg.substr(tmp2 + 2, tmp3 - 1 - (tmp2 + 1))});
    
                    tmp1 = tmp3 + 1;
                }
                return true;
            }

        public:


        public:

            bool error(){
                return p_error;
            }

            void print(){
                
                std::cout << "Method/Path: " << method << ' ' << path << std::endl;

                for( auto i = cookies.begin(); i != cookies.end(); ++i)
                    std::cout << i->first << ": " << i->second <<std::endl;

                return;
            };

            H_HTTP(char* _msg, size_t _msg_size){
            
            p_error = 0;

            msg = std::string(_msg,_msg_size);

            if(!check_completion())
                p_error = p_error | 1;
                
            if(!get_path())
                p_error = p_error | 2;

            if(!get_header())
                p_error = p_error | 4;
            };

            ~H_HTTP(){
            }





    };


    typedef int (*Function)(H_HTTP); 

    struct Task{

        #define HTTP_Function 1
        #define HTTP_Pipe 2
        #define HTTP_Socket 4

        private:

        unsigned int Type;
        Function Func;
        int sock_pipe;

        private:

            
        public:

            Task(int _dest, unsigned int _Type): sock_pipe(_dest), Type(_Type){};

            Task(Function _dest, unsigned int _Type): Func(_dest), Type(_Type){};

            H_HTTP process(H_HTTP _h){

                if (Type & HTTP_Function)
                    Func(_h);
                
                if(Type & HTTP_Pipe)
                    return _h;

                if(Type & HTTP_Socket)
                    return _h;

                return _h;
            }

    };
    
}

#endif


