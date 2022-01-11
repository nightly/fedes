#include "fedes/instrumentation/timer.hpp"

#include <chrono>
#include <string>
#include <iostream>

namespace fedes::internal {
	
	/*
	 * @brief Constructs a timer with the given name
	 * 
	 * @param name: Name of the timer which will be used to identify it within output
	 * @param output_stream: File, std::cout, where writes will be sent to & flushed. Default parameter: std::cout.
	 */
	Timer::Timer(const std::string& name, std::ostream& output_stream) 
	: name_(name), output_stream_(output_stream), stopped_(false) {
		start_ = std::chrono::high_resolution_clock::now();
	}

	/*
	 * @brief Stops a timer which can then be restarted. Doesn't default to writing anything.
	 * 
	 * @return Returns the duration of timer until now
	 */
	long long Timer::Stop() {
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
		return end - start;
	}

	/*
	 * @brief Stops the timer and outputs the duration to the pre-set output_stream.
	 */
	void Timer::StopWithWrite() {
		auto duration = Stop();
		output_stream_ << "Timer for " << name_ << " took " << duration << " µs (microseconds) or " << (duration * 0.001) << " ms (milliseconds) \n";
		output_stream_.flush();
	}

	/*
	 * @brief Restarts the timer by changing the start time to now. End time isn't tracked within the timer itself.
	 */
	void Timer::Restart() {
		stopped_ = false;
		start_ = std::chrono::high_resolution_clock::now();
	}

	/*
	* @brief Destructor for Timer which by default will also output a write if not previously stopped
	*/
	Timer::~Timer() {
		if (!stopped_) {
			StopWithWrite();
		}
		else {
			Stop();
		}
	}

}
