#include <iostream>
#include <thread>
#include <chrono>
#include "connect.hpp"

std::ostream& operator<< (std::ostream& stream, std::array<int,2> data)
{
    stream << data[0] << " " << data[1];
    return stream;
}

int main()
{
    Connection *in;
    in = new Client;
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    in->receive_data();
    std::cout << "1" << std::endl;
    in->receive_data();
    std::cout << "2" << std::endl;
    in->receive_data();
    std::cout << "3" << std::endl;
    return 0;
}