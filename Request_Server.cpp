#ifndef _HTTP_COOKIE
#define _HTTP_COOKIE 1


#include <string>
#include <cstring>


namespace HTTP
{

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

    class Request_Server{

        private:
            
            std::string Request;

            std::string path;

        private:

            bool check_completion(){
                
                if(Request.rfind("\r\n\r\n") != std::string::npos)
                    return true;
                return false;
            };

        public:

        Request_Server(std::string _Request):Request(_Request){


        }

        public:
    
    };
} // namespace HTTP

#endif
