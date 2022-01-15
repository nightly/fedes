#include "octree.hpp"

#include <iostream>

#include "fedes/indexing/octree/octree.hpp"
#include "fedes/indexing/octree/interpolations.hpp"
#include "fedes/instrumentation/timer.hpp"


/*
 * @brief Nearest Point Method with Octree Index with timing
 */
void OctreeNPM(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth) {
	fedes::internal::Timer build_timer("Octree Build");
	fedes::Octree<double> octree(source.nodes, points_per_leaf, max_depth);
	build_timer.StopWithWrite();

	fedes::internal::Timer interpolation_timer("Octree NPM Interpolation");
	fedes::OctreeNearestPointMethod(octree, source, target);
	interpolation_timer.StopWithWrite();
}


/*
 * @brief Distance Method using Field of Points with Octree Index with timing
 */
void OctreeDMUFOP(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth) {
	fedes::internal::Timer timer(__FUNCSIG__);
}

/*
 * @brief Distance Method using Elements with Octree Index with timing
 */
void OctreeDMUE(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth) {
	fedes::internal::Timer timer(__FUNCSIG__);
}

/*
 * @brief Element Shape Function with Octree Index with timing
 */
void OctreeESF(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth) {
	fedes::internal::Timer timer(__FUNCSIG__);
}
