#include <iostream>
#include <boost/asio.hpp>

class hoge {
public:
	void operator()() const
	{
		std::cout << "hoge :: operator()!" << std::endl;
	}
};

inline void* asio_handler_allocate( std::size_t size, hoge* )
{
	std::cout << "custom allocator!" << std::endl;
	return ::operator new( size );
}

void asio_handler_deallocate( void* pointer, std::size_t size, hoge* )
{
	::operator delete( pointer );
	std::cout << "custom deallocator!" << std::endl;
}

template<class F>
void asio_handler_invoke( F f, hoge* )
{
	std::cout << "custom invocation!" << std::endl;
	f();
}

int main() {
	boost::asio::io_service io;
	const hoge h;

	io.post( h );
	io.run();
}
