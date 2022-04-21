#ifndef CryptoMarketData_H
#define CryptoMarketData_H
#include <string>
class CryptoMarketData
{
	std::string market;
	std::string price;

public:
	CryptoMarketData(std::string market_temp,std::string price_temp):market(market_temp),price(price_temp) {
	}
	void update_price(std::string newPrice);
    std::string get_price();
	std::string get_market();
};

#endif