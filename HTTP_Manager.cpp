#ifndef _HTTP_SERVER
#define _HTTP_SERVER 1


#include <unordered_map>
#include <string>
#include <cstring>
#include <iostream>
#include <string>
#include <chrono>


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





namespace HTTP{

    class H_HTTP{

        private:

            char info [4] = "200";

            bool p_error;

            std::string msg, method, path, request;

            std::unordered_map<std::string, std::string> Request_Header, Respond_Cookies;
            
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
                for (short i = 0; i != 2; )
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

                    Request_Header.insert({msg.substr(tmp1, tmp2 - tmp1 ), msg.substr(tmp2 + 2, tmp3 - 1 - (tmp2 + 1))});
    
                    tmp1 = tmp3 + 1;
                }
                return true;
            }

        public:


        public:
            static bool check_completion(char * _Buff, size_t _n){
            std::string msg(_Buff,_n);  
                if(msg.rfind("\r\n\r\n") != std::string::npos)
                    return true;
                return false;
            }

            bool error(){
                return p_error;
            }

            void print(){
                
                std::cout << "Method/Path: " << method << " / " << path << std::endl;

                std::cout << "Headername: Value" <<std::endl;

                for( auto i = Request_Header.begin(); i != Request_Header.end(); ++i)
                    std::cout << i->first << ": " << i->second <<std::endl;

                return;
            };

            std::string Respond(char * _Buff, size_t _n){

                std::string Header;
                char top[16] = "HTTP/1.1 XXX OK";
                strncpy(&top[9],info,3);

                char Header1[] = "Date:";

                Header.append(info);

                //temp
                char* info_str = "OK";
                Header.append(info_str);
                


            }

            std::string get(std::string _Key){
                if (Request_Header.contains(_Key))
                    return Request_Header[_Key];

                if (Respond_Cookies.contains(_Key))
                    return Respond_Cookies[_Key];
                    
                return "";
            }
            bool set( std::string _Key, std::string _Value){

                if(Request_Header.contains(_Key))
                    return false;
                
                Respond_Cookies[_Key] = _Value;
                return true;
            }

            const std::string operator[](std::string _Key){

                if (Request_Header.contains(_Key))
                    return Request_Header[_Key];

                if (Respond_Cookies.contains(_Key))
                    return Respond_Cookies[_Key];
                    
                return "";
            }

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

            ~H_HTTP(){}
    };
}

#endif


