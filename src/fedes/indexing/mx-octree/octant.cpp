#include "fedes/indexing/mx-octree/octant.hpp"

#include <array>
#include <iostream>
#include <string>

#include "fedes/maths/vector3.hpp"
#include "fedes/maths/z_ordering.hpp"

namespace fedes::internal {

	/* 
	 * @brief Octant constructor, creating the Octant with the given center and extent
	 * @tparam T: type of point contained within the Octant, i.e. double/float
	 * @param center: center of this Octant
	 * @param extent: half how far the Octant extends within each axis (width, height, depth)
	 */
	template <typename T>
	Octant<T>::Octant(const Vector3<T>& center, const Vector3<T>& extent)
		: center(center), extent(extent) {
	}

	/*
	 * @brief Octant constructor overload, creating the Octant with the given center and extent + parent
	 * @tparam T: type of point contained within the Octant, i.e. double/float
	 * @param center: center of this Octant
	 * @param extent: half how far the Octant extends within each axis (width, height, depth)
	 * @param parent: the parent of the newly created Octant
	 */
	template <typename T>
	Octant<T>::Octant(const Vector3<T>& center, const Vector3<T>& extent, Octant<T>* parent)
		: center(center), extent(extent), parent(parent) {
	}

	/*
	 * @brief Octant constructor overload, creating the Octant with the given parent
	 * @tparam T: type of point contained within the Octant, i.e. double/float
	 * @param parent: the parent of the newly created Octant
	 */
	template <typename T>
	Octant<T>::Octant(Octant<T>* parent)
		: parent(parent) {
	}

	/*
	 * @brief Returns true/false if the Octant is a leaf, using the invariant that there are either 0 or 8 children.
	 */
	template <typename T>
	bool Octant<T>::IsLeaf() const {
		return child[0] == nullptr;
	}

	/*
	 * @brief Returns true if the Octant contains a point, false otherwise
	 */
	template <typename T>
	bool Octant<T>::IsEmpty() const {
		return points.empty();
	}

	/*
	 * @brief Provided a point, returns which octant/node to traverse using the current Octant's center
	 *
	 * Morton Encoding, also known as Z-Ordering is used here. The point in question is compared to the Octant's center
	 * to determine whether it is larger in the X, Y, and Z axis'. Then pattern matching is used to determine the correct
	 * child octant to return. Examples: 000 would return 0, whereas 111 would return 7. Used for insertion & traversal.
	 * 
	 * @param point: the point in question to determine the correct sub-Octant for based on the current Octant.
	 * @return unsigned integer from 0 to 7 representing the child octant within the child[] array
	 */
	template <typename T>
	uint_fast8_t Octant<T>::DetermineChildOctant(const Vector3<T>& point) const {
		return fedes::DetermineDirection(center, point);
	}

	/*
	 * @brief Calculates the minimum squared distance to a given point from this Octant
	 * @param point: 3-dimensional point to calculate minimum distance against
	 * @return Minimum distance squared against the provided point
	 */
	template <typename T>
	T Octant<T>::MinimumDistanceSq(const Vector3<T>& point) const {
		return 0;
	}

	/*
	* @brief Octant destructor
	*/
	template <typename T>
	Octant<T>::~Octant() noexcept {
		
	}
	
	/* Explicit template instantiations */
	template class Octant<double>;
	template class Octant<float>;
}