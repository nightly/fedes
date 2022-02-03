#include "fedes/maths/distance.hpp"

#include "fedes/maths/vector3.hpp"

#include <cmath>
#include <concepts>

namespace fedes {

	/*
	 * @brief Euclidean Distance Squared which omits the square root computation when the actual distance value isn't required
	 * @tparam T: numeric type, i.e. double, float
	 * @param p: 3 dimensional point
	 * @param q: second 3 dimensional point to compute squared distance against
	 * @returns Squared Distance between two points
	 */
	template <typename T>
	T DistanceSquared(const fedes::Vector3<T>& p,  const fedes::Vector3<T>& q) {
		return pow(q.x - p.x, 2) + pow(q.y - p.y, 2) + pow(q.z - p.z, 2);
	}

	/*
	 * @brief Euclidean Distance
	 * @tparam T: numeric type,  i.e. double, float
	 * @param p: 3 dimensional point
	 * @param q: the second 3 dimensional point to compute distance against
	 * @returns Euclidean Distance between two points
	 */
	template <typename T>
	T Distance(const fedes::Vector3<T>& p, const fedes::Vector3<T>& q) {
		return sqrt(DistanceSquared(p, q));
	}


	/* Explicit template instantiations */
	template double DistanceSquared<double>(const fedes::Vector3<double>&, const fedes::Vector3<double>&);
	template float  DistanceSquared<float>(const fedes::Vector3<float>&, const fedes::Vector3<float>&);
	
	template double Distance<double>(const fedes::Vector3<double>&, const fedes::Vector3<double>&);
	template float  Distance<float>(const fedes::Vector3<float>&, const fedes::Vector3<float>&);
}