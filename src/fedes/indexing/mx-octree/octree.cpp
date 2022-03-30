#include "fedes/indexing/mx-octree/octree.h"

#include <span>
#include <vector>
#include <stdexcept>
#include <memory>
#include <limits>
#include <array>
#include <unordered_map>
#include <bitset>
#include <queue>

#include <iostream>
#include <string>

#include "fedes/maths/vector3.h"
#include "fedes/indexing/mx-octree/octant.h"
#include "fedes/indexing/mx-octree/octant_comparator.h"
#include "fedes/indexing/mx-octree/traversals.h"
#include "fedes/maths/distance.h"
#include "fedes/maths/z_ordering.h"

namespace fedes {

	/**
	 * @brief Index by node. Octree will be constructed based on a set of unique points provided.
	 * @tparam T: point type of Octree, e.g. double, float
	 * @param points: Unique set of points to be indexed from any std::span-compliant container (array, vector, etc)
	 * @param points_per_leaf: the threshold number of points to be contained within a leaf before splitting into a branch
	 * @param max_depth: the maximum depth for the Octree
	 * @exception length_error: If an empty set of points is sent, no Octree will be constructed
	 */
	template <typename T>
	Octree<T>::Octree(const std::span<Vector3>& points, const size_t points_per_leaf, const size_t max_depth) 
		: points_(points), points_per_leaf_(points_per_leaf), max_depth_(max_depth) {
		if (points_.empty()) {
			throw std::length_error("Octree Constructor: Empty set of initial points sent");
		}
		ConstructRoot();
		for (size_t i = 0; i != points_.size(); i++) {
			Insert(*root_, i, 0);
		}
	}

	/*
	 * @brief Index by element constructor overload for the Octree, using unique points and corresponding elements.
	 * @tparam T: point type of Octree, e.g. double, float
	 * @param points: Unique set of points to be indexed from any std::span-compliant container (array, vector, etc)
	 * @param elements: a 2-dimensional std::span compliant container that details elements with a list of its nodes
	 * @param points_per_leaf: the threshold number of points to be contained within a leaf before splitting into a branch
	 * @param max_depth: the maximum depth for the Octree
	 * @exception length_error: If an empty set of points or elements are sent, no Octree will be constructed
	 */
	template <typename T>
	Octree<T>::Octree(const std::span<Vector3>& points, const std::span<std::span<size_t>>& elements, size_t points_per_leaf, size_t max_depth) 
		: points_(points), elements_(elements), points_per_leaf_(points_per_leaf), max_depth_(max_depth) {
		if (points_.empty()) {
			throw std::length_error("Octree Element Index Constructor: Empty set of initial points sent");
		}
		if (elements_.empty()) {
			throw std::length_error("Octree Element Index Constructor: Empty set of elements sent");
		}
		ConstructRoot();
		for (size_t i = 0; i != points_.size(); i++) {
			Insert(*root_, i, 0);
		}
		node_elements_.resize(points_.size());
		for (size_t e = 0; e < elements_.size(); e++) {
			for (size_t n = 0; e < elements_[e].size(); n++) {
				node_elements_[elements_[e][n]].emplace_back(e);
			}
		}
	}

	/* @brief Calculates and sets the root Octant/node (center & extent) from the given set of points.
	 */
	template <typename T>
	void Octree<T>::ConstructRoot() {
		fedes::Vector3<T> min(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
		fedes::Vector3<T> max(std::numeric_limits<T>::min(), std::numeric_limits<T>::min(), std::numeric_limits<T>::min());
		fedes::Vector3<T> center;
		for (auto& v : points_) {
			center += v;
			if (v.x > max.x) {
				max.x = v.x;
			}
			if (v.x < min.x) {
				min.x = v.x;
			}

			if (v.y > max.y) {
				max.y = v.y;
			}
			if (v.y < min.y) {
				min.y = v.y;
			}

			if (v.z > max.z) {
				max.z = v.z;
			}
			if (v.z < min.z) {
				min.z = v.z;
			}
		}
		fedes::Vector3<T> extent((max.x - min.x), (max.y - min.y), (max.z - min.z));
		center /= points_.size();
		root_ = new Octant(center, extent / 2, nullptr);
	}

	/*
	* @brief Inserts a point starting from the given Octant
	* @param octant: the Octant to insert into from. The root at the start, and in recursive cases, any arbitrary Octant.
	* @param point_id: the point index to insert into the correct Octant from points_
	* @param size_t depth: the current depth of the given Octant
	*/
	template <typename T>
	void Octree<T>::Insert(Octant& octant, size_t point_id, size_t depth) {
		// Handle leaf node case
		if (octant.IsLeaf()) {
			// We examine our splitting criterion and accordingly either allow the insertion or split first
			if (octant.points.size() <= points_per_leaf_ || depth == max_depth_) {
				octant.points.emplace_back(point_id);
				return;
			} else { 
				return Split(octant, point_id, depth);
			}
		}
		// Handle branch node/interior node case
		uint_fast8_t oct = octant.DetermineChildOctant(points_[point_id]);
		return Insert(*(octant.child[oct]), point_id, ++depth);
	}

	/*
	 * @brief Splits a leaf node into 8 new nodes and re-inserts the old point indexes into the newly created leaf octants
	 * @param octant: the leaf Octant that will be split and as a result turn into a branch Octant
	 * @param point_id: the point ID of points_ to be inserted straight from this Octant
	 * @param depth: the depth of the Octant being split
	 */
	template<typename T>
	void Octree<T>::Split(Octant& octant, size_t point_id, size_t depth) {
		std::vector<size_t> current_points = std::move(octant.points);

		for (uint_fast8_t i = 0; i < 8; i++) {
			fedes::Vector3<T> split_center = octant.center;
			split_center.x += octant.extent.x * (i & 4 ? 0.5 : -0.5);
			split_center.y += octant.extent.y * (i & 2 ? 0.5 : -0.5);
			split_center.z += octant.extent.z * (i & 1 ? 0.5 : -0.5);
			octant.child[i] = new Octant(split_center, octant.extent / 2, &octant);
		}

		for (auto& p : current_points) {
			Insert(octant, p, depth);
		}
		Insert(octant, point_id, depth);
	}

	/*
	 * @brief Searches and checks whether a given point exists within the Octree, starting from the root.
	 * @param point: the point to search for. Must exactly match
	 * @return True/false depending whether the point is contained within the Octree
	 */
	template <typename T>
	bool Octree<T>::Find(const Vector3& point) const {
		return FindAtOctant(point, *root_);
	}

	/*
	 * @brief Returns true if a point exists within the Octree, false otherwise
	 * @param point: the point to search for. Must exactly match
	 * @param octant: the octant to search recursively from.
	 * @return True/false depending whether the point is contained within the Octree
	 */
	template <typename T>
	bool Octree<T>::FindAtOctant(const Vector3& point, const Octant& octant) const {
		if (octant.IsLeaf()) {
			if (!octant.IsEmpty()) {
				for (auto& p : octant.points) {
					if (points_[p] == point) {
						return true;
					}
				}
				return false;
			} else {
				return false;
			}
		}
		return FindAtOctant(point, *octant.child[octant.DetermineChildOctant(point)]);
	}

	/*
	* @brief Creates a new order post order iterator from root octant
	*/
	template <typename T>
	Octree<T>::post_order_iterator Octree<T>::post_begin() {
		return post_order_iterator(root_);
	}

	/*
	 * @brief Determines and creates the end-point of the post-order-iterator
	 */
	template <typename T>
	Octree<T>::post_order_iterator Octree<T>::post_end() {
		return post_order_iterator(nullptr);
	}

	/* @brief Locates the nearest point to the given query point assuming a static Octree index
	* @param query_point: query point to search the nearest neighbour(s) for
	* @return Nearest point as the index identifier from the original container
	*/
	template <typename T>
	size_t Octree<T>::Nearest(const Vector3& query_point) const {
		return NearestSearch(query_point, *root_);
	}

	/* @brief Performs the nearest point search recursively
	* 
	* This algorithm is optimized and designed with the following in mind:
	* a) only 1 nearest neighbour needs to be returned
	* b) no deletions occur from the index once created — in FEDES, meshes are static
	*
	* @param query_point: the query point to search the nearest neighbour(s) for
	* @param octant: the current Octant we are searching recursively from
	* @return Nearest point as the index identifier from the original container
	*/
	template <typename T>
	size_t Octree<T>::NearestSearch(const Vector3& query_point, const Octant& octant) const {
		if (!octant.IsLeaf()) { // Interior node case
			uint_fast8_t oct = octant.DetermineChildOctant(query_point);
			Octant* child = (octant.child[oct]);
			if (!child->IsLeaf() || !child->IsEmpty()) {
				// The node we wish to traverse to is an interior node or leaf node with points
				return NearestSearch(query_point, *child);
			} else {
				// The child node we wanted to traverse to is a leaf with no points, therefore, we must select a different node
				Octant* best_octant = nullptr;
				T best_distance = std::numeric_limits<T>::max();
				for (uint_fast8_t i = 0; i < 8; i++) {
					if ((i == oct) || ((octant.child[i]->IsLeaf()) && octant.child[i]->IsEmpty())) {
						continue; // Exclude the node previously selected as well as any node that can't be traversed to
					}
					T distance = fedes::DistanceSquared(query_point, octant.child[i]->center);
					if (distance < best_distance) {
						best_octant = octant.child[i];
						best_distance = distance;
					}
				}
				return NearestSearch(query_point, *best_octant);
			}
		} else {
			// We have arrived at the best final leaf, although since leaves may contain more than 1 point, some searching is left
			std::size_t best_point_index{};
			T best_distance = std::numeric_limits<T>::max();
			for (auto& p : octant.points) {
				T distance = fedes::DistanceSquared(query_point, points_[p]);
				if (distance < best_distance) {
					best_distance = distance;
					best_point_index = p;
				}
			}
			// Check neighbouring Octants for minimum possible distance to query point 
			// versus current best distance to see if we must check them at all.
			return best_point_index;
		}
	}

	/*
	 * @brief Performs Field of Point search
	 * @param query_point: the query point to use as origin for the search
	 * @return An array of points found in each quadrant. If a point cannot be found in a direction, it will be NaN.
	 */
	template <typename T>
	std::array<Vector3<T>, 8> Octree<T>::FieldOfPoints(const Vector3& query_point) const {
		std::array<Vector3, 8> arr;
		return arr;
	}


	/*
	 * @brief Clears the octree and deallocates all memory
	 * 
	 * The approach used is to use post-order traversal to clear the tree and its Octants, instead of using
	 * a chain of destructors calling each child octant with `delete` recursively
	 */
	template<typename T>
	void Octree<T>::Clear() {
		post_order_iterator it = post_begin();
		while (it != post_end()) {
			auto octant = it.operator->();
			delete octant;
			++it;
		}
	}

	/*
	* @brief Octree destructor
	*/
	template <typename T>
	Octree<T>::~Octree() noexcept {
		Clear();
	}

	/* Explicit template instantiations */
	template class Octree<double>;
	template class Octree<float>;
}