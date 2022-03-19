#pragma once

#include "fedes/maths/Vector3.hpp"
#include "fedes/indexing/mx-octree/octant.hpp"

namespace fedes {
	
	template <typename T>
	struct OctantComparator {
	public:
		using Octant = fedes::internal::Octant<T>;
		const fedes::Vector3<T> query_point;
	public:
		OctantComparator(const fedes::Vector3<T>& query_point);
		bool operator () (const Octant* a, const Octant* b);
	};
}