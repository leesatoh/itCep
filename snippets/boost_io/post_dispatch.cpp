#include <iostream>
#include <boost/asio.hpp>

int main() {
	boost::asio::io_service io;	//(1)

	io.post( [](){ std::cout << "post" << std::endl; } );		//(2)
	io.dispatch( []{ std::cout << "dispatch" << std::endl; } );	//(3)

	io.run();	//(4)
}
