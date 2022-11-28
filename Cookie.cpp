#ifndef _HTTP_COOKIE
#define _HTTP_COOKIE 1

#include <iostream>
#include <string>
#include <cstring>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cmath>


namespace HTTP{

    static double Standart_Cookie_Expiration_h = 48;

    class Cookie{

        private:

            std::string Name;

            std::string Value;

            double expiers_h;

            std::string Domain;
            std::string Path;

            bool secure = false;
            bool HttpOnly = false;

            
        
        private:

            std::string get_expiration_date(){
                
                time_t rawtime;
                time(&rawtime);

                rawtime += expiers_h * 60 * 60;

                std::string expiration;

                char expirationdate_str[30], temp[25];
                memset(expirationdate_str,0,30);

                //asctime Www Mmm dd hh:mm:ss yyyy
                strcpy(temp,asctime(localtime(&rawtime)));
                
                strcpy(expirationdate_str,"Wdy, DD Mon YYYY HH:MM:SS CET");

                strncpy(expirationdate_str,temp,3);
                strncpy(&expirationdate_str[5],&temp[8],2);
                strncpy(&expirationdate_str[8],&temp[4],3);
                strncpy(&expirationdate_str[12],&temp[20],4);
                strncpy(&expirationdate_str[17],&temp[11],8);

                return std::string(expirationdate_str);
            }


        public:

        const std::string expiration;

        public:

            void domain(std::string _Domain){ Domain = _Domain; };

            void path(std::string _Path){ Path = _Path; };

            std::string get_cookie_str(){
                
                std::string Kecks;

                Kecks = "Set-Cookie: " + Name + '=' + Value;

                Kecks +=  "; Expiers=" + get_expiration_date();

                if(Path.size() > 0)
                    Kecks += "; Path=" + Path ;
                
                if(Domain.size() > 0)
                    Kecks += "; Domain=" + Domain;

                if(secure)
                    Kecks += "; Secure";

                if(HttpOnly)
                    Kecks += "; HttpOnly";
                
                
                return Kecks;
                }


            Cookie(std::string _name, std::string _value): Name(_name),Value(_value),expiers_h(Standart_Cookie_Expiration_h){};

            Cookie(std::string _name, std::string _value, double _h): Name(_name),Value(_value),expiers_h(_h){}


    };
};

int main(int argc, char const *argv[])
{
    HTTP::Cookie C("eric","20");
    C.domain("Google.de");
    C.path("/");

    std::cout << C.get_cookie_str() << std::endl;
    
    return 0;
}


#endif
