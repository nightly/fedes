#pragma once

#include <array>
#include <span>
#include <vector>
#include <queue>
#include <optional>

#include "fedes/indexing/octree/traversals.hpp"
#include "fedes/maths/vector3.hpp"
#include "fedes/indexing/octree/octant.hpp"

namespace fedes {	

	template<typename T>
	class Octree {
	public:
		using Octant = fedes::internal::Octant<T>;
		using post_order_iterator = fedes::OctreePostOrderIterator<fedes::Octree<T>>;
	private:
		Octant* root_ = nullptr;
		std::vector<Vector3<T>> points_;

		size_t points_per_leaf_;
		size_t max_depth_;
	public:
		Octree() = delete;
		Octree(const std::vector<Vector3<T>>& points, size_t points_per_leaf, size_t max_depth);
		~Octree();

		Octree(const Octree& other) noexcept = delete;
		Octree& operator=(const Octree& other) noexcept = delete;
		Octree(Octree&& other) noexcept;
		Octree& operator=(Octree&& other) noexcept;

		bool Find(const Vector3<T>& point) const;
		std::vector<T> KNearestNeighbours(std::queue<Octant>& visit_queue, std::queue<T>& result_queue, fedes::Vector3<T> point, size_t k) const;
		std::array<std::optional<T>, 8> FieldOfPoints(fedes::Vector3<T> point) const;

		post_order_iterator post_begin();
		post_order_iterator post_end();
	private:
		void InsertAtOctant(Octant& octant, size_t point_id, size_t depth);
		void Split(Octant& octant, size_t point_id, size_t depth);
		bool FindAtOctant(const Vector3<T>& point, const Octant& octant) const;
		void Clear();
	};
}