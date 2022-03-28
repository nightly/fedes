#include "fedes/indexing/mx-octree/octant_comparator.hpp"

#include "fedes/maths/vector3.hpp"
#include "fedes/indexing/mx-octree/octant.hpp"

namespace fedes {

	/*
	 * @brief Octant Comparator constructor. Used in the priority queue.
	 * @tparam T: the type of point data (e.g. float, double).
	 * @param query_point: the query point to sort the Octants by, as we use minimum distance to the query point.
	 */
	template <typename T>
	OctantComparator<T>::OctantComparator(const fedes::Vector3<T>& query_point) 
		: query_point(query_point) {
	}


	/*
	 * @brief Operator() overload — the comparator.
	 * @returns True if b's minimum distance to the query point is smaller than a's, false otherwise.
	 */
	template <typename T>
	bool OctantComparator<T>::operator () (const Octant* a, const Octant* b) {
		if (a->MinimumDistanceSq(query_point) < b->MinimumDistanceSq(query_point)) {
			return true;
		}
		return false;
	}

	/* Explicit template instantiations */
	template struct OctantComparator<double>;
	template struct OctantComparator<float>;
}