#include <chrono>
#include <iostream>

namespace fedes::internal {

	// RAII Timer
	class Timer {
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start_;
		std::string name_;
	public:
		Timer(std::string& name) 
		: name_(name) {
			start_ = std::chrono::high_resolution_clock::now();
		}

		void Restart() {
			start_ = std::chrono::high_resolution_clock::now();
		}

		auto Stop() {
			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();

			auto duration = end - start;
			return duration;
		}

		void StopWithConsolePrint() {
			auto duration = Stop();
			std::cout << "Timer for " << name_ << " took " << duration << " μs (microseconds)\n";
		}


		~Timer() {
			StopWithConsolePrint();
		}
	};
}