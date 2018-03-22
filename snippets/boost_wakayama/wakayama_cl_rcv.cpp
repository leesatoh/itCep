/*
* wakayamaライブラリによくある使い方
*/
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
namespace asio = boost::asio;
namespace ip = boost::asio::ip;

//クライアントソケットインターフェース
class IClient{
	public:
		virtual void Close()=0;
	private:
		virtual void AsyncRead()=0;
		virtual void OnAsyncRead(const boost::system::error_code& error)=0;
//		virtual void AsyncWrite(const std::string& sndbuf_)=0;
};
//クライアントソケット
class Client :IClient{
	asio::streambuf buf;
	ip::tcp::socket sock;
	public:
		Client(asio::io_service& ios, const std::string& addr, int port):sock(ios){
			sock.connect(ip::tcp::endpoint(ip::address::from_string(addr), port));
			AsyncRead();
		}
		void Close(){
			ShutdownRead();
			boost::system::error_code ec;
			sock.close(ec);
			if(ec){
				std::cout<<ec<<std::endl;
			}else{
				std::cout << "Close OK" << std::endl;
			}
		}
	private:
		void AsyncRead()
			{
				asio::async_read(sock, buf, asio::transfer_at_least(1), boost::bind(&Client::OnAsyncRead, this, asio::placeholders::error));
			}
		void ShutdownRead()
			{
				if(!sock.is_open()){	return;			}
				sock.shutdown(boost::asio::ip::tcp::socket::shutdown_receive);
				if(sock.available()){	return;		}
				boost::system::error_code errcode;
				auto len = asio::read(sock, buf, errcode);
				if(errcode && errcode != boost::asio::error::eof)
				{
					std::cout << errcode << std::endl;
				}else{
					if(len){
						RecieveDo();
					}
				}
			}
		void OnAsyncRead(const boost::system::error_code& errcode)
			{
				if(errcode)
				{
					if(errcode != boost::asio::error::eof){
						std::cout << errcode << std::endl;
					}
				}else{
					RecieveDo();
					// 続けて受信する
					AsyncRead();
				}
			}
		void RecieveDo(){
				// 受信したデータをstringに変換
				const std::string data(asio::buffer_cast<const char*>(buf.data()), buf.size());
				std::cout << data << std::endl;
				buf.consume(buf.size());
			}
			
};
//シグナル
class Signal{
	asio::signal_set signal_set;
	public:
		Signal(asio::io_service& ios)
		:signal_set(ios, SIGINT)
		{
			signal_set.async_wait(boost::bind(&Signal::OnCancel, this, asio::placeholders::error, boost::asio::placeholders::signal_number));
		}
	private:
		void OnCancel(const boost::system::error_code& error, int signal_number)
		{
		    if (!error && signal_number == SIGINT)
		      std::cout << "on cancel signal" << std::endl;
		   signal_set.get_io_service().stop();
		}
};

//メイン
int main()
{
	//on_cancelのときにソケットもクローズしたい
    asio::io_service ios;

    
	Signal sg(ios);
    Client cl(ios, "127.0.0.1", 31400);
    ios.run();
    cl.Close();
}
