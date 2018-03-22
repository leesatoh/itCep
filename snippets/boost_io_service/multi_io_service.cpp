#include <thread>
#include <iostream>

#include <boost/asio/io_service.hpp>

namespace {
    // とりあえずの処理1 改(1秒停止)
    void func1() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout<<"func1"<<std::endl;
    }
    // とりあえずの処理2 改(3秒停止)
    void func2() {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout<<"func2"<<std::endl;
    }
}

int main() {
    boost::asio::io_service ios1;
    boost::asio::io_service ios2;

    // io_serviceの中にあるキューに追加する
    for(std::size_t i=0; i<2; ++i) {
        ios1.post(func1);
        ios1.post(func2);
        ios2.post(func1);
        ios2.post(func2);
    }

    // キューがなくなるまで実行を続ける
    //  スレッド
    std::thread t1([&ios1]{ios1.run();});
    std::thread t2([&ios2]{ios2.run();});

    t1.join();
    t2.join();
}
