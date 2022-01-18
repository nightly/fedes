#include "fedes/maths/distance.hpp"

#include "fedes/maths/vector3.hpp"

#include <cmath>
#include <concepts>

namespace fedes {

	/*
	* @brief Euclidean Distance Squared which omits the square root computation when the actual distance value isn't required
	* 
	* @tparam T: numeric type, i.e. double, float
	* @param point: 3 dimensional point
	* @param second_point: the second 3 dimensional point to compute distance against
	* 
	* @returns Squared Distance between two points
	*/
	template <typename T>
	T DistanceSquared(const fedes::Vector3<T>& point,  const fedes::Vector3<T>& second_point) {
		return pow(second_point.x - point.x, 2) + pow(second_point.y - point.y, 2) + pow(second_point.z - point.z, 2);
	}

	/*
	* @brief Euclidean Distance
	*
	* @tparam T: numeric type,  i.e. double, float
	* @param point: 3 dimensional point
	* @param second_point: the second 3 dimensional point to compute distance against
	* 
	* @returns Euclidean Distance between two points
	*/
	template <typename T>
	T Distance(const fedes::Vector3<T>& point, const fedes::Vector3<T>& second_point) {
		return sqrt(DistanceSquared(point, second_point));
	}


	/* Explicit template instantiations */
	template double DistanceSquared<double>(const fedes::Vector3<double>&, const fedes::Vector3<double>&);
	template float  DistanceSquared<float>(const fedes::Vector3<float>&, const fedes::Vector3<float>&);
	
	template double Distance<double>(const fedes::Vector3<double>&, const fedes::Vector3<double>&);
	template float  Distance<float>(const fedes::Vector3<float>&, const fedes::Vector3<float>&);
}