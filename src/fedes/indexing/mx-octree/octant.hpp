#pragma once 

#include <array>
#include <vector>

#include "fedes/maths/vector3.hpp"

namespace fedes::internal {
	template <typename T>
	class Octant {
	public:
		using value_type = T;
		Vector3<T> center;
		Vector3<T> extent; // Extent = half of each dimension (half width, height, depth)
		std::vector<size_t> points;
		std::array<Octant<T>*, 8> child = {nullptr}; // @Todo: hold one pointer instead of 8
		Octant<T>* parent = nullptr;
	public:
		Octant(const Vector3<T>& center, const Vector3<T>& extent);
		Octant(const Vector3<T>& center, const Vector3<T>& extent, Octant<T>* parent);
		Octant(Octant<T>* parent);
		~Octant() noexcept;

		Octant(const Octant& other) = delete;
		Octant& operator=(const Octant& other) = delete;
		Octant(Octant&& other) = delete;
		Octant& operator=(Octant&& other) = delete;

		bool IsLeaf() const;
		bool IsEmpty() const;
		uint_fast8_t DetermineChildOctant(const Vector3<T>& point) const;
		T MinimumDistanceSq(const Vector3<T>& point) const;
	};
}