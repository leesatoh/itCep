#include <iostream>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

int main()
{
    asio::io_service io_service;

    tcp::acceptor acc(io_service, tcp::endpoint(tcp::v4(), 31400));
    tcp::socket socket(io_service);

    boost::system::error_code error;
    acc.accept(socket, error);

    if (error) {
        std::cout << "accept failed: " << error.message() << std::endl;
    }
    else {
        std::cout << "accept correct!" << std::endl;
    }
}
