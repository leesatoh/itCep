#include <iostream>
#include <vector>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/join.hpp>

int main()
{
	std::vector<std::string>	v;
	std::string id = "UniqIDxx";
	const std::string  symbol = std::string("Symbol");
	double	target = 100.110;
//	std::string  targetstr = boost::lexical_cast<std::string>(target);
	v.push_back(symbol);
//	v.push_back(targetstr);
	v.push_back(boost::lexical_cast<std::string>(target));
	v.push_back(id);
	const std::string s = boost::algorithm::join(v, ",");
    std::cout << s << std::endl;
}
