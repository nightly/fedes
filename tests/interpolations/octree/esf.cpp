#include <gtest/gtest.h>
#include "fedes/interpolations/octree/octree.h"

#include <memory>

#include <BS_thread_pool.hpp>

#include "fedes/model/model.h"
#include "fedes/model/writers.h"
#include "fedes/model/parsers.h"
#include "fedes/model/examples.h"
#include "fedes/indexing/octree/octree.h"


TEST(OctreeInterpolationsESF, DISABLED_Model1) {
	BS::thread_pool pool;
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 1);
	fedes::Octree<double> octree(source.nodes, source.elements, 8, 10);
	fedes::ParallelElementShapeFunction<double>(octree, source, target, pool);
}
