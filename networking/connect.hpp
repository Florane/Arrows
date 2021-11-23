#pragma once
#include <boost/asio.hpp>
#include <array>

class Connection
{
protected:
    boost::asio::ip::tcp::socket* linkSocket;
public:
    void send_data(std::array<int,2> data);
    std::array<int,2> receive_data();
};

class Server : public Connection
{
public:
    Server();
};

class Client : public Connection
{
public:
    Client(std::string ip);
    Client() : Client("127.0.0.1") {};
};