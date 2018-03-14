#include <iostream>
#include <random>
int main()
{
    std::random_device rnd;     // 非決定的な乱数生成器
    for (int i = 0; i < 10; ++i) {
        //std::cout << std::hex << rnd() << "\n";
        std::cout << rnd() << "\n";
    }
    return 0;
}
