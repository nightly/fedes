// Octant: represents a single Node of an Octree.
#pragma once 

#include <array>

#include "fedes/maths/vector3.hpp"

namespace fedes::internal {
	template <typename T>
	struct Octant {
	public:
		Vector3<T> extent;
		Vector3<T> center;
		Vector3<T>* point = nullptr;
		std::array<Octant<T>*, 8> children = {nullptr};
	public:
		Octant(const Vector3<T>& center, const Vector3<T>& extent);
		Octant(const Vector3<T>& center, const Vector3<T>& extent, const Vector3<T>& point);

		Octant(const Octant& other);
		Octant& operator=(const Octant& other);
		Octant(Octant&& other);
		Octant& operator=(Octant&& other);

		bool IsLeaf() const;
		int DetermineChildOctant(const Vector3<T>& insertion_point) const;
	};
}