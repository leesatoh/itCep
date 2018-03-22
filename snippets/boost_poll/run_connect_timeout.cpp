/**
* If you want to success, do command. 
* nc -l 31400 
*/
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

namespace asio = boost::asio;

class Client {
    asio::io_service& io_service_;
    asio::ip::tcp::socket socket_;
    asio::deadline_timer connect_timer_;
public:
    Client(asio::io_service& io_service)
        : io_service_(io_service),
          socket_(io_service),
          connect_timer_(io_service)
    {}

    void connect()
    {
        socket_.async_connect(
            asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.10"), 31400),
            boost::bind(&Client::on_connect, this, _1));

        // 5秒でタイムアウト
        connect_timer_.expires_from_now(boost::posix_time::seconds(5));
        connect_timer_.async_wait(boost::bind(&Client::on_connect_timeout, this, _1));
    }

    void on_connect(const boost::system::error_code& error)
    {
        if (error) {
            std::cout << "接続失敗: " << error.message() << std::endl;
        }
        else {
            // タイムアウトのタイマーを切る :
            // タイムアウトのハンドラにboost::asio::error::operation_abortedエラーが渡される
            connect_timer_.cancel();

            std::cout << "接続成功" << std::endl;
        }
    }

    void on_connect_timeout(const boost::system::error_code& error)
    {
        if (!error) {
            // タイムアウト : 接続を切る。接続のハンドラがエラーになる
            socket_.close();
        }
    }
};

int main()
{
    asio::io_service io_service;

    Client client(io_service);
    client.connect();

    io_service.run();
}
