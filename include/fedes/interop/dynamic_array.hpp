#pragma once 

#include <vector>

namespace fedes {
	template<typename T>
	std::vector<T> DynArray1DAsVec(T* buffer, size_t count);
}

#include "../src/interop/dynamic_array.cpp"