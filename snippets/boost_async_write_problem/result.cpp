#include <iostream>
#include <vector>
#include <queue>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
namespace asio = boost::asio;
namespace ip = boost::asio::ip;


class Client{
	ip::tcp::socket sock;
	std::queue<std::vector<char>> sndque;
	public:
		Client(asio::io_service& ios, const std::string& addr, int port):sock(ios){
			sock.connect(ip::tcp::endpoint(ip::address::from_string(addr), port));
 		   sock.set_option(ip::tcp::acceptor::send_buffer_size{128});
		}
		void Write(std::vector<char>&& buf){
			auto is_empty = sndque.empty();
			sndque.push(std::move(buf));
		
			if(is_empty){
			    asio::async_write(sock, asio::buffer(sndque.front()), 
		                    boost::bind(&Client::OnSend, this,
                            asio::placeholders::error,
                            asio::placeholders::bytes_transferred));
            }
		}
    void OnSend(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (error) {
            std::cout << "send failed: " << error.message() << std::endl;
        }
        else {
            std::cout << "send correct!" << std::endl;
        }
        std::cout << "sndque.front().size():" << sndque.front().size() << std::endl;
        std::cout << "bytes_transferred    :" << bytes_transferred << std::endl;
        sndque.pop();
		if(!sndque.empty()){
		    asio::async_write(sock, asio::buffer(sndque.front()), 
	                    boost::bind(&Client::OnSend, this,
                        asio::placeholders::error,
                        asio::placeholders::bytes_transferred));
         }
        
    }		
};
int main()
{
    asio::io_service io_service;
    Client Cl(io_service, "127.0.0.1", 31400);
	Cl.Write(std::vector<char>(256*1024*1024, 'A'));
	Cl.Write(std::vector<char>(256*1024*1024, 'B'));

    io_service.run();
}
