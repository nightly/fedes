#include "fedes/indexing/octant.h"

#include <array>
#include <unordered_map>

#include <mpark/patterns.hpp>

namespace fedes::internal {
	template<typename T>
	Octant<T>::Octant(const Vector3<T>& center, const Vector3<T>& extent)
		: center(center), extent(extent) {}

	template<typename T>
	Octant<T>::Octant(const Vector3<T>& center, const Vector3<T>& extent, const Vector3<T> _point)
		: center(center), extent(extent) {
		Vector3<T>* p = new Vector3<T>(_point.x, _point.y, _point.z);
		point = p;
	}

	// A node will either have 8 children or 0 children, allowing us to easily figure out whether a node is a leaf or not
	template<typename T>
	bool Octant<T>::IsLeaf() {
		return children[0] == nullptr;
	}

	// Morton Encoding/Z-Ordering
	// Where 111 represents index 7 (greater X, Y, and Z), and 000 represents index 0
	// Three booleans attained by comparing the point to the center of the octant for each axis
	template<typename T>
	int Octant<T>::DetermineChildOctant(const Vector3<T>& insertion_point) {
		using namespace mpark::patterns;

		bool x_larger = center.x > insertion_point.x;
		bool y_larger = center.y > insertion_point.y;
		bool z_larger = center.z > insertion_point.z;

		std::tuple<bool, bool, bool> tuple{x_larger, y_larger, z_larger};

		return match(tuple)(
			pattern(ds(true, true, true)) = [] { return 7; },
			pattern(ds(true, true, false)) = [] { return 6; },
			pattern(ds(true, false, true)) = [] { return 5; },
			pattern(ds(true, false, false)) = [] { return 4; },
			pattern(ds(false, true, true)) = [] { return 3; },
			pattern(ds(false, true, false)) = [] { return 2; },
			pattern(ds(false, false, true)) = [] { return 1; },
			pattern(ds(false, false, false)) = [] { return 0; });
	}

	template struct Octant<double>;
	template struct Octant<float>;
}