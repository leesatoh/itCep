// clang++ -std=c++11 -lboost_system test1.cpp

#include <iostream>

#include <boost/asio/io_service.hpp>

namespace {
    // とりあえずの処理1
    void func1() {
        std::cout<<"func1"<<std::endl;
    }
    // とりあえずの処理2
    void func2() {
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
    ios.run();
}

