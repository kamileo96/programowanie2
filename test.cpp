#include <random>
#include <iostream>
#include <ctime>
using namespace std;
int main() {
	default_random_engine engine{static_cast<long unsigned int>(time(0))};
    uniform_int_distribution<> distr(0,1);  
	for(int i=0; i<10; ++i)
    	std::cout<<  distr(engine) << " ";
	return 0;
}
