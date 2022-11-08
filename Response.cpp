#ifndef _HTTP_RESPONSE
#define _HTTP_RESPONSE 1


#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <map>
#include <list>


namespace HTTP{

    const static std::map<std::string,std::string> Response_Codes = {{"200", "ok"},{"201", "Created"},{"202", "Accepted"},{"204","No Content"},
                                                                    {"300", "Multiple Choices"},
                                                                    {"400", "Bad Request"}, {"401", "Unauthorized"},{"403", "Forbidden"}, {"404", "Not Found"}};
    

    class Response{
        

        private:

            typedef std::string str;

            char Code[4] = "200";

            char* Body;
            size_t Body_Size;


            std::map<std::string, std::string> Headers;
            std::unordered_map<std::string, std::string> Content;

        private:


        public:

            

        public:

        void set_ResponseCode(str _code){
            if (_code.size() != 4)
                return;
            strcpy(Code,_code.c_str());
        };

        void set_ResponseCode(int _code){
            if(_code <= 99 | _code >= 600)
                return;
            strcpy(Code, std::to_string(_code).c_str());
            }
        
        std::pair<std::string,std::string> get_ResponseCode(){
            if(Response_Codes.count(Code))
                return std::pair<std::string,std::string>(Code, Response_Codes.at(Code));
            return {Code, "Definition not Found"};
        } 

        str& operator [](str _key){
            if(!Content.count(_key))
                    Content.insert({_key,str("")});
                return Content[_key];
                };
        
        
        str get_var(str _key){
            if(!Content.count(_key))
                return str("");
            return Content[_key];
        }

        str& add_Header(str _header, str _V){
            Headers[_header] = _V;
            return Headers[_header];
        }

        str get_Header(str _header){
            if(!Headers.count(_header))
                return str("");
            return Headers[_header];
        }
    };

};





#endif