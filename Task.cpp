
#ifndef SERVER_TASKS
#define SERVER_TASKS 1


#include "HTTP_Manager.cpp"

namespace ez_soc{

typedef HTTP::H_HTTP (*Function)(HTTP::H_HTTP*); 

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

            HTTP::H_HTTP process(HTTP::H_HTTP _h){

                if (Type & HTTP_Function)
                    return Func(&_h);
                
                if(Type & HTTP_Pipe)
                    return _h;

                if(Type & HTTP_Socket)
                    return _h;

                return _h;
            }
    };
} // namespace ez_soc
#endif