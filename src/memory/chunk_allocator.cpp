// @Todo: WIP

#include "fedes/memory/chunk_allocator.hpp"

namespace fedes {

	/*
	 * @brief A non thread-safe chunk-based custom memory allocator compliant specialization of std::allocator_traits  
	 * @tparam T: object in question to allocate chunk-wise
	 * @tparam chunk_size: size of each chunk 
	 */
	template <typename T, size_t chunk_size>
	ChunkAllocator<T, chunk_size>::ChunkAllocator()  {
	}

	/*
	 * @brief Conforms to allocate() of std::allocator_traits
	 */
	template <typename T, size_t chunk_size>
	T* ChunkAllocator<T, chunk_size>::allocate(size_type n) {
		// 1. If the current chunk isn't full, we can use placement new to allocate at the endpoint
		// 2. If full, assign a new chunk using our chunk_size tparam and allocate inside that new chunk
		// 3. Update chunk tracking
	}

	/*
	 * @brief Conforms to deallocate() of std::allocator_traits 
	 */
	template <typename T, size_t chunk_size>
	void ChunkAllocator<T, chunk_size>::deallocate(T* pointer, size_type n) {
		::operator delete(pointer);
	}


}