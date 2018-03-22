#include <ctime>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>
class Consumer{
    boost::asio::io_service& io_service_;
    boost::asio::strand strand_;
    boost::lockfree::queue<std::string*> queue_;
    public:
        explicit Consumer(boost::asio::io_service& io_service, int queue_max_)
            : io_service_(io_service)
            , strand_(io_service)
            , queue_(queue_max_)
            {}
            ~Consumer(){
                std::string* buf;
                std::cout << "leave " << queue_.pop(buf) << std::endl;
            }
        void push(std::string* buf){
             if(queue_.push(buf)){
                 io_service_.post(strand_.wrap(boost::bind(&Consumer::consume, this)));	//‡”Ô‚Éˆ—‚·‚é‚Ì‚Åpost
             }else{
                abort();//ƒƒ‚ƒŠ‚ªŽæ‚ê‚È‚¢ê‡
             }
         }
    private:
       void consume()
       {
             boost::this_thread::sleep(boost::posix_time::seconds(1));
             std::string* buf;
             while(queue_.pop(buf)){
                 std::cout << *buf << std::endl;
                 delete buf;
             }
        }
};
             
class Job {
    boost::asio::io_service& io_service_;
    boost::asio::strand strand_;
    std::string name_;
    Consumer& consumer_;
public:
    explicit Job(boost::asio::io_service& io_service, const std::string& name, Consumer& consumer)
        : io_service_(io_service)
        , strand_(io_service)
        , name_(name)
        , consumer_(consumer)
         {}
    void lets_do()
    {
        io_service_.dispatch(strand_.wrap(boost::bind(&Job::doing, this)));
    }
private:
    void doing()
    {
        std::time_t stdtime = std::time(nullptr);
        consumer_.push(new std::string(name_ + ":" + std::asctime(std::localtime(&stdtime))));
    }
};

class JobManager {
    boost::asio::io_service& io_service_;
    boost::shared_ptr<boost::asio::io_service::work> work_;
    boost::thread_group group_;
public:
    JobManager(boost::asio::io_service& io_service)
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
    ~JobManager()
    {
        stop();
    }

};



int main()
{
	boost::asio::io_service io_service;
	boost::asio::io_service io_service_job;
	
	Consumer   consumer(io_service, 3);
    JobManager manager(io_service_job);
    Job jobA(io_service_job, "[Good job A]", consumer);
    Job jobB(io_service_job, "[Good job B]", consumer);
    for(int i=0; i<2 ; i++){
        jobA.lets_do();
        jobB.lets_do();
    }
    sleep(1);	//ƒXƒŒƒbƒh‚Å‚Ì“o˜^‚ð—Dæ
	std::cout << "Consume Start." << std::endl;
    for(int i=0; i<3 ; i++){
        jobA.lets_do();
        jobB.lets_do();
    }
	io_service.run();
	std::cout << "Complete." << std::endl;
	manager.stop();
}
