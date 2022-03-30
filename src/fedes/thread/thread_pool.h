#pragma once 

#include <thread>

namespace fedes {

	class ThreadPool {
	public:
	private:
		std::vector<std::thread> threads_;
		size_t thread_count_;
	public:
		ThreadPool();
		ThreadPool(size_t num_of_threads);
		~ThreadPool();

		ThreadPool(const ThreadPool& other) = delete;
		ThreadPool& operator=(const ThreadPool& other) = delete;
		ThreadPool(ThreadPool&& other) noexcept = delete;
		ThreadPool& operator=(ThreadPool&& other) noexcept = delete;

	private:	

	};
}