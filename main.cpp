#include <iostream>
#include "CryptoAssetsManager.h"



int main() {

	bitvavo_api::start_operation();

	
	std::cin.get();

	bitvavo_api::stop_operation();

	return 0;

	
}