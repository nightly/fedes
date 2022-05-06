#include "octree.h"

#include <iostream>

#include <thread_pool.hpp>

#include "fedes/indexing/octree/octree.h"
#include "fedes/interpolations/octree/octree.h"
#include "fedes/instrumentation/timer.h"

/*
 * @brief Nearest Point Method with Octree Index with timing
 */
void OctreeNPM(fedes::Model& source, fedes::Model& target, size_t max_depth, size_t points_per_leaf,
	          thread_pool& pool) {
	fedes::internal::Timer build_timer("Octree Node Index Construction");
	fedes::Octree<double> octree (source.nodes, max_depth, points_per_leaf, &pool);
	auto build_duration = build_timer.Stop();

	fedes::internal::Timer interpolation_timer("Octree NPM Interpolation");
	// fedes::NearestPointMethod(octree, source, target);
	fedes::ParallelNearestPointMethod(octree, source, target, pool);
	auto interpolation_duration = interpolation_timer.Stop();
	build_timer.WriteDuration(build_duration);
	interpolation_timer.WriteDuration(interpolation_duration);
}


/*
 * @brief Distance Method using Field of Points with Octree Index with timing
 */
void OctreeDMUFOP(fedes::Model& source, fedes::Model& target, size_t max_depth, size_t points_per_leaf,
	             double radius, thread_pool& pool) {
	fedes::internal::Timer build_timer("Octree Node Index Construction");
	fedes::Octree<double> octree(source.nodes, max_depth, points_per_leaf, &pool);
	auto build_duration = build_timer.Stop();

	fedes::internal::Timer interpolation_timer("Octree Distance Method using Field of Points Interpolation");
	fedes::ParallelFieldOfPoints(octree, source, target, pool, radius);
	auto interpolation_duration = interpolation_timer.Stop();
	build_timer.WriteDuration(build_duration);
	interpolation_timer.WriteDuration(interpolation_duration);
}

/*
 * @brief Distance Method using Elements with Octree Index with timing
 */
void OctreeDMUE(fedes::Model& source, fedes::Model& target, size_t max_depth, size_t points_per_leaf,
	           size_t min_scan_dmue, thread_pool& pool) {
	fedes::internal::Timer build_timer("Octree Element Index Construction");
	fedes::Octree<double> octree(source.nodes, source.elements, max_depth, points_per_leaf, &pool);
	auto build_duration = build_timer.Stop();

	fedes::internal::Timer interpolation_timer("Octree Distance Method using Elements Interpolation");
	fedes::ParallelDMUE(octree, source, target, pool, min_scan_dmue);
	auto interpolation_duration = interpolation_timer.Stop();
	build_timer.WriteDuration(build_duration);
	interpolation_timer.WriteDuration(interpolation_duration);
}

/*
 * @brief Element Shape Function with Octree Index with timing
 */
void OctreeESF(fedes::Model& source, fedes::Model& target, size_t max_depth, size_t points_per_leaf,
	          size_t max_leaf_scans_threshold, thread_pool& pool) {
	fedes::internal::Timer build_timer("Octree Element Index Construction");
	fedes::Octree<double> octree(source.nodes, source.elements, max_depth, points_per_leaf, &pool);
	auto build_duration = build_timer.Stop();

	fedes::internal::Timer interpolation_timer("Octree Element Shape Function Interpolation");
	fedes::ParallelElementShapeFunction(octree, source, target, pool, max_leaf_scans_threshold);
	auto interpolation_duration = interpolation_timer.Stop();
	build_timer.WriteDuration(build_duration);
	interpolation_timer.WriteDuration(interpolation_duration);
}
