#include <iostream>
#include <boost/format.hpp>

int main()
{
	std::cout << boost::format("%s\n") % "hello, world";
	return 0;
}
