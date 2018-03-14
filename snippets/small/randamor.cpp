#include <iostream>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
int main()
{
//    boost::random::random_device seed_gen;
    boost::random::mt19937 gen(0);
  //  boost::random::uniform_int_distribution<> dist(1, 6);
    boost::random::uniform_real_distribution<double> dist(90.01, 98.55);

    for (int i = 0; i < 10; ++i) {
        //int result = dist(gen);
        double result = dist(gen);
        std::cout << result << std::endl;
    }
}
