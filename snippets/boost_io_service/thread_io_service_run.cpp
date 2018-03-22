// clang++ -std=c++11 -lboost_system -lpthread test.cpp

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
    boost::asio::io_service ios;

    // io_serviceの中にあるキューに追加する
    for(std::size_t i=0; i<2; ++i) {
        ios.post(func1);
        ios.post(func2);
    }

    // キューがなくなるまで実行を続ける
    //  スレッド
    std::thread t([&ios]{ios.run();});
    //  メインスレッド
//    ios.run();

    t.join();
}
