#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
class Manager {
    boost::asio::io_service& io_service_;
    boost::shared_ptr<boost::asio::io_service::work> work_;
    boost::thread_group group_;
public:
    Manager(boost::asio::io_service& io_service)
        : io_service_(io_service)
    {
        work_.reset(new boost::asio::io_service::work(io_service_));
        for (std::size_t i = 0; i < 3; ++i) {
            group_.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
        }
    }
    void stop()
    {
        work_.reset();
        group_.join_all();
    }
    ~Manager()
    {
        stop();
    }

};


class Worker {
    boost::asio::io_service& io_service_;
    boost::asio::strand strand_;
    std::string name_;
public:
    explicit Worker(boost::asio::io_service& io_service, const std::string& name)
        : io_service_(io_service), strand_(io_service), name_(name) {}
    void lets_do(std::shared_ptr<const std::string> msg)
    {
        io_service_.dispatch(strand_.wrap(boost::bind(&Worker::doing, this, msg)));
    }
private:
    void doing(std::shared_ptr<const std::string> msg)
    {
    	std::cout <<  "start : " + name_ + " -------" << *msg << std::endl;
        boost::this_thread::sleep(boost::posix_time::seconds(2));
    	std::cout <<  "end   : " + name_ + " =======" << *msg << std::endl;
    }
};

int main()
{
	boost::asio::io_service io_service;
    Manager manager(io_service);

    Worker workerA(io_service, "[Working A Job]");
    Worker workerB(io_service, "[Working B Job]");

	auto pt1 = std::make_shared<std::string>("pt1");

	std::cout << "Would you do work?" << std::endl;
    workerA.lets_do(pt1);
    workerA.lets_do(pt1);
    workerB.lets_do(pt1);
    workerB.lets_do(pt1);
    workerA.lets_do(pt1);
	std::cout << "Sounds good ! pt1 working on thread." << std::endl;

	for(int i=0; i<10 ; i++){
		auto pt2 = std::make_shared<std::string>("pt:" + std::to_string(i));
	    workerA.lets_do(pt2);
   	 	workerB.lets_do(pt2);
	}
	std::cout << "Go it. All work doing." << std::endl;
	manager.stop();
}
