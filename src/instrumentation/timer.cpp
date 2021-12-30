#include "fedes/instrumentation/timer.hpp"

#include <chrono>
#include <string>
#include <iostream>

namespace fedes::internal {

		Timer::Timer(const std::string& name, std::ostream& output_stream) 
		: name_(name), output_stream_(output_stream) {
			start_ = std::chrono::high_resolution_clock::now();
		}

		long long Timer::Stop() {
			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
			return end - start;
		}

		void Timer::StopWithWrite() {
			auto duration = Stop();
			output_stream_ << "Timer for " << name_ << " took " << duration << " µs (microseconds) or " << (duration * 0.001) << " ms (milliseconds) \n";
			output_stream_.flush();
		}

		void Timer::Restart() {
			start_ = std::chrono::high_resolution_clock::now();
		}

		Timer::~Timer() {
			StopWithWrite();
		}

}
