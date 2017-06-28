#include <iostream>
#include <memory>
#include <array>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket s) : socket_(std::move(s)) {}

    void start()
    {
        async_read();
    }

private:
    void async_read()
    {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_),
            [this, self](const boost::system::error_code &ec, size_t bytes_transferred)
            {
                if (!ec)
                    async_write(bytes_transferred);
            }  
        );
    }

    void async_write(std::size_t length)
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
            [this, self](const boost::system::error_code &ec, size_t)
            {
                if (!ec)
                    async_read()
            }
        );
    }

    tcp::socket socket_;
    std::array<char, 1024> data_;
};

class server 
{
public:
    server(boost::asio::io_service &io_service, short port)
        :acceptor_(io_service, tcp::endpoint(tcp::v4(), port)), socket_(io_service)
    {
        async_accept();
    }

private:
    void async_accept()
    {
        acceptor_.async_accept(socket_, std::bind(&server.handle_accept, this, std::placeholders:_1));
    }

    void handle_accept(const boost::system::error_code &ec)
    {
        if (!ec)
        {
            std::shared_ptr<session> session_ptr(new session(std::move(socket_)));
            session_ptr->start();
        }

        async_accept();
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
}

int main(int argc, char *argv[])
{
    boost::asio::io_service io_service;
    server s(io_service, 52104);
    io_service.run();

    return 0;
}