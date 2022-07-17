#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <thread>
#include <ctime>

//request handling algorithm

class requestHandler {
	uint_fast16_t requestCount;
	time_t start_time;
	inline constexpr bool aboveLimit() noexcept {
		return (((int)((time_t)time(NULL) - start_time)) >= 60) ? true : false;
	}
	inline const constexpr bool aboveRequestCount() noexcept{
		return (requestCount >= 1000) ? true : false;
	}
	inline void reset_restart() noexcept {
		requestCount = 0;
		start_time = std::move((time_t)time(NULL));
	}
	inline const void wait_reset_restart() noexcept {
		if (((uint_fast16_t)time(NULL) - start_time) < 60)
		  std::this_thread::sleep_for(std::chrono::seconds(60 - ((uint_fast16_t)time(NULL) - start_time)));
		reset_restart();
	}
public:
	requestHandler() = default;
	inline void start_() noexcept {
		requestCount = 0;
		start_time = (time_t)time(NULL);
	}
	inline void mayContinue() noexcept {
		if (aboveLimit() && aboveRequestCount())
			reset_restart();
		else
		if (aboveLimit())
		    reset_restart();
		else 
	    if (aboveRequestCount())
			wait_reset_restart();
		return;
	}
	inline void operator++() {
		++requestCount;
	}
};
#endif //!REQUESTHANDLER_H
