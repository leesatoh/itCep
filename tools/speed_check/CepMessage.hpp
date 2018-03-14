#pragma once
#include "InterFaces.h"
#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <string.h>
using namespace std::chrono;
//------------------------------------
inline int get_time_char(char* s, size_t max, timespec* ts) {
	char buf[32];
	struct tm t;
//	localtime_s(&t, &(ts->tv_sec));
	localtime_r(&(ts->tv_sec), &t);
	strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", &t);
	return(sprintf(s, "%s.%09ld", buf, ts->tv_nsec));
}
//------------------------------------
inline void get_time_spec(timespec* ts, const char* time_str_) {
	struct	TIME_STR {
		char	year[4 + 1];
		char	mont[2 + 1];
		char	mday[2 + 1];
		char	hour[2 + 1];
		char	minu[2 + 1];
		char	secd[2 + 1];
		char	usec[9 + 1];
	};
	TIME_STR		time_str;
	tm				ltm;


	memcpy(&time_str, time_str_, sizeof(time_str));
	time_str.year[sizeof(time_str.year) - 1] = 0;
	time_str.mont[sizeof(time_str.mont) - 1] = 0;
	time_str.mday[sizeof(time_str.mday) - 1] = 0;
	time_str.hour[sizeof(time_str.hour) - 1] = 0;
	time_str.minu[sizeof(time_str.minu) - 1] = 0;
	time_str.secd[sizeof(time_str.secd) - 1] = 0;
	time_str.usec[sizeof(time_str.usec) - 1] = 0;
	//------- 時刻を設定
	memset(&ltm, 0, sizeof(ltm));
	ltm.tm_year = atoi(time_str.year)-1900;
	ltm.tm_mon = atoi(time_str.mont)-1;
	ltm.tm_mday = atoi(time_str.mday);
	ltm.tm_hour = atoi(time_str.hour);
	ltm.tm_min = atoi(time_str.minu);
	ltm.tm_sec = atoi(time_str.secd);
	ltm.tm_isdst = -1;	//夏時間は無視
						//------- i64日時に変換
	ts->tv_sec = mktime(&ltm);
	ts->tv_nsec = atol(time_str.usec);

}
class cCepMessage :public IMessage {
public:
	int get_message(char* buf, size_t len) {
		struct timespec ts;
		timespec_get(&ts, TIME_UTC);
		return(get_time_char(buf, len, &ts));
	}
	void show_elapsed_time(const char* buffer_data) {
		struct timespec ts_den;
		struct timespec ts_now;
		timespec_get(&ts_now, TIME_UTC);
		get_time_spec(&ts_den, buffer_data);
		std::cout << "diff"
			" tv_sec:" << std::to_string(ts_now.tv_sec - ts_den.tv_sec) <<
			" tv_nsec:" << std::to_string(ts_now.tv_nsec - ts_den.tv_nsec) <<
			 std::endl;
	}
};
