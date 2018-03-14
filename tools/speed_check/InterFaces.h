#pragma once
#include <string>
class IRunnable
{
public:
	virtual void run() = 0;
};
class IMessage
{
public:
	virtual int get_message(char* buf, size_t len) = 0;
	virtual void show_elapsed_time(const char* buffer_data) = 0;
};
