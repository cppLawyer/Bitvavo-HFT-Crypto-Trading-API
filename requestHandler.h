#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H
///////////////////////
#include <atomic>
#include <thread>
#include <ctime>
//////////////////////

///request handling algorithm///

class requestHandler {
	std::atomic<uint_fast16_t> requestCount;
	time_t start_time;
	inline constexpr bool aboveLimit() noexcept {
		return (((int)((time_t)time(NULL) - start_time)) >= 60) ? true : false;
	}
	inline const constexpr bool aboveRequestCount() {
		return (requestCount.load(std::memory_order_relaxed) >= 1000) ? true : false;
	}
	inline void reset_restart() noexcept {
		requestCount.store(0, std::memory_order_relaxed);
		start_time = std::move((time_t)time(NULL));
	}
	inline const void wait_reset_restart() noexcept {
		if (((uint_fast16_t)time(NULL) - start_time) < 60) {
			std::this_thread::sleep_for(std::chrono::seconds(60 - ((uint_fast16_t)time(NULL) - start_time)));
		}
		reset_restart();
	}
public:
	requestHandler() = default;
	inline void start_() noexcept {
		requestCount.store(0, std::memory_order_relaxed);
		start_time = (time_t)time(NULL);
	}
	 inline void mayContinue() noexcept {
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
		this->requestCount.fetch_add(1,std::memory_order_relaxed);
	}
};

#endif //!REQUESTHANDLER_H
