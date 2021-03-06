#include <boost/asio.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
using namespace std;
namespace asio = boost::asio;
namespace ip = asio::ip;

int client();
int server();

int main()
{
    char c;
    cout << "クライアント？サーバー？c/s\n>";
    cin >> c;
    if (c == 'c')
    {
        client();
    }
    else if (c == 's')
    {
        server();
    }
}

int client()
try
{
    asio::io_service io_service;

    //TCPソケットを作成する
    ip::tcp::socket sock(io_service);

    //ホスト情報を取得
    cout << "サーバー名\n>";
    string server_name;
    cin >> server_name;
    cout << "ポート番号\n>";
    u_short port = 0;
    cin >> port;
    sock.connect(ip::tcp::endpoint(ip::address::from_string(server_name), port));

    //メッセージを送受信
    string buffer;
    while (true)
    {

        asio::streambuf receive_buffer;
        boost::system::error_code error;
        asio::read(sock, receive_buffer, asio::transfer_at_least(1), error);
        if (error && error != asio::error::eof)
        {
            cout << "receive failed: " << error.message() << endl;
            break;
        }
        buffer = boost::asio::buffer_cast<const char*>(receive_buffer.data());
        asio::write(sock, asio::buffer(buffer));

        cout << "受信:" << buffer << endl;
    }

    return 0;
}
catch (exception& e)
{
    cout << e.what();
    return 1;
}

int server()
try
{
    asio::io_service io_service;

    //TCPソケットを作成する
    ip::tcp::socket sock(io_service);

    //IPv4のソケットアドレス情報を設定する
    cout << "ポート番号\n>";
    u_short port = 0;
    cin >> port;
    ip::tcp::acceptor acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(), port));

    //クライアントからの接続を受け入れる
    acceptor.accept(sock);

    //メッセージを送受信
    string buffer;
    while (true)
    {
        this_thread::sleep_for(std::chrono::milliseconds(100));
        buffer = "test";
        auto start = chrono::system_clock::now();
        asio::write(sock, asio::buffer(buffer));


        asio::streambuf receive_buffer;
        boost::system::error_code error;
        asio::read(sock, receive_buffer, asio::transfer_at_least(1), error);
        if (error && error != asio::error::eof)
        {
            cout << "receive failed: " << error.message() << endl;
            break;
        }
        auto end = chrono::system_clock::now();
        auto diff = end - start;
        cout << "elapsed time = "
             << chrono::duration_cast<chrono::nanoseconds>(diff).count()
             << " nanosec."
             << endl;
    }
    return 0;
}
catch (exception& e)
{
    cout << e.what();
    return 1;
}
