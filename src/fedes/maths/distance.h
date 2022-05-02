#pragma once

#include <concepts>
#include <cmath>

#include "fedes/maths/vector3.h"
#include "fedes/common/log.h"

namespace fedes {

	template <std::floating_point T = double>
	T DistanceSquared(const fedes::Vector3<T>& p, const fedes::Vector3<T>& q) {
		return pow(q.x - p.x, 2) + pow(q.y - p.y, 2) + pow(q.z - p.z, 2);
	}

	template <std::floating_point T = double>
	T DistanceSquared(const fedes::Vector3<T>& p, const T x, const T y, const T z) {
		return pow(x - p.x, 2) + pow(y - p.y, 2) + pow(z - p.z, 2);
	}

	/*
	 * @brief Returns Euclidean Distance between two given points 
	 */
	template <std::floating_point T = double>
	T Distance(const fedes::Vector3<T>& p, const fedes::Vector3<T>& q) {
		return std::sqrtf(DistanceSquared(p, q));
	}

}