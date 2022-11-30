#ifndef _HTTP_RESPONSE
#define _HTTP_RESPONSE 1


#include <iostream>

#include <string>
#include <cstring>

#include <unordered_map>
#include <map>
#include <list>
#include <vector>

#include <fstream>
#include <sys/stat.h>

#include "Cookie.cpp"


namespace HTTP{

    static std::map<std::string,std::string> Response_Codes = {{"200", "ok"},{"201", "Created"},{"202", "Accepted"},{"204","No Content"},
                                                                    { "300", "Multiple Choices"},
                                                                    { "400", "Bad Request"}, {"401", "Unauthorized"},{"403", "Forbidden"}, {"404", "Not Found"}};
    
    static std::map<std::string, std::string> Content_Type = {{".css", "text/css"}, {".csv", "text/csv"}, {".html", "text/html"}, {".xlm", "text/xml"},
                                                              {".js","application/javascript"}, {".json", "application/json"}, {".pdf", "application/pdf"},{".zip", "application/zip"}};
    
    const static size_t MAX_FILE_SIZE = 1000000;

    class Response{
        
        private:

            typedef std::string str;

            char Code[4] = "200";

            std::map<std::string, std::string> Headers;

            std::unordered_map<std::string, Cookie> Cookies;

            char* Body;
            size_t Body_Size;

            std::string Filetype;

        private:

            inline bool file_exists(const std::string & _path){
                struct stat buffer;
                return (stat(_path.c_str(),&buffer) == 0);

            }

            ssize_t read_File(const char * _Path, size_t maxBuffsize = MAX_FILE_SIZE){
                
                std::ifstream MyFile(_Path, std::ios::in | std::ios::binary | std::ios::ate);
                ssize_t Size;
                if(!MyFile.is_open())
                    return -1;

                Size = MyFile.tellg();
                MyFile.seekg(0, std::ios::beg);
                if( Size > maxBuffsize)
                    return -2;
                
                Body = new char[Size];

                MyFile.read(Body, Size);

                MyFile.close();

                return Size;    
            }
        
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

        void body(char* _body, size_t _size, std::string _filetype = ".html"){
            Body = new char[_size];
            strncpy(Body,_body,_size);
            Filetype = _filetype;
        };

        ssize_t body_file(str _path){

            if(!file_exists(_path))
                return -1;
            
            Filetype = _path.substr(_path.rfind('.'), _path.size() - _path.rfind('.'));

            return read_File(_path.c_str());    
        }

        Cookie& ad_cookie(str _Name){
            if(Cookies.count(_Name))
                return Cookies.at(_Name);
            Cookies.insert({_Name,{_Name,NULL}});
            return Cookies.at(_Name);
        }

        Cookie& operator[](str _Name){ return ad_cookie(_Name); };

    };
};

#endif