#include "connect.hpp"
#define tcp boost::asio::ip::tcp

void Connection::send_data(std::array<int,2> data)
{
    boost::system::error_code ignored;
    boost::asio::write(*linkSocket, boost::asio::buffer(data), ignored);
}

std::array<int,2> Connection::receive_data()
{
    std::array<int,2> data;
    boost::system::error_code error;

    size_t len = boost::asio::read(*linkSocket,boost::asio::buffer(data,sizeof(data)), error);

    return data;
}

Server::Server()
{
    boost::asio::io_context context;
    tcp::acceptor acceptor(context, tcp::endpoint(tcp::v4(), 8080));
    linkSocket = new tcp::socket(context);
    acceptor.accept(*linkSocket);
}

Client::Client(std::string ip)
{
    boost::asio::io_context context;
    tcp::resolver resolver(context);
    tcp::resolver::results_type endpoints = resolver.resolve(ip.c_str(), "8080");
    linkSocket = new tcp::socket(context);
    boost::asio::connect(*linkSocket, endpoints);
}