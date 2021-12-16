#pragma once

#include <array>
#include <span>

#include "fedes/data/vector3.h"
#include "fedes/indexing/octant.h"

namespace fedes {
	using namespace fedes::internal;
	
	template<typename T>
	class Octree {
	private:
		Octant<T>* root_ = nullptr;
	public:
		Octree(const std::span<Vector3<T>>& points);
		~Octree();

		Octree(const Octree& other);
		Octree& operator=(const Octree& other);
		Octree(Octree&& other);
		Octree& operator=(Octree&& other);

		void Insert(const Vector3<T>& point);
		bool Find(const Vector3<T>& point) const;
		void Split(Octant<T>* octant, const Vector3<T>& insertion_point);
	private:
		void InsertAtOctant(Octant<T>* octant, const Vector3<T>& point);
		void Clear();
	};
}