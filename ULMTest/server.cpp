#include "server.h"


bool AsioSession::recv(std::string *rec)
{
    socket_.non_blocking(true);
    boost::system::error_code ec;
    std::size_t length = socket_.read_some(boost::asio::buffer(data_, max_length), ec);

    if ( !ec )
    {
        *rec = std::string( (const char*)data_, length);
        return true;
    }
    else if( ec == boost::asio::error::would_block)
    {
        return false;
    }
    else{
        throw std::runtime_error(ec.message());
    }
}


bool AsioSession::send(const std::string& msg)
{
    boost::system::error_code ec;
    boost::asio::write(socket_, boost::asio::buffer(msg.data(), msg.length()), ec );

    if (!ec)
    {
        return true;
    }
    else{
        std::cout << ec.message() << std::endl;
        return false;
    }
}


