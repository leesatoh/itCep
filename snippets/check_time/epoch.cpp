#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std::chrono;

int main()
{
  system_clock::time_point p = system_clock::now(); // 現在日時を取得

  auto s = duration_cast<nanoseconds>(p.time_since_epoch()); // エポックからの経過時間(秒)を取得
////  std::cout << s.count() << std::endl;
// std::string str = std::to_string(s.count());
// std::cout << str << std::endl;
  std::ostringstream sout;
  sout << std::setfill('0') << std::setw(24) << s.count();
  std::cout << sout.str() <<std::endl;
}
