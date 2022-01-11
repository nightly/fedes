#pragma once 

#include <array>
#include <vector>

#include "fedes/maths/vector3.hpp"

namespace fedes::internal {
	template <typename T>
	class Octant {
	public:
		Vector3<T> extent;
		Vector3<T> center;
		std::vector<size_t> points;
		std::array<Octant<T>*, 8> child = {nullptr}; // @Todo: hold one pointer instead of 8
	public:
		Octant(const Vector3<T>& center, const Vector3<T>& extent);
		Octant(const Vector3<T>& center, const Vector3<T>& extent, const Vector3<T>& point);
		~Octant();

		Octant(const Octant& other) = delete;
		Octant& operator=(const Octant& other) = delete;
		Octant(Octant&& other) = delete;
		Octant& operator=(Octant&& other) = delete;

		bool IsLeaf() const;
		bool IsEmpty() const;
		uint8_t DetermineChildOctant(const Vector3<T>& point) const;
	};
}