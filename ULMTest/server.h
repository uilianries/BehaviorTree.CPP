#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class AsioSession
{
public:
    AsioSession(tcp::socket socket)
        : socket_(std::move(socket))
    {
    }

    bool recv(std::string* rec);
    bool send(const std::string& msg);

private:

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

//class AsioServer
//{
//public:
//    AsioServer(boost::asio::io_service& io_service, short port)
//        : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
//          socket_(io_service)
//    {
//        std::cout << "Waiting for first and only connection" << std::endl;
//        acceptor_.accept(socket_, [this](const boost::system::error_code& ec)
//        {
//            if (!ec)
//            {
//                std::make_shared<AsioSession>(std::move(socket_));
//            }
//        });
//    }

//private:

//    std::shared_ptr<AsioSession> session_;
//    tcp::acceptor acceptor_;
//    tcp::socket socket_;
//};


#endif // SERVER_H
