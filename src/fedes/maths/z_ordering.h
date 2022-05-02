#pragma once

#include <concepts>

#include "fedes/maths/vector3.h"

namespace fedes {
	
	template <std::floating_point T>
	uint_fast8_t DetermineDirection(const fedes::Vector3<T>& origin, const fedes::Vector3<T>& p);
}