#pragma once

#include "fedes/maths/vector3.hpp"
#include <concepts>

namespace fedes {

	template <typename T>
	T DistanceSquared(const fedes::Vector3<T>& point, const fedes::Vector3<T>& second_point);

	template <typename T>
	T Distance(const fedes::Vector3<T>& point, const fedes::Vector3<T>& second_point);

}