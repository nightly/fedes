#pragma once

#include <concepts>
#include <span>
#include <limits>
#include <algorithm>

#include "fedes/maths/Vector3.h"
#include "fedes/maths/distance.h"
#include "fedes/indexing/octree/octant.h"

namespace fedes {
	
	template <std::floating_point T>
	struct OctantComparator {
	public:
		using Octant = fedes::internal::Octant<T>;
		using Vector3 = fedes::Vector3<T>;
	public:
		const Vector3& query;
	public:
		/*
		 * Octant Comparator constructor 
		 */
		OctantComparator(const Vector3& query)
			: query(query) {}

		/* 
		 * @brief operator() overload for usage as our comparator - sorting by increasing order
		 */
		bool operator () (const Octant* a, const Octant* b) {
			return a->MinimumDistanceSq(query) > b->MinimumDistanceSq(query); // Min heap
		}

	};

}