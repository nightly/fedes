#pragma once 

#include <vector>

namespace fedes {
	template<typename T>
	std::vector<T> DynArray1DAsVec(T* buffer, size_t count);
}

// Remove the need for verbose explicit template instanatiation by including the templated definitions
#include "../src/interop/dynamic_array.cpp"