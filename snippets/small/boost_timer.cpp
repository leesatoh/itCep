#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

namespace asio = boost::asio;
namespace chrono = std::chrono;

void on_timer(const boost::system::error_code& ec)
{
    if (!ec) {
        std::cout << "on timer" << std::endl;
    }
}

int main()
{
    asio::io_service io_service;
    asio::steady_timer timer(io_service); // chronoベースのタイマー

    timer.expires_from_now(chrono::milliseconds(100)); // chronoのdurationを渡す
    timer.async_wait(on_timer);

    io_service.run();
}
