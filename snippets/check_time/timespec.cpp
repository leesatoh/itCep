#include <iostream>
#include <stdio.h>
using namespace std;

//!時間取得
double calcTime()
{
    struct::timespec getTime;
    clock_gettime(CLOCK_MONOTONIC, &getTime);
    return (getTime.tv_sec + getTime.tv_nsec*1e-9) *1000;
}

int main()
{

    double start = calcTime();
	for(int i=0; i<100; i++){
	}
 //   int cnt = 0;
//    while(cnt < 100)
//    {
//        cnt++;
//    }
    double end = calcTime();

    printf("処理時間[ms]：%lf\n", end -start);

    return 0;
}
