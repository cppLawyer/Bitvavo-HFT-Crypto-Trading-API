#ifndef CryptoMarketData_H
#define CryptoMarketData_H

#include <string>
#include "bitvavo.h"

class CryptoMarketData
{
	std::string price;
	std::string market;
public:
	inline CryptoMarketData(std::string market_temp,std::string price_temp):market(market_temp),price(price_temp) {
	}
	inline CryptoMarketData() noexcept = default;
	inline CryptoMarketData(CryptoMarketData&& temp) noexcept {
		this->market = temp.market;
		this->price = temp.price;
	}//if you use a vector, whats out for std::move in this case

	inline constexpr void update_price(const std::string&& newPrice) noexcept {
		this->price = std::move(newPrice);
	}
	inline constexpr std::string get_market() noexcept{
		return this->market;
	}
	inline constexpr std::string get_price() noexcept {
		return this->price;
	}
	inline constexpr void set_price(std::string tmpPrice) noexcept {
		this->price = std::move(tmpPrice);
	}
	inline constexpr CryptoMarketData& set_market(std::string tmpMarket) noexcept {
		this->market = std::move(tmpMarket);
		return *this;
	}	
};
#endif
