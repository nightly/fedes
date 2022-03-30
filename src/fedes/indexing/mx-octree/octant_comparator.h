#pragma once

#include "fedes/maths/Vector3.h"
#include "fedes/indexing/mx-octree/octant.h"

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