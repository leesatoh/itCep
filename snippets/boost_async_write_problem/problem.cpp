#include <iostream>
#include <vector>
#include <boost/asio.hpp>
//#include <boost/asio/spawn.hpp>
#include <boost/system/error_code.hpp>

int main()
{
    using namespace boost;
    namespace ip = asio::ip;
    using tcp = ip::tcp;

    asio::io_service io_service{};
    auto const endpoint = tcp::endpoint{ip::address::from_string("127.0.0.1"), 12345};

/*    // server
    asio::spawn(io_service, [&](asio::yield_context yield) {
        auto acceptor = tcp::acceptor{io_service, endpoint};
        auto sock = tcp::socket{io_service};
        acceptor.async_accept(sock, yield);
        auto ec = system::error_code{};
        while (!ec) {
            auto buffer = std::array<char, 128>{};
            auto size = sock.async_read_some(asio::buffer(buffer), yield[ec]);
            std::cout.write(buffer.data(), size) << std::endl;
        }
    });
    io_service.run_one(); // start accept
*/
    auto sock = tcp::socket{io_service};
    boost::asio::strand mystrand(io_service);
    sock.connect(endpoint);
	
    sock.set_option(tcp::acceptor::send_buffer_size{128});

    auto buffer1 = std::vector<char>(256*1024*1024, 'A');
    auto buffer2 = std::vector<char>(256*1024*1024, 'B');
    auto counter = 0;
    asio::async_write(sock, asio::buffer(buffer1)
            , mystrand.wrap([&](system::error_code, std::size_t) {
        if (++counter == 2) sock.close();
    }));
    asio::async_write(sock, asio::buffer(buffer2)
            , mystrand.wrap([&](system::error_code, std::size_t) {
        if (++counter == 2) sock.close();
    }));

    io_service.run();
}
