#include <iostream>
#include <vector>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp> // random_generator
//------------------------------------------------------------
class IMessageFactory{
	public:
		virtual std::string GetMessage(std::string& id_) = 0;
    protected:
    	virtual std::string CreateID(){
    		const boost::uuids::uuid id = boost::uuids::random_generator()();
    		return(boost::lexical_cast<std::string>(id));
    	};
};
//------------------------------------------------------------
class FxFactory :public IMessageFactory{
	std::string	symbol;
	double	baselo;
	double	basehi;
	double	target;
	int		count;
	int		target_count;
			boost::random::mt19937 gen;
	public:
		void Init(const char* symbol_, double baselo_, double basehi_, double target_, int count_){
			symbol = std::string(symbol_);
			baselo = baselo_;
			basehi = basehi_;
			target = target_;
			target_count  = count_;
			count = 0;
		};
		double GetRandomPrice(){
			boost::random::uniform_real_distribution<double> dist(baselo, basehi);
			return(dist(gen));
		};
		std::string GetMessage(std::string& id_){
			std::vector<std::string>	v;
			id_ = CreateID();
			double		price;
			count++;
			//----------------------
			if(count == target_count){
				price = target;
			}else{
				price = GetRandomPrice();
			}
			v.push_back(id_);
			v.push_back(symbol);
			v.push_back(boost::lexical_cast<std::string>(price));
			return(boost::algorithm::join(v, ","));
		};
};
int
main(){
	FxFactory	f;
	std::string	id;

	f.Init("USDJPY", 100.10, 130.55, 90.01, 10);
	for(int i=0; i<10; i++){
		std::cout << f.GetMessage(id) << std::endl;
	}
}
		
