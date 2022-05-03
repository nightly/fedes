#pragma once

#include <array>
#include <span>
#include <vector>
#include <optional>
#include <queue>
#include <functional>
#include <limits>
#include <stdexcept>
#include <concepts>
#include <unordered_set>
#include <utility>

#include <thread_pool.hpp>

#include "fedes/maths/vector3.h"
#include "fedes/maths/distance.h"
#include "fedes/maths/z_ordering.h"
#include "fedes/maths/element_type.h"
#include "fedes/maths/geometry.h"
#include "fedes/indexing/octree/traversals.h"
#include "fedes/indexing/octree/octant.h"
#include "fedes/indexing/octree/octant_comparator.h"
#include "fedes/common/log.h"

namespace fedes {	

	template<std::floating_point PointT = double>
	class Octree {
	public:
		using value_type = PointT;
		using Octant = fedes::internal::Octant<PointT>;
		using Vector3 = fedes::Vector3<PointT>;
		using post_order_iterator = fedes::OctreePostOrderIterator<fedes::Octree<PointT>>;
	private:
		Octant* root_ = nullptr;

		const std::span<Vector3>* points_;
		const std::vector<std::vector<size_t>>* elements_ = nullptr;
		std::vector<std::vector<size_t>> node_elements_; // @brief: Node to element lookup
		ElementType element_type_;
		thread_pool* pool_;

		size_t leaf_split_threshold_; // @brief: Splitting threshold - until a leaf contains X points, prevent splitting
		size_t max_depth_; // @brief: Maximal depth of the Octree, where root Octant is at depth = 0
	public:

		// ====================================================================
		// Constructors & destructor
		// ====================================================================

		Octree() = delete;

		/*
		 * @brief Node index constructor overload
		 */
		Octree(const std::span<Vector3>& points, size_t max_depth, size_t leaf_split_threshold, 
			  std::optional<thread_pool*> pool = std::nullopt)
		: points_(&points), leaf_split_threshold_(leaf_split_threshold), max_depth_(max_depth) {
			if (points_->empty()) {
				throw std::length_error("Octree Index Constructor: Empty set of initial points sent");
			}
			
			if (pool.has_value()) {
				pool_ = pool.value();
				ParallelConstructRoot();
			} else {
				ConstructRoot();
			}

			for (size_t i = 0; i != points_->size(); i++) {
				Insert(*root_, i, 0);
			}
			FEDES_INFO("[Octree Node Index]: parallelize = {}, max depth {}, leaf split threshold {}", pool.has_value(),
				max_depth_, leaf_split_threshold_);
			FEDES_INFO("[Octree Node Index]: root center {}, root 1/2 e: {}, number of points {}", root_->center,
				root_->extent, points_->size());
		}

		/*
		 * @brief Element index constructor overload
		 */
		Octree(const std::span<Vector3>& points, const std::vector<std::vector<size_t>>& elements, size_t max_depth, 
			  size_t leaf_split_threshold, std::optional<thread_pool*> pool = std::nullopt)
		: points_(&points), elements_(&elements), leaf_split_threshold_(leaf_split_threshold), max_depth_(max_depth) {
			if (points_->empty()) {
				throw std::length_error("Octree Element Index Constructor: Empty set of initial points sent");
			}
			if (elements_->empty()) {
				throw std::length_error("Octree Element Index Constructor: Empty set of elements sent");
			}
			element_type_ = DetermineElementType(*elements_);
			
			if (pool.has_value()) {
				pool_ = pool.value();
				ParallelConstructRoot();
				ParallelNodeElementMap();
			} else {
				ConstructRoot();
				NodeElementMap();
			}

			for (size_t i = 0; i != points_->size(); i++) {
				Insert(*root_, i, 0);
			}
			FEDES_INFO("[Octree Element Index]: parallelize = {}, max depth {}, leaf split threshold {}", pool.has_value(),
				max_depth, leaf_split_threshold_);
			FEDES_INFO("[Octree Element Index]: root center {}, root 1/2 e: {}, number of points {}, number of elements {}", root_->center,
				root_->extent, points_->size(), elements_->size());
			FEDES_INFO("[Octree Element Index]: element type is {}", element_type_);
		}

		~Octree() noexcept {
			Clear();
		}

		Octree(const Octree& other) = delete;
		Octree& operator=(const Octree& other) = delete;
		Octree(Octree&& other) noexcept = delete;
		Octree& operator=(Octree&& other) noexcept = delete;

		// ====================================================================
		// Searches (pick, NPM, DMUFOP, DMUE, ESF)
		// ====================================================================
		
		/*
		 * @brief Returns -1 or the given index if the point is located. 
		 */
		[[nodiscard]] int_fast64_t Find(const Vector3& point) const {
			return Find(point, *root_);
		}

		[[nodiscard]] size_t Nearest(const Vector3& query_point) const {
			return NearestNeighbour(query_point);
		}

		[[nodiscard]] std::array<std::pair<size_t, PointT>, 8> FieldOfPoints(const Vector3& query_point, const PointT max_radius) const {
			return FieldSearch(query_point, *root_, max_radius);
		}

		[[nodiscard]] size_t DMUE(const Vector3& query_point, size_t min_element_scan = 20) const {
			return DMUESearch(query_point, min_element_scan);
		}
		
		/* 
		 * @param leaf_threshold: In case of differing geometries — the whole tree need not be searched before the boundaries are relaxed.
		 */
		[[nodiscard]] std::pair<size_t, MuesfData> MUESF(const Vector3& query_point, size_t leaf_threshold) const {
			return MUESFSearch(query_point, leaf_threshold);
		}

		void RadiusSearch(const Vector3& query_point, PointT radius, std::vector<size_t>& results) const {
			PointT radius_sq = radius * radius;
			RadiusSearch(*root_, query_point, radius, radius_sq, results);
		}

		// ====================================================================
		// General purpose getters/information
		// ====================================================================

		/*
		 * @brief Returns the center and extent information for the root Octant. 
		 */
		std::tuple<Vector3 , Vector3> root() const {
			return { root_->center, root_->extent };
		}

		const std::vector<std::vector<size_t>>& node_elements() const {
			return node_elements_;
		}

		ElementType element_type() const {
			return element_type_;
		}

		// ====================================================================
		// Iterators & traversals 
		// ====================================================================

		/*
		 * @brief Creates a new order post order iterator from root octant
		 */
		post_order_iterator post_begin() {
			return post_order_iterator(root_);
		}

		/*
		 * @brief Determines and creates the end-point of the post-order-iterator
		 */
		post_order_iterator post_end() {
			return post_order_iterator(nullptr);
		}

	private:

		// ====================================================================
		// Construction, insertion, splitting, and destruction
		// ====================================================================

		void ConstructRoot() {
			Vector3 min(std::numeric_limits<PointT>::max(), std::numeric_limits<PointT>::max(), std::numeric_limits<PointT>::max());
			Vector3 max(std::numeric_limits<PointT>::min(), std::numeric_limits<PointT>::min(), std::numeric_limits<PointT>::min());
			Vector3 center;
			for (auto& v : *points_) {
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

			Vector3 extent((max.x - min.x), (max.y - min.y), (max.z - min.z));
			center = (max + min) / 2;
			root_ = new Octant(center, extent / 2, nullptr);
		}

		void ParallelConstructRoot() {
			Vector3 min(std::numeric_limits<PointT>::max(), std::numeric_limits<PointT>::max(), std::numeric_limits<PointT>::max());
			Vector3 max(std::numeric_limits<PointT>::min(), std::numeric_limits<PointT>::min(), std::numeric_limits<PointT>::min());
			Vector3 center;
			for (auto& v : *points_) {
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

			Vector3 extent((max.x - min.x), (max.y - min.y), (max.z - min.z));
			center = (max + min) / 2;
			root_ = new Octant(center, extent / 2, nullptr);
		}

		void NodeElementMap() {
			node_elements_.resize(points_->size());

			for (size_t e = 0; e < elements_->size(); ++e) {
				for (size_t n = 0; n < (*elements_)[e].size(); ++n) {
					node_elements_[(*elements_)[e][n]].emplace_back(e);
				}
			}
		}

		void ParallelNodeElementMap() {
			// With a concurrent container/collection, the node to element mapping can be done in parallel,
			node_elements_.resize(points_->size());

			for (size_t e = 0; e < elements_->size(); ++e) {
				for (size_t n = 0; n < (*elements_)[e].size(); ++n) {
					node_elements_[(*elements_)[e][n]].emplace_back(e);
				}
			}
		}
		
		/*
		* @brief Inserts a point starting from the given Octant
		* @param octant: the Octant to insert into from. The root at the start, and in recursive cases, any arbitrary Octant.
		* @param point_id: the point index to insert into the correct Octant from points_
		* @param size_t depth: the current depth of the given Octant
		*/
		void Insert(Octant& octant, size_t point_id, size_t depth) {
			if (octant.IsLeaf()) {
				// We examine our splitting criterion and accordingly either allow the insertion or split first
				if (octant.points.size() < leaf_split_threshold_ || depth == max_depth_) { 
					// Prohibit split case
					octant.points.emplace_back(point_id);
					return;
				} else {
					return Split(octant, point_id, depth);
				}
			}
			// Branch node/interior node case
			uint_fast8_t oct = octant.DetermineChildOctant((*points_)[point_id]);
			return Insert(*(octant.child[oct]), point_id, ++depth);
		}

		/*
		 * @brief Splits a leaf node into 8 new nodes and re-inserts the old point indexes into the newly created leaf octants
		 * @param octant: the leaf Octant that will be split and as a result turn into a branch Octant
		 * @param point_id: the point ID of points_ to be inserted straight from this Octant
		 * @param depth: the depth of the Octant being split
		 */
		void Split(Octant& octant, size_t point_id, size_t depth) {
			std::vector<size_t> current_points = std::move(octant.points);

			for (uint_fast8_t i = 0; i < 8; i++) {
				Vector3 split_center = octant.center;
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
		 * @brief Clears the octree and deallocates all memory
		 *
		 * The approach used is to use post-order traversal to clear the tree and its Octants, instead of using
		 * a chain of destructors calling each child octant with `delete` recursively
		 */
		void Clear() {
			post_order_iterator it = post_begin();
			while (it != post_end()) {
				auto octant = it.operator->();
				delete octant;
				++it;
			}
		}

		// ====================================================================
		// Searches (pick, NPM, FOP, DMUE, MUESF)
		// ====================================================================

		/*
		 * @brief Recurisvely returns the Octant containing the given point
		 */
		const Octant& OctantContainingPoint(const Vector3& point, const Octant& octant) const {
			if (octant.IsLeaf()) {
				return octant;
			} else {
				return OctantContainingPoint(point, *octant.child[octant.DetermineChildOctant(point)]);
			}
		}

		/*
		 * @brief Recursively attempts to find a given exact point
		 * @param octant: starting Octant to find the point from - usually the root.
		 * @returns -1 if the point cannot be found, or the index within points_ if the point was found.
		 */
		int_fast64_t Find(const Vector3& point, const Octant& octant) const {
			const Octant& o = OctantContainingPoint(point, octant);
			if (!o.points.empty()) {
				for (auto& p : o.points) {
					if (point == (*points_)[p]) {
						return p;
					}
				}
			} else {
				// FEDES_TRACE("[Octree Find]: Traversed to Octant not containing target point, Octant min: {} and max {}", o.aabb_min, o.aabb_max);
				return -1;
			}
			// FEDES_TRACE("[Octree Find]: Traversed to empty Octant, Octant min: {} and max {}", o.aabb_min, o.aabb_max);
			return -1;
		}

		size_t NearestNeighbour(const Vector3& query_point) const {
			size_t best{};
			PointT best_distance_sq = std::numeric_limits<PointT>::max();
			std::priority_queue<Octant*, std::vector<Octant*>, OctantComparator<PointT>> pq(query_point);
			pq.push(root_);

			while (!pq.empty() && best_distance_sq > pq.top()->MinimumDistanceSq(query_point)) {
				Octant* node = pq.top();
				pq.pop();

				if (node->IsLeaf()) {
					for (const auto& p : node->points) {
						PointT dist_sq = fedes::DistanceSquared(query_point, (*points_)[p]);
						if (dist_sq < best_distance_sq) {
							best = p;
							best_distance_sq = dist_sq;
							// FEDES_DEBUG("[PQ] Query point {}, current nearest {}", query_point, (*points_)[p]);
						}
					}
				} else { // Enqueue interior nodes
					for (size_t i = 0; i < 8; ++i) {
						pq.push(node->child[i]);
					}
				}
			}

			return best;
		}


		size_t DMUESearch(const Vector3& query_point, size_t scan_number) const {
			size_t best_element = 0;
			PointT best_dist_sq = std::numeric_limits<PointT>::max();
			size_t scanned = 0;
			std::unordered_set<size_t> considered_elements;
			std::priority_queue<Octant*, std::vector<Octant*>, OctantComparator<PointT>> pq(query_point);
			pq.push(root_);

			while (!pq.empty() && scanned < scan_number) {
				Octant* node = pq.top();
				pq.pop();

				if (node->IsLeaf()) {
					for (const auto& p : node->points) {
						for (const auto& e : node_elements_[p]) {
							if (considered_elements.contains(e)) {
								continue;
							} else {
								considered_elements.insert(e);
								scanned++;
								PointT dist_sq = fedes::AverageDistanceSq((*elements_)[e], *points_, query_point);
								if (dist_sq < best_dist_sq) {
									best_dist_sq = dist_sq;
									best_element = e;
								}
							}
						}
					}
				} else { // Enqueue interior nodes
					for (size_t i = 0; i < 8; ++i) {
						pq.push(node->child[i]);
					}
				}
			}

			FEDES_INFO("Element {} selected -- average distance {} -- after checking against {} other elements", best_element, best_dist_sq, (scanned-1));
			return best_element;
		}


		/*
		 * @param max_leaf_scans: in case geometry differs, without this parameter, the whole tree would be unnecessarily searched before boundaries are relaxed. 
		 */
		std::pair<size_t, MuesfData> MUESFSearch(const Vector3& query_point, size_t max_leaf_scans = 1000) const {
			bool found = false;
			std::unordered_set<size_t> considered_elements;
			std::priority_queue<Octant*, std::vector<Octant*>, OctantComparator<PointT>> pq(query_point);
			size_t scanned_leaves = 0;
			pq.push(root_);

			// Tetrahedron
			double g3 = -0.01;
			double h3 = -0.01;
			double r3 = -0.01;
			double g_h_r3 = 1.01;

			// Wedge
			double g4 = -0.01;
			double h4 = -0.01;
			double r4 = -1.01;
			double gg4 = 1.01;
			double hh4 = 1.01;
			double rr4 = 1.01;

			// Hexahedron
			double g5 = -1.01;
			double h5 = -1.01;
			double r5 = -1.01;
			double gg5 = 1.01;
			double hh5 = 1.01;
			double rr5 = 1.01;

			std::optional<MuesfData> opt;

			while (!found) {

				if (pq.empty() || scanned_leaves > max_leaf_scans) { // || scanned_leaves > x || some real formula (?)
					FEDES_DEBUG("[MUSEF] Geometries are different. Node is not contained inside any examined element. Search must be relaxed.");
					pq.push(root_);
					considered_elements.clear();
					scanned_leaves = 0;

					// Tetrahedron
					g3 = g3 - 0.05;
					h3 = h3 - 0.05;
					r3 = r3 - 0.05;
					g_h_r3 = g_h_r3 + 0.05;

					// Wedge
					g4 = g4 - 0.05;
					h4 = h4 - 0.05;
					r4 = r4 - 0.05;
					gg4 = gg4 + 0.05;
					hh4 = hh4 + 0.05;
					rr4 = rr4 + 0.05;

					// Hexahedron
					g5 = g5 - 0.05;
					h5 = h5 - 0.05;
					r5 = r5 - 0.05;
					gg5 = gg5 + 0.05;
					hh5 = hh5 + 0.05;
					rr5 = rr5 + 0.05;
				}

				Octant* node = pq.top();
				pq.pop();

				if (node->IsLeaf()) {
					scanned_leaves++;
					for (const auto& p : node->points) {
						for (const auto& e : node_elements_[p]) {
							if (considered_elements.contains(e)) {
								continue;
							} else { 
								considered_elements.insert(e);
								switch (element_type_) {
								case ElementType::Tetrahedron:
									 opt = TetrahedronContainment(*points_, (*elements_)[e],
										query_point, g3, h3, r3, g_h_r3);
									if (opt.has_value()) {
										return { e, *opt };
									}
									break;
								case ElementType::Wedge:
									opt = WedgeContainment(*points_, (*elements_)[e],
										query_point, g4, h4, r4, gg4, hh4, rr4);
									if (opt.has_value()) {
										return { e, *opt };
									}
									break;
								case ElementType::Hexahedron:
									opt = HexahedronContainment(*points_, (*elements_)[e],
										query_point, g5, h5, r5, gg5, hh5, rr5);
									if (opt.has_value()) {
										return { e, *opt };
									}
									break;
								}

							}
						}
					}
				} else { // Enqueue interior nodes
					for (size_t i = 0; i < 8; ++i) {
						pq.push(node->child[i]);
					}
				}
			}

		}

		// @return: Point ID and Euclidean Distance to query point for each quadrant
		[[nodiscard]] std::array<std::pair<size_t, PointT>, 8> FieldSearch(const Vector3& query, const Octant& o, const PointT max_radius) const {
			std::array<std::pair<size_t, PointT>, 8> field;

			std::vector<size_t> radius_points;
			PointT radius_sq = max_radius * max_radius;
			RadiusSearch(*root_, query, max_radius, radius_sq, radius_points);

			for (const auto& p : radius_points) {
				uint_fast8_t dir = fedes::DetermineDirection(query, (*points_)[p]);
				if (field[dir] == std::pair<size_t, PointT>()) {
					field[dir] = std::make_pair(p, fedes::Distance(query, (*points_)[p]));
				}
				else {
					PointT dist = fedes::Distance(query, (*points_)[p]);
					if (dist < field[dir].second) {
						field[dir] = std::make_pair(p, dist);
					}
				}
			}

			return field;
		}
		
		void RadiusSearch(const Octant& octant, const Vector3& query_point, PointT radius, PointT radius_sq, std::vector<size_t>& results) const {
			if (octant.IsLeaf()) {
				for (const auto& p : octant.points) {
				// @Note: _Octants_ in the given radius versus _points_ in the given radius is separatable/distinguishable
				/*
					if (std::abs(fedes::DistanceSquared(query_point, (*points_)[p])) < radius_sq) {
						results.emplace_back(p);
					}
				*/
					results.emplace_back(p);
				}
			} else {
				for (uint_fast8_t i = 0; i < 8; ++i) {
					if (octant.child[i]->WithinSphere(query_point, radius, radius_sq)) {
						RadiusSearch(*octant.child[i], query_point, radius, radius_sq, results);
					}
				}
			}

		}

	};
}