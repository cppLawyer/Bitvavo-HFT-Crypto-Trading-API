#ifndef CryptoMarketData_H
#define CryptoMarketData_H
////////////////////
#include <string>
///////////////////

///Basic Data organize class///

class CryptoMarketData
{
	std::string market;
	std::string price;

public:
	inline CryptoMarketData(std::string market_temp,std::string price_temp):market(market_temp),price(price_temp) {
	}
	inline CryptoMarketData() noexcept = default;
	inline CryptoMarketData(CryptoMarketData&& temp) noexcept {
		this->market = temp.market;
		this->price = temp.price;
	}//if you use a vector

	inline constexpr void update_price(const std::string&& newPrice) noexcept {
		this->price = newPrice;
	}

	inline constexpr std::string get_price() noexcept {
		return this->price;
	}

	inline constexpr std::string get_market() noexcept {
		return this->market;
	}
	inline constexpr CryptoMarketData& set_price(std::string tmpPrice) noexcept {
		this->price = std::move(tmpPrice);
		return *this;
	}
	inline constexpr CryptoMarketData& set_market(std::string tmpMarket) noexcept {
		this->market = std::move(tmpMarket);
		return *this;
	}
	
};

#endif