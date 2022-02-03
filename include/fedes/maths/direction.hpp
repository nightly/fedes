#pragma once

#include "fedes/maths/vector3.hpp"

namespace fedes {
	
	template <typename T>
	uint_fast8_t DetermineDirection(const fedes::Vector3<T>& origin, const fedes::Vector3<T>& p);
}