#include "fedes/interop/dynamic_array.h"

#include <vector>

namespace fedes {

	template<typename T>
	std::vector<T> Dyn1DArrayAsVec(T* buffer, size_t count) {
		std::vector<T> vec;
		vec.reserve(count);
		size_t offset = sizeof(T);
		for (size_t current = 0; current < count; current++) {
			T element = *(buffer + (current * offset));
			vec.push_back(element);
		}
		return vec;
	}

}