#pragma once

#include "fedes/maths/vector3.hpp"
#include <concepts>

namespace fedes {

	template <typename T>
	T DistanceSquared(const fedes::Vector3<T>& p, const fedes::Vector3<T>& q);

	template <typename T>
	T Distance(const fedes::Vector3<T>& p, const fedes::Vector3<T>& q);

}