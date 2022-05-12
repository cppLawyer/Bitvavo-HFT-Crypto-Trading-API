#ifndef BITVAVO_H
#define BITVAVO_H

///////////////////////////////
#include "CryptoMarketData.h"
#include "requestHandler.h"
#include "curl/curl.h";
#include <atomic>
#include <thread>
#include <iostream>
#include <array> //array because of speed do not use vector//
/// //////////////////////////

///MAIN HEADERFILE///


namespace server {
	static std::string serverData;
	inline static constexpr size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) noexcept
	{
		size_t realsize = size * nmemb;
		serverData.append((const char*)contents, realsize);
		return realsize;
	}
	const constexpr uint_fast8_t MarketDataSize = 241;//change this value to the Cryptocurrency count of bitvavo// 
	//currently 241 is the most recent value
};

class bitvavo {
	std::thread priceThread;
	std::atomic<bool> updatePrice;
	CryptoMarketData* crypto_price_data = new CryptoMarketData[server::MarketDataSize];
	requestHandler reqHandle;

	inline void request_data_from_server(const std::string&& api_link) noexcept {
			CURL* curl;
			CURLcode res;

			curl_global_init(CURL_GLOBAL_DEFAULT);
			curl = std::move(curl_easy_init());
			curl_easy_setopt(curl, CURLOPT_URL, api_link.c_str());
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //supports HTTPS requests
			server::serverData.clear();
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, server::WriteCallback);
			res = std::move(curl_easy_perform(curl));
			curl_easy_cleanup(curl);
			curl_global_cleanup();
			{
				if (res != CURLE_OK) {
					std::cerr << "Problem Requesting Data From Server\n";
					exit(-1);
				}
			}//optional if you are sure it works


			++reqHandle;//inc request count after operation
			return;
		}//can be reused for other functions from the api// use move r-value for performance benefit

	inline void printCrypto_data() noexcept{
		     
		{
			system("cls");// Windows XP >
			system("clear"); //Linux and MacOS 
			for (uint_fast8_t i = 0; i < server::MarketDataSize; ++i) {
				std::cout << "Market: " << crypto_price_data[i].get_market() <<
					"   Price: " << crypto_price_data[i].get_price() <<"\n";
			}

		}//optional but just for testing

	}

	inline void fix_JSON() noexcept {
		
		request_data_from_server(std::move("https://api.bitvavo.com/v2/ticker/price"));
		server::serverData.erase(server::serverData.begin() + 1);//erasing first character
		std::string::iterator sr_it = server::serverData.begin();
		uint_fast8_t index = 0;
		do {
			sr_it += 12; //skip
			std::string marketTMP;
			while (*sr_it != '"') {
				marketTMP += *sr_it;
				++sr_it;
			}
			sr_it += 12; //skip
			std::string priceTMP;
			while (*sr_it != '"') {
				priceTMP += *sr_it;
				++sr_it;
			}
			sr_it += 3; //skip
			crypto_price_data[index++].set_market(marketTMP).set_price(priceTMP);
		} while (sr_it != server::serverData.end());

		printCrypto_data(); //optional just as an example

		return;
	}//gives meaning to Json data
public:


	inline bitvavo(void) noexcept = default;
	bitvavo(bitvavo&&) = delete;
	bitvavo(bitvavo&) = delete;
	inline void start() noexcept{
		reqHandle.start_();
		fix_JSON();
		++reqHandle;
		this->priceThread = std::move(std::thread(std::move([&]() {
			updatePrice.store(true, std::memory_order_relaxed);
			while (updatePrice.load(std::memory_order_acquire)) {
				reqHandle.mayContinue();
				 fix_JSON();
			}   }

		)));
	}
	inline void stop() noexcept{
		updatePrice.store(false, std::memory_order_release);//release to ensure sync, relaxed would be faster
		priceThread.join();
	}
	inline ~bitvavo() noexcept{
		if (updatePrice.load(std::memory_order_relaxed) == true) {
			updatePrice.store(false, std::memory_order_release); //release to ensure sync, relaxed would be faster
			if (priceThread.joinable())
				  priceThread.join();

			
		}

		delete[] crypto_price_data;
	}
};









#endif // BITVAVO_H

