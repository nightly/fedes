#pragma once

#include <vector>

namespace fedes {

	template <typename T, size_t chunk_size>
	class ChunkAllocator {
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = size_t;
	private:
		T* current_ptr;
	public:
		ChunkAllocator();
		
		template<class U>
		ChunkAllocator(ChunkAllocator<U, chunk_size> const&) noexcept;
		
		~ChunkAllocator();

		T* allocate(size_type n);
		void deallocate(T* pointer, size_type n);
	private:

	};

	template <class T, size_t chunk_size>
	bool operator==(ChunkAllocator<T, chunk_size> const&, ChunkAllocator<T, chunk_size> const&);

	template <class T, size_t chunk_size>
	bool operator!=(ChunkAllocator<T, chunk_size> const&, ChunkAllocator<T, chunk_size> const&);

}