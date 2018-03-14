#include "ThreadPool.hpp"
#include <iostream>
#include <chrono>

class WorkA : public Runnable
{
public:
    void run() final {
        std::cout << "WorkA" << std::endl;
    }
};

class WorkB : public Runnable
{
public:
    WorkB(int value)
        : value_(value)
    {}
    void run() final {
        std::cout << "WorkB value=" << value_ << std::endl;
    }
private:
    int value_;
};

int main()
{
    ThreadPool tp(5);

    for (int n = 0; n < 1000; n++) {
        while (!tp.add(std::make_shared<WorkA>())) {
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        while (!tp.add(std::make_shared<WorkB>(n))) {
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    std::cout << "Finish!!-------------------------- " << std::endl;
    return 0;
}
