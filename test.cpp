#include <iostream>
#include <chrono>
#include <ctime>

int main(int argc, char const *argv[])
{
    auto x = std::chrono::system_clock::now();
    std::time_t y = std::chrono::system_clock::to_time_t(x);

    std::cout << std::ctime(&y) << std::endl;
    return 0;
}
