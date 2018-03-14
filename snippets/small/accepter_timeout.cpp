#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/bind.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

class Client {
    asio::io_service& io_service_;
    tcp::socket socket_;
    asio::streambuf receive_buff_;

    asio::steady_timer timer_; // タイムアウト用のタイマー
    bool is_canceled_;

public:
    Client(asio::io_service& io_service)
        : io_service_(io_service),
          socket_(io_service),
          timer_(io_service),
          is_canceled_(false)
    {}

    void start()
    {
        connect();
    }

private:
    // 接続
    void connect()
    {
        socket_.async_connect(
                tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 31400),
                boost::bind(&Client::on_connect, this, asio::placeholders::error));
    }

    // 接続完了
    void on_connect(const boost::system::error_code& error)
    {
        if (error) {
            std::cout << "connect failed : " << error.message() << std::endl;
            return;
        }

        start_receive();
    }

    // メッセージ送信
    void start_receive()
    {
        boost::asio::async_read(
            socket_,
            receive_buff_,
            asio::transfer_all(),
            boost::bind(&Client::on_receive, this,
                        asio::placeholders::error, asio::placeholders::bytes_transferred));

        // 5秒でタイムアウト
        //timer_.expires_from_now(std::chrono::seconds(5));
        timer_.expires_from_now(std::chrono::milliseconds(500));
        timer_.async_wait(boost::bind(&Client::on_timer, this, _1));
    }

    // 受信完了
    // error : エラー情報
    // bytes_transferred : 送信したバイト数
    void on_receive(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (error == asio::error::operation_aborted) {
            std::cout << "タイムアウト" << std::endl;
        }
        else {
            // タイムアウトになる前に処理が正常終了したのでタイマーを切る
            // タイマーのハンドラにエラーが渡される
            timer_.cancel();
            is_canceled_ = true;

            if (error) {
                std::cout << "その他のエラー : " << error.message() << std::endl;
            }
            else {
                std::cout << "受信成功" << std::endl;
            }
        }
    }

    // タイマーのイベント受信
    void on_timer(const boost::system::error_code& error) {
        if (!error && !is_canceled_) {
            socket_.cancel(); // 通信処理をキャンセルする。受信ハンドラがエラーになる
        }
    }
};

int main()
{
    asio::io_service io_service;
    Client client(io_service);

    client.start();

    io_service.run();
}
