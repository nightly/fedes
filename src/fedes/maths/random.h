#pragma once

#include <concepts>
#include <random>
#include <vector>

#include "fedes/maths/vector3.h"

namespace fedes {

	template <std::floating_point T>
	std::vector<fedes::Vector3<T>> GenerateRandomArray(double lower_bound, double upper_bound, size_t num_of_points) {
		std::vector<fedes::Vector3<T>> points;
		points.reserve(num_of_points);
		std::uniform_real_distribution<T> uniform(lower_bound, upper_bound);
		std::default_random_engine engine;
		for (size_t i = 0; i < num_of_points; ++i) {
			points.emplace_back(fedes::Vector3(uniform(engine), uniform(engine), uniform(engine)));
		}
		return points;
	}
}