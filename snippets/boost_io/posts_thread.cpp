#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

//数えるだけ
class counter
{
public:
	counter()
		: i_( 0 )
	{}

	void operator()()
	{
		std::cout << "count: " << i_ << std::endl;
		++i_;
	}

private:
	int i_;
};

int main() {
	std::cout << "main thread : " << boost::this_thread::get_id() << std::endl;

	//
	boost::asio::io_service io;

	//postで文字を出力させる(1)
	for( int i=0; i<5; ++i )
		io.post( []{ std::cout << boost::this_thread::get_id() << " : post." << std::endl; } );

	//スレッドを4つ作り、runを走らせる(2)
	boost::thread_group tg;
	for( int i=0; i<4; ++i )
		tg.create_thread( boost::bind( &boost::asio::io_service::run, &io ) );

	//1秒待つ(3)
	boost::this_thread::sleep( boost::posix_time::seconds( 1 ) );

	//数えてみたり(4)
	counter c;
	for( int i=0; i<5; ++i )
		io.post( std::ref( c ) );

	//文字を出力してみたり(5)
	for( int i=0; i<5; ++i )
		io.post(
			[&]{ io.dispatch(
				[]{ std::cout << boost::this_thread::get_id() << " : dispatch." << std::endl; }
				);
			} );

	//終わるのを待つ(6)
	tg.join_all();

	std::cout << "finished!" << std::endl;
}
