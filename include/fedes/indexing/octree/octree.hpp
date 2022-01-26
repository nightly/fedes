#pragma once

#include <array>
#include <span>
#include <vector>
#include <queue>
#include <optional>
#include <functional>

#include "fedes/indexing/octree/traversals.hpp"
#include "fedes/maths/vector3.hpp"
#include "fedes/indexing/octree/octant.hpp"

namespace fedes {	

	template<typename T>
	class Octree {
	public:
		using value_type = T;
		using Octant = fedes::internal::Octant<T>;
		using post_order_iterator = fedes::OctreePostOrderIterator<fedes::Octree<T>>;
	private:
		Octant* root_ = nullptr;
		std::span<Vector3<T>> points_;

		size_t points_per_leaf_;
		size_t max_depth_;
	public:
		Octree() = delete;
		Octree(const std::span<Vector3<T>>& points, size_t points_per_leaf, size_t max_depth);
		~Octree();

		Octree(const Octree& other) = delete;
		Octree& operator=(const Octree& other) = delete;
		Octree(Octree&& other) noexcept = delete;
		Octree& operator=(Octree&& other) noexcept = delete;

		bool Find(const Vector3<T>& point) const;
		size_t Nearest(const Vector3<T>& query_point) const;
		std::array<fedes::Vector3<T>, 8> FieldOfPoints(const Vector3<T>& query_point);

		post_order_iterator post_begin();
		post_order_iterator post_end();
	private:
		void InsertAtOctant(Octant& octant, size_t point_id, size_t depth);
		void Split(Octant& octant, size_t point_id, size_t depth);
		bool FindAtOctant(const Vector3<T>& point, const Octant& octant) const;
		size_t NearestSearch(const Vector3<T>& query_point, const Octant& octant) const;
		void Clear();
	};
}