
/*
#include <boost/signals2.hpp>
#include <thread>
#include <mutex>
#include <iostream>

boost::signals2::signal<void(int)> s;
std::mutex m;

void loop()
{
  for (int i = 0; i < 100; ++i)
    s(i);
}

int main()
{
  s.connect([](int i){
    std::lock_guard<std::mutex> lock{m};
    std::cout << i << '\n';
  });
  std::thread t1{loop};
  std::thread t2{loop};
  t1.join();
  t2.join();
}
*/
#include <iostream>
#include <boost/system/error_code.hpp>
#include <boost/signals2.hpp>
#include <boost/asio.hpp>

class demo_model
{
public:
    demo_model(boost::asio::io_service& io_service)
    :   io_service_(io_service)
    ,   timer_(io_service)
    {}

    boost::signals2::signal<void (const char*)> signal;

    void do_something()
    {
        timer_.expires_from_now(boost::posix_time::seconds(5));
        timer_.async_wait([this](boost::system::error_code const& ec)
        {
            if (ec) return;
            this->signal("aaaa");
            this->signal("b bb");
        });
    }

private:
    boost::asio::io_service& io_service_;
    boost::asio::deadline_timer timer_;
};

int main()
{
    using namespace std;

    boost::asio::io_service io_service;
    demo_model m(io_service);

    m.signal.connect([](const char* p){ sleep(10); cout << "Signalled! (1)" << p << endl; });
    m.signal.connect([](const char* p){ sleep(10); cout << "Signalled! (2)" << p << endl; });

    m.do_something();

    io_service.run();
	cout << "end" << endl;

    return 0;
}

//“¯ŠúŒ^‚â‚ñBBŽg‚¢‚à‚ñ‚É‚È‚ç‚È‚¢