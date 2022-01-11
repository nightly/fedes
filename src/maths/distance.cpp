#include "fedes/maths/distance.hpp"

#include "fedes/maths/vector3.hpp"

#include <cmath>

namespace fedes {

	/*
	* @brief Euclidean distance
	*
	* @tparam T: numeric type
	* @param point: 3 dimensional point
	* @param second_point: the second 3 dimensional point to compute distance against
	*/
	template <std::integral T>
	T Distance(fedes::Vector3<T> point, fedes::Vector3<T> second_point) {
		return sqrt(pow(second_point.x - point.x, 2) + pow(second_point.y - point.y, 2) + pow(second_point.z - point.z, 2));
	}
}