#include <gtest/gtest.h>
#include "fedes/indexing/octree/octree.h"

#include <memory>

#include <thread_pool.hpp>

#include "fedes/model/model.h"
#include "fedes/model/examples.h"
#include "fedes/model/parsers.h"


TEST(OctreeConstruct, Root_Model1) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 1);
	fedes::Octree<double> octree(source.nodes, 10, 8);
	const auto& [center, extent] = octree.root();
	ASSERT_EQ(center, fedes::Vector3<double>(156.75000000000000, 0.0000000000000000, 654.94499999999994));
	ASSERT_EQ(extent, fedes::Vector3<double>(156.75000000000000, 196.93000000000001, 230.05500000000001));
}

TEST(OctreeConstruct, Root_Model1_Parallel) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 1);
	thread_pool pool;
	fedes::Octree<double> octree(source.nodes, 10, 8, &pool);
	const auto& [center, extent] = octree.root();
	ASSERT_EQ(center, fedes::Vector3<double>(156.75000000000000, 0.0000000000000000, 654.94499999999994));
	ASSERT_EQ(extent, fedes::Vector3<double>(156.75000000000000, 196.93000000000001, 230.05500000000001));
}

TEST(OctreeDestruct, Model1) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 1);
	{
		fedes::Octree<double> octree(source.nodes, 10, 8);
	}
	EXPECT_NO_THROW();
}