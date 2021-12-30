#pragma once

#include <array>
#include <span>

#include "fedes/maths/vector3.hpp"
#include "fedes/indexing/octree/octant.hpp"

namespace fedes {
	using namespace fedes::internal;
	
	template<typename T>
	class Octree {
	private:
		Octant<T>* root_ = nullptr;
	public:
		Octree(const std::span<Vector3<T>>& points);
		~Octree();

		Octree(const Octree& other) = delete;
		Octree& operator=(const Octree& other) = delete;
		Octree(Octree&& other) = delete;
		Octree& operator=(Octree&& other) = delete;

		bool Find(const Vector3<T>& point) const;
	private:
		void InsertAtOctant(Octant<T>* octant, const Vector3<T>& point);
		void Split(Octant<T>* octant, const Vector3<T>& insertion_point);
		void Clear();
	};
}