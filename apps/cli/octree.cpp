#include "octree.h"

#include <iostream>

#include "fedes/indexing/mx-octree/octree.h"
#include "fedes/interpolations/octree.h"
#include "fedes/instrumentation/timer.h"


/*
* @brief Octree Build with Timing
*/
fedes::Octree<double> OctreeBuild(fedes::Model& source, size_t points_per_leaf, size_t max_depth) {
	fedes::internal::Timer build_timer("Octree Build");
	return fedes::Octree<double>(source.nodes, points_per_leaf, max_depth);
}

/*
 * @brief Nearest Point Method with Octree Index with timing
 */
void OctreeNPM(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth) {
	fedes::Octree<double> octree = OctreeBuild(source, points_per_leaf, max_depth);
	fedes::internal::Timer interpolation_timer("Octree NPM Interpolation");
	fedes::OctreeNearestPointMethod(octree, source, target);
}


/*
 * @brief Distance Method using Field of Points with Octree Index with timing
 */
void OctreeDMUFOP(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth) {
	fedes::Octree<double> octree = OctreeBuild(source, points_per_leaf, max_depth);
	fedes::internal::Timer interpolation_timer("Octree Distance Method using Field of Points Interpolation");
	fedes::OctreeFieldOfPoints(octree, source, target);
}

/*
 * @brief Distance Method using Elements with Octree Index with timing
 */
void OctreeDMUE(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth) {
	fedes::Octree<double> octree = OctreeBuild(source, points_per_leaf, max_depth);
	fedes::internal::Timer timer(__FUNCSIG__);
}

/*
 * @brief Element Shape Function with Octree Index with timing
 */
void OctreeESF(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth) {
	fedes::Octree<double> octree = OctreeBuild(source, points_per_leaf, max_depth);
	fedes::internal::Timer timer(__FUNCSIG__);
}
