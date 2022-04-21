#include "CryptoMarketData.h"


void CryptoMarketData::update_price(std::string newPrice) {
	this->price = newPrice;
}

std::string CryptoMarketData::get_price() {
	return this->price;
}

std::string CryptoMarketData::get_market() {
	return this->market;
}
