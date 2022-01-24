#include "fedes/indexing/octree/octant.hpp"

#include <array>
#include <tuple>
#include <matchit.h>

#include <iostream>
#include <string>

#include "fedes/maths/vector3.hpp"

namespace fedes::internal {

	/* 
	 * @brief Octant constructor, creating the octant with the given center and extent
	 *
	 * @tparam T: the type of point contained within the Octant, i.e. double/float
	 * @param center: the center of this Octant
	 * @param extent: how far the octant extends within each axis (width, height, depth)
	 */
	template<typename T>
	Octant<T>::Octant(const Vector3<T>& center, const Vector3<T>& extent)
		: center(center), extent(extent) {
	}

	/*
	 * @brief Returns true/false if the Octant is a leaf, using the invariant that there are either 0 or 8 children.
	 */
	template<typename T>
	bool Octant<T>::IsLeaf() const {
		return child[0] == nullptr;
	}

	/*
	 * @brief Returns true if the Octant contains a point, false otherwise
	 */
	template<typename T>
	bool Octant<T>::IsEmpty() const {
		return points.empty();
	}

	/*
	 * @brief Provided a point, returns which octant/node to traverse to given this octant's center
	 *
	 * Morton Encoding, also known as Z-Ordering is used here. The point in question is compared to the octant's center
	 * to determine whether it is larger in the X, Y, and Z axis'. Then pattern matching is used to determine the correct
	 * child octant to return. Example: 000 would return 0, where as 111 would return 7. Used for insertion & traversal.
	 * 
	 * @param point: the point in question to determine the correct sub-Octant for based on the current Octant.
	 * 
	 * @returns unsigned integer from 0 to 7 representing the child octant within the child[] array
	 */
	template<typename T>
	uint8_t Octant<T>::DetermineChildOctant(const Vector3<T>& point) const {
		using namespace matchit;

		bool x_larger = point.x >= center.x;
		bool y_larger = point.y >= center.y;
		bool z_larger = point.z >= center.z;
		std::tuple<bool, bool, bool> tuple{ x_larger, y_larger, z_larger };
	
		return match(tuple)(
		pattern | ds(true, true, true) = [] { return 7; },
		pattern | ds(true, true, false) = [] { return 6; },
		pattern | ds(true, false, true) = [] { return 5; },
		pattern | ds(true, false, false) = [] { return 4; },
		pattern | ds(false, true, true) = [] { return 3; },
		pattern | ds(false, true, false) = [] { return 2; },
		pattern | ds(false, false, true) = [] { return 1; },
		pattern | ds(false, false, false) = [] { return 0; }
		);
	}

	/*
	* @brief Octant destructor
	*/
	template <typename T>
	Octant<T>::~Octant() {
		
	}
	
	/* Explicit template instantiations */
	template class Octant<double>;
	template class Octant<float>;
}