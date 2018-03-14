#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>
#include "InterFaces.h"
using namespace std;
namespace asio = boost::asio;
namespace ip = asio::ip;


class cWorkClient : public IRunnable{
public:cWorkClient(string server_name, u_short server_port)
	:server_name_(server_name),
	server_port_(server_port)
{}
private:
	string server_name_;
	u_short server_port_;
public:
	void run() final {
		try{
		asio::io_service io_service;

		//TCPソケットを作成する
		ip::tcp::socket sock(io_service);
		sock.connect(ip::tcp::endpoint(ip::address::from_string(server_name_), server_port_));

		//メッセージを送受信
		string buffer;
		asio::streambuf receive_buffer;
		boost::system::error_code error;
		while (true)
		{
			asio::read(sock, receive_buffer, asio::transfer_at_least(1), error);
			if (error && error != asio::error::eof)
			{
				cout << "receive failed: " << error.message() << endl;
				break;
			}
//			cout << "Recv:" << boost::asio::buffer_cast<const char*>(receive_buffer.data()) << endl;
			asio::write(sock, receive_buffer);
			receive_buffer.consume(receive_buffer.size());
		}
		}
		catch (exception& e)
		{
			cout << e.what();
		}
	}
};
class cWorkServer : public IRunnable {
public:cWorkServer(u_short server_port, IMessage* message)
	:server_port_(server_port), message_(message)
{}
private:
	u_short server_port_;
	IMessage* message_;
public:
	void run() final {
		try{
		asio::io_service io_service;

		//TCPソケットを作成する
		ip::tcp::socket sock(io_service);

		ip::tcp::acceptor acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(), server_port_));

		//クライアントからの接続を受け入れる
		acceptor.accept(sock);

		//メッセージを送受信
		char buf[1024];
	//	int len;
		asio::streambuf receive_buffer;
		boost::system::error_code error;

		while (true)
		{
			this_thread::sleep_for(std::chrono::milliseconds(1000));
			//len = message_->get_message(buf, sizeof(buf));
			message_->get_message(buf, sizeof(buf));
			asio::write(sock, asio::buffer(std::string(buf)));
//			cout << "Send:" << buf << endl;


			asio::read(sock, receive_buffer, asio::transfer_at_least(1), error);
			if (error && error != asio::error::eof)
			{
				cout << "receive failed: " << error.message() << endl;
				break;
			}
			message_->show_elapsed_time(boost::asio::buffer_cast<const char*>(receive_buffer.data()));
			receive_buffer.consume(receive_buffer.size());
		}
		}
		catch (exception& e)
		{
			cout << e.what();
		}

	}
};
