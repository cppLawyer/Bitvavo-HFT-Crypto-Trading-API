#ifndef CryptoAssetsManager_H
#define CryptoAssetsManager_H
#include <curl/curl.h>
#include <iostream>
#include "CryptoMarketData.h"
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>
namespace bitvavo_api {
	namespace process_bitvavo_api {
		namespace bitvavoRateLimitManager{
		 //rate limit is <= 1000 GET request a minute
			class time_and_stop_manager {
				volatile uint_fast16_t requestCount;
				time_t start_time;
				inline bool aboveLimit() {
					return (((int)((time_t)time(NULL) - this->start_time)) >= 60) ? true : false;
				}
				inline bool aboveRequestCount() {
					return (requestCount >= 1000) ? true : false;
				}
				inline void reset_restart() {
					this->requestCount = 0;
					this->start_time = (time_t)time(NULL);
				}
				inline void wait_reset_restart() {
					if (((uint_fast16_t)time(NULL) - this->start_time) < 60) {
						std::this_thread::sleep_for(std::chrono::seconds(60 - ((uint_fast16_t)time(NULL) - this->start_time)));
					}
					reset_restart();
				}
			 public:
				 inline void start_() {
					 this->requestCount = 0;
					 this->start_time = (time_t)time(NULL);
				 }
				inline void mayContinue() {
					if (aboveLimit() && aboveRequestCount()) {
						reset_restart();
					}
					else {
						if (aboveLimit()) {
							reset_restart();	
						}
						else {
							if (aboveRequestCount()) {
								wait_reset_restart();
								
							}
						}
					}

					return;
				}

				inline void operator++() {
					++requestCount;
				}
		    };
		}
		bitvavoRateLimitManager::time_and_stop_manager tManager;
		static std::atomic<bool> keep_updating;
		static std::thread globalTH;
		std::string server_response;
		std::vector<CryptoMarketData> vec_of_crypto_data;

		inline static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
		{
			size_t realsize = size * nmemb;
			server_response.append((const char*)contents, realsize);
			return realsize;
		}

		inline bool request_data_from_server() {
			CURL* curl;
			CURLcode res;

			curl_global_init(CURL_GLOBAL_DEFAULT);

			curl = curl_easy_init();
			if (curl) {
				curl_easy_setopt(curl, CURLOPT_URL, "https://api.bitvavo.com/v2/ticker/price");


				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);


				server_response.clear();
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
				res = curl_easy_perform(curl);
				curl_easy_cleanup(curl);
			}
			curl_global_cleanup();
			++tManager;
			return (res == CURLE_OK);
		}

		inline void give_meaning_to_data() {
			bool ok = request_data_from_server();
			if (ok == false) {
				std::cerr << "Problem Requesting Data From Server\n"; 
				exit(-1);
			}

			server_response.erase(server_response.begin() + 1);//erasing first character
			std::string::iterator sr_it = server_response.begin();
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
				vec_of_crypto_data.push_back(std::move(CryptoMarketData(marketTMP, priceTMP)));
			} while (sr_it != server_response.end());

			return;
		}

		inline void set_crypto_data(int status = 0) {
			give_meaning_to_data();
			system("cls");
			for (int i = 0; i < vec_of_crypto_data.size(); ++i) {
				std::cout << "Market: " << vec_of_crypto_data[i].get_market() <<
					"   Price: " << vec_of_crypto_data[i].get_price() << "\n";
			}

		}//function that gets price info and name one time use

		inline void update_crypto_price() {

			keep_updating.store(true, std::memory_order_relaxed);
			while (keep_updating.load(std::memory_order_relaxed)) {
				vec_of_crypto_data.clear();
				tManager.mayContinue();
				set_crypto_data();
			}


		}//thread that updates price

		inline void process_manager() {
			tManager.start_();
			set_crypto_data();
			std::thread data_update_thread(update_crypto_price);
			globalTH = std::move(data_update_thread);
			return;
		}






	}
	void start_operation() {
		process_bitvavo_api::process_manager();
	}
	void stop_operation() {
		process_bitvavo_api::keep_updating.store(false, std::memory_order_relaxed);
		process_bitvavo_api::globalTH.join();
	}


}

#endif