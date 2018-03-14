#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp> // random_generator

using namespace boost::uuids;

int main()
{
    // ランダムにユニークIDを生成
    const uuid id1 = random_generator()();
    const uuid id2 = random_generator()();

    std::cout << id1 << std::endl;
    std::cout << id2 << std::endl;
}
