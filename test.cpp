#include <random>
#include <iostream>
#include <chrono>
using namespace std;
int main() {
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine engine(seed);
    uniform_int_distribution<> coin(0,1);
    int coinFlip = coin(engine);
    //for(int i = 0; i<10; i++){
		cout << coinFlip << "\n";
	//}
	
	return 0;
}
