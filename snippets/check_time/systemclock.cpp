#include <iostream>
#include <chrono>

int main() {
    
    auto start = std::chrono::system_clock::now();

    // 重い処理
    long long j = 0;
    for(int i = 1; i <= 100; ++i)
    {
        j += i;
    }

    auto end = std::chrono::system_clock::now();

    // 経過時間をミリ秒単位で表示
    auto diff = end - start;
    std::cout << "elapsed time = "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count()
              << " nanosec."
              << std::endl;

    return 0;
}
