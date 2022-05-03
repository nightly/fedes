#include <gtest/gtest.h>
#include "fedes/interpolations/octree/octree.h"

#include <memory>

#include "fedes/model/model.h"
#include "fedes/model/writers.h"
#include "fedes/model/parsers.h"
#include "fedes/model/examples.h"
#include "fedes/indexing/octree/octree.h"


TEST(OctreeInterpolationsDMUE, DISABLED_Model1) {
	thread_pool pool;
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 1);
	fedes::Octree<double> octree(source.nodes, source.elements, 10, 8);
	fedes::ParallelDMUE(octree, source, target, pool);
}
