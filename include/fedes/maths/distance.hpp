#pragma once

#include "fedes/maths/vector3.hpp"
#include <concepts>

namespace fedes {

	template <std::integral T>
	T Distance(fedes::Vector3<T> point, fedes::Vector3<T> second_point);

}