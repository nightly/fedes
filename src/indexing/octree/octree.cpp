#include "fedes/indexing/octree/octree.hpp"

#include <span>
#include <vector>
#include <stdexcept>
#include <iostream>

#include "fedes/maths/vector3.hpp"
#include "fedes/indexing/octree/octant.hpp"
#include "fedes/indexing/octree/traversals.hpp"

namespace fedes {

	/**
	 * @brief Octree will be constructed based on a set of unique points provided
	 *
	 * @tparam T: point type of Octree, e.g. double, float
	 * @param points: Unique set of points to be indexed from any std::span-compliant container (array, vector, etc)...
	 * @param points_per_leaf: the maximum number of points to be contained within a leaf before splitting into a branch
	 * @param max_depth: the maximum depth for the Octree
	 * 
	 * @exception length_error If an empty set of points is sent, no Octree will be constructed
	 */
	template <typename T>
	Octree<T>::Octree(const std::vector<Vector3<T>>& points, const size_t points_per_leaf, const size_t max_depth) 
		: points_(points), points_per_leaf_(points_per_leaf), max_depth_(max_depth) {
		if (points_.empty()) {
			throw std::length_error("Octree Constructor: Empty set of initial points sent");
		}
		Vector3<T> min, max;
		for (auto& v : points) {
			if (v.x > max.x) {
				max.x = v.x;
			}
			else if (v.x < min.x) {
				min.x = v.x;
			}

			if (v.y > max.y) {
				max.y = v.y;
			}
			else if (v.y < min.y) {
				min.y = v.y;
			}

			if (v.z > max.z) {
				max.z = v.z;
			}
			else if (v.z < min.z) {
				min.z = v.z;
			}
		}
		Vector3<T> center = Vector3<T>((max.x + min.x) / 2, (max.y + min.y) / 2, (max.z + min.z) / 2);
		Vector3<T> extent = Vector3<T>((max.x - min.x), (max.y - min.y), (max.z - min.z));

		root_ = new Octant(center, extent / 2);

		for (size_t i = 0; i != points_.size(); i++) {
			InsertAtOctant(*root_, i, 0);
		}
	}

	/*
	* @brief Inserts a point starting from the given Octant
	* 
	* @param octant: the Octant to insert into from. The root at the start, and in recursive cases, any arbitrary Octant.
	* @param point_id: the point index to insert into the correct Octant from points_
	* @param size_t depth: the current depth of the given Octant
	*/
	template<typename T>
	void Octree<T>::InsertAtOctant(Octant& octant, size_t point_id, size_t depth) {
		// Handle leaf node case
		if (octant.IsLeaf()) {
			// We examine if the Octant has no points or allow an insertion if our splitting criterion isn't active
			if (octant.IsEmpty() || ((octant.points.size() < points_per_leaf_) || (depth == max_depth_))) {
				octant.points.emplace_back(point_id);
				return;
			} else { // Our splitting criterion is active
				return Split(octant, point_id, depth);
			}
		}
		// Handle branch node/interior node case
		int oct = octant.DetermineChildOctant(points_[point_id]);
		return InsertAtOctant(*(octant.child[oct]), point_id, ++depth);
	}

	/*
	 * @brief Splits a leaf node into 8 new nodes and re-inserts the old point indexes into the newly created leaf octants
	 * 
	 * Insertions from this function will occur directly from this Octant instead of starting from the root
	 * 
	 * @param octant: the leaf Octant that will be split and as a result turn into a branch Octant
	 * @param point_id: the point ID of points_ to be inserted straight from this Octant
	 * @param depth
	 */
	template<typename T>
	void Octree<T>::Split(Octant& octant, size_t point_id, size_t depth) {
		std::vector<size_t> current_points = octant.points;
		octant.points.clear();

		Vector3<T> split_origin;
		for (int i = 0; i < 8; i++) {
			split_origin.x = octant.extent.x * (i & 4 ? 0.5 : -0.5);
			split_origin.y = octant.extent.y * (i & 2 ? 0.5 : -0.5);
			split_origin.z = octant.extent.z * (i & 1 ? 0.5 : -0.5);
			octant.child[i] = new Octant(split_origin, octant.extent / 2);
		}

		for (auto& p : current_points) {
			InsertAtOctant(octant, p, depth);
		}
		InsertAtOctant(octant, point_id, depth);
	}

	/*
	 * @brief Searches and checks whether a given point exists within the Octree, starting from the root.
	 *
	 * @param point: the point to search for. Must exactly match
	 * @return True/false depending whether the point is contained within the Octree
	 */
	template <typename T>
	bool Octree<T>::Find(const Vector3<T>& point) const {
		return FindAtOctant(point, *root_);
	}

	/*
	 * @brief Returns true if a point exists within the Octree, false otherwise
	 *
	 * @param point: the point to search for. Must exactly match
	 * @param octant: the octant to search recursively from.
	 * 
	 * @returns True/false depending whether the point is contained within the Octree
	 */
	template <typename T>
	bool Octree<T>::FindAtOctant(const Vector3<T>& point, const Octant& octant) const {
		if (octant.IsLeaf()) {
			if (!octant.points.empty()) {
				bool found_match = false;
				for (auto& p : octant.points) {
					if (points_[p] == point) {
						found_match = true;
					}
				}
				return found_match;
			}
			else {
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

	/*
	* @brief K Nearest Neighbour
	*/
	template <typename T>
	Vector3<T> Octree<T>::NearestNeighbour(fedes::Vector3<T> point, size_t k) const {
		// Queue
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
	Octree<T>::~Octree() {
		Clear();
	}

	/* Explicit template instantiations */
	template class Octree<double>;
	template class Octree<float>;
}