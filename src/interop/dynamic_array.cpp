#include <vector>
#include <concepts>

namespace fedes {

	template<typename T>
	std::vector<T> Dyn1DArrayAsVec(T* buffer, size_t count) {
		std::vector<T> vec;
		vec.reserve(count);
		for (size_t i = 0; i < count; i++) {
			T element = *(buffer + i);
			vec.push_back(element);
		}
		return vec;
	}

}