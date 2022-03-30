// @Todo: WIP
#include "fedes/thread/thread_pool.h"

#include <thread>

namespace fedes {

	/*
	 * @brief Default ThreadPool constructor. Will create a thread pool based on the available number of hardware threads. 
	 */
	ThreadPool::ThreadPool() 
		:	thread_count_(std::thread::hardware_concurrency()) {
		threads_.reserve(thread_count_);
		for (size_t i = 0; i < thread_count_; i++) {
			// threads_.emplace_back(std::thread());
		}
	}

	/*
	 * @brief ThreadPool constructor which allows you to specify the number of threads within the pool.
	 * @param num_of_threads: Number of threads to be created/made available
	 */
	ThreadPool::ThreadPool(size_t num_of_threads) 
		: thread_count_(num_of_threads) {
		threads_.reserve(thread_count_);
		for (size_t i = 0; i < thread_count_; i++) {
			// threads_.emplace_back(std::thread());
		}
	}

	/*
	 * @brief ThreadPool destructor 
	 */
	ThreadPool::~ThreadPool() {

	}


}