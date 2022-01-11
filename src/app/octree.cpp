#include "octree.hpp"

#include <iostream>

#include "fedes/indexing/octree/octree.hpp"
#include "fedes/instrumentation/timer.hpp"

/*
 * @brief Nearest Point Method with Octree Index with timing
 */
void OctreeNPM(fedes::Model& source, fedes::Model& target) {
	fedes::internal::Timer timer(__FUNCSIG__);
	fedes::Octree<double> octree(source.nodes, 8, 10);
}

/*
 * @brief Distance Method using Field of Points with Octree Index with timing
 */
void OctreeDMUFOP(fedes::Model& source, fedes::Model& target) {
	fedes::internal::Timer timer(__FUNCSIG__);
}

/*
 * @brief Distance Method using Elements with Octree Index with timing
 */
void OctreeDMUE(fedes::Model& source, fedes::Model& target) {
	fedes::internal::Timer timer(__FUNCSIG__);
}

/*
 * @brief Element Shape Function with Octree Index with timing
 */
void OctreeESF(fedes::Model& source, fedes::Model& target) {
	fedes::internal::Timer timer(__FUNCSIG__);
}
