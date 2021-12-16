#pragma once 

#include <vector>
namespace fedes {
	template<typename T>
	std::vector<T> Dyn1DArrayAsVec(T* buffer, size_t count);
}