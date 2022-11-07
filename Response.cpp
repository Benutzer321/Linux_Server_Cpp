#ifndef _HTTP_RESPONSE
#define _HTTP_RESPONSE 1


#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <map>
#include <list>


namespace HTTP{

    struct Value{

        void* Pointer;
        size_t Size;

        Value(void* _Pointer, size_t _Size_in_Bytes){
            Size = _Size_in_Bytes;
            Pointer = new void[Size];
            strncpy(Pointer,_Pointer,Size);

        };

        Value(std::string _str){
            Size = _str.size();
            Pointer = new char[Size];
            strncpy((char*)Pointer,_str.c_str(),Size);
            };

        Value():Pointer(nullptr),Size(NULL){}

        Value& operator=(const Value& other){
            if(this = &other)
                return this*;
            if(Size != other.Size){
                Size = other.Size;
                delete Pointer;
                Pointer = new void[Size];
            }
            std::copy(other.Pointer, other.Pointer + other.Size, Pointer)
            return *this;

            
        }

        ~Value(){
            if(Size)
                delete[] Pointer;
        }
        
    };

    class Response{
        

        private:

            typedef std::string str

            std::list<std::string> Response;
            std::map<std::string, Value> Headers;
            std::unordered_map<std::string, Value> Content;

        private:


        public:

        Value& operator [](str _key){
            if(!Content.count(key)){

                    Content.insert({key,Value(nullptr,0)});
                }
                return Content[key];
                };
        
        
        str get_var_str(str _key){
            if(!Content.count(key))
                return str("")
            return str(Content[_key].Pointer, Content[_key].Size)
        }

        size_t get_var_size(str _key){
            if(!Content.count(_key))
                return 0;
            return Content[_key].Size;
        }

        void add_Header(str _header, Value _V){
            Headers[_header] = _V;
        }

        Value& get_Header(str _header){
            if(!Headers.count(key))
                return {nullptr, NULL}   
        }
        

        





        size_t get_Response(char* _Buff, size_t _n, size_t _s = 0){

        }
        

        }
            


        public:




    };

};





#endif