// Octant: represents a single Node of an Octree.
#pragma once 

#include <array>
#include <unordered_map>

#include "fedes/data/vector3.h"

namespace fedes::internal {
	template <typename T>
	struct Octant {
	public:
		Vector3<T> extent;
		Vector3<T> center;
		Vector3<T>* point;
		std::array<Octant<T>*, 8> children;
	public:
		Octant(const Vector3<T>& center, const Vector3<T>& extent);
		Octant(const Vector3<T>& center, const Vector3<T>& extent, Vector3<T> point);

		Octant(const Octant& other);
		Octant& operator=(const Octant& other);
		Octant(Octant&& other);
		Octant& operator=(Octant&& other);

		bool IsLeaf();
		int DetermineChildOctant(const Vector3<T>& insertion_point);
	};
}