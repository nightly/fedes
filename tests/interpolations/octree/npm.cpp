#include <gtest/gtest.h>
#include "fedes/interpolations/octree/octree.h"

#include <memory>

#include "fedes/model/model.h"
#include "fedes/model/writers.h"
#include "fedes/model/parsers.h"
#include "fedes/model/examples.h"
#include "fedes/indexing/octree/octree.h"


TEST(OctreeInterpolationsNPM, DISABLED_Model1) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 1);
	fedes::Octree<double> octree(source.nodes, 10, 8);
	fedes::NearestPointMethod<double>(octree, source, target);

	ASSERT_EQ(target.displacement[2], source.displacement[92]);
	ASSERT_EQ(target.displacement[14], source.displacement[2015]);
	ASSERT_EQ(target.displacement[25000], std::vector<double>({ 0.059610000000000003 , -0.028618000000000001,  0.44091999999999998 }));
	ASSERT_EQ(target.stress[100000], std::vector<double>({ 144810000.00000000, 429390000.00000000, 77178000.000000000, -78032000.000000000, -89415000.000000000, -36849000.000000000 }));
}