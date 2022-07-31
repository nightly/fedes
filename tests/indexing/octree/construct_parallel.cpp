#include <gtest/gtest.h>
#include "fedes/indexing/octree/octree.h"

#include <memory>

#include <BS_thread_pool.hpp>

#include "fedes/model/model.h"
#include "fedes/model/examples.h"
#include "fedes/model/parsers.h"

TEST(OctreeConstructParallel, Root_Model1) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 1);
	BS::thread_pool pool;

	fedes::Octree<double> octree(source.nodes, source.elements, 20, 20);
	const auto& [center, extent] = octree.root();

	fedes::Octree<double> octreeP(source.nodes, 10, 8, &pool);
	const auto& [centerP, extentP] = octreeP.root();

	ASSERT_EQ(center, centerP);
	ASSERT_EQ(extent, extentP);
}

TEST(OctreeConstructParallel, Root_Model2) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 2);
	BS::thread_pool pool;

	fedes::Octree<double> octree(source.nodes, source.elements, 15, 0);
	const auto& [center, extent] = octree.root();

	fedes::Octree<double> octreeP(source.nodes, 10, 8, &pool);
	const auto& [centerP, extentP] = octreeP.root();

	ASSERT_EQ(center, centerP);
	ASSERT_EQ(extent, extentP);
}

TEST(OctreeConstructParallel, Root_Model3) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 3);
	BS::thread_pool pool;

	fedes::Octree<double> octree(source.nodes, 5, 0);
	const auto& [center, extent] = octree.root();

	fedes::Octree<double> octreeP(source.nodes, 5, 0, &pool);
	const auto& [centerP, extentP] = octreeP.root();

	ASSERT_EQ(center, centerP);
	ASSERT_EQ(extent, extentP);
}

TEST(OctreeConstructParallel, Root_Model4) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 4);
	BS::thread_pool pool;

	fedes::Octree<double> octree(source.nodes, 15, 15);
	const auto& [center, extent] = octree.root();

	fedes::Octree<double> octreeP(source.nodes, 15, 5, &pool);
	const auto& [centerP, extentP] = octreeP.root();

	ASSERT_EQ(center, centerP);
	ASSERT_EQ(extent, extentP);
}

