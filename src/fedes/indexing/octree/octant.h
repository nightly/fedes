#pragma once 

#include <array>
#include <vector>
#include <concepts>
#include <algorithm>

#include "fedes/maths/vector3.h"
#include "fedes/maths/z_ordering.h"
#include "fedes/maths/distance.h"

namespace fedes::internal {
	template <std::floating_point PointT>
	class Octant {
	public:
		using value_type = PointT;
		using Vector3 = Vector3<PointT>;
	public:
		Vector3 center;
		Vector3 extent; // @brief: half how far the Octant extends from the center (+=, -=) (width, height, depth) [half_size]
		Vector3 aabb_min;
		Vector3 aabb_max;
		std::vector<size_t> points;
		std::array<Octant<PointT>*, 8> child = { nullptr };
	public:
		Octant() noexcept = default;
		Octant(const Vector3& center, const Vector3& extent)
		: center(center), extent(extent) {
			aabb_min = center - extent;
			aabb_max = center + extent;
		}
		~Octant() noexcept = default;

		Octant(const Octant& other) = delete;
		Octant& operator=(const Octant& other) = delete;
		Octant(Octant&& other) = delete;
		Octant& operator=(Octant&& other) = delete;

		/*
		 * @brief Returns true/false if the Octant is a leaf, using the invariant that there are either 0 or 8 children.
		 */
		bool IsLeaf() const {
			return child[0] == nullptr;
		}

		bool IsEmpty() const {
			return points.empty();
		}

		/*
		 * @brief Provided a point, returns which child Octant the point belongs to using Morton Encoding/Z-Ordering
		 *
		 *		3------7.
		 *		|`.    | `.
		 *		|  `2--+---6
		 *		|   |  |   |
		 *		1---+--5.  |
		 *		 `. |    `.|
		 *		   `0------4
		 *
		 * @param point: the point in question to determine the correct sub-Octant for based on the current Octant.
		 * @return unsigned integer from 0 to 7 representing the child octant within the child[] array
		 */
		[[nodiscard]] uint_fast8_t DetermineChildOctant(const Vector3& point) const {
			return fedes::DetermineDirection(center, point);
		}
		
		/*
	     * @brief Calculates the minimum squared distance to a given point from this Octant
		 * Handles containment (return 0 case).
		 */
		PointT MinimumDistanceSq(const Vector3& point) const {
			const PointT& x1 = aabb_min.x, &x2 = aabb_max.x;
			const PointT& y1 = aabb_min.y, &y2 = aabb_max.y;
			const PointT& z1 = aabb_min.z, &z2 = aabb_max.z;
			constexpr PointT zero = 0.00;

			PointT dx{std::max({ (x1 - point.x), zero, (point.x - x2) })};
			PointT dy{std::max({ (y1 - point.y), zero, (point.y - y2) })};
			PointT dz{std::max({ (z1 - point.z), zero, (point.z - z2) })};
			return (dx*dx + dy*dy + dz*dz);
		}

		bool WithinSphere(const Vector3& point, PointT radius, PointT radius_sq) const {
			Vector3 abs_rel(std::abs(point.x - center.x), std::abs(point.y - center.y), std::abs(point.z - center.z));

			// Minkowski sum
			Vector3 max(radius + extent.x, radius + extent.y, radius + extent.z);
			if ((abs_rel.x > max.x) || (abs_rel.y > max.y) || (abs_rel.z > max.z)) {
				return false;
			}

			// Overlapping or Full Inside
			if ((abs_rel.x < extent.x) || (abs_rel.y < extent.y) || (abs_rel.z < extent.z)) {
				return true; 
			}

			// Corner case
			PointT dist_sq = fedes::DistanceSquared(extent, abs_rel);
			return dist_sq < radius_sq;
		}
	};
}