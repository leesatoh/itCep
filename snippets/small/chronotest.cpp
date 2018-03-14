#include <iostream>
#include <unistd.h>
#include <chrono>

int main(){
 std::chrono::system_clock::time_point  start, end; // 型は auto で可
 start = std::chrono::system_clock::now(); // 計測開始時間
// 処理
	usleep(1000000);
 end = std::chrono::system_clock::now();  // 計測終了時間
 double elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count(); //処理に要した時間をミリ秒に変換
 auto ctm = std::chrono::system_clock::to_time_t(start);
std::cout << "elapsed:" << elapsed << " start:" << std::ctime(&(ctm)) << std::endl;
}
