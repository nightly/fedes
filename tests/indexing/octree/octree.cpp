#include "fedes/indexing/octree/octree.hpp"
#include <gtest/gtest.h>

#include <memory>
#include <array>
#include <iostream>

#include "fedes/maths/vector3.hpp"


class OctreeTest : public ::testing::Test {
protected:
	std::unique_ptr<fedes::Octree<double>> octree_;
	std::vector<fedes::Vector3<double>> points;

	void SetUp() override {
		points.emplace_back(-1.0, -1.0, -1.0);
		points.emplace_back(0.0, 0.0, 0.0);
		points.emplace_back(0.1, 0.1, 0.1);
		points.emplace_back(0.5, 0.25, 0.75);
		points.emplace_back(0.95, 0.95, 0.95);
		points.emplace_back(0.99, 0.99, 0.99);
		points.emplace_back(1.0, 1.0, 1.0);
		
		octree_ = std::make_unique<fedes::Octree<double>>(points, 1, 10);
	};
};

TEST_F(OctreeTest, Find) {
	ASSERT_EQ(octree_->Find(fedes::Vector3<double>(0.3, 0.3, 0.3)), false);
	ASSERT_EQ(octree_->Find(fedes::Vector3<double>(0.5, 0.25, 0.75)), true);
}

TEST_F(OctreeTest, Nearest) {
	fedes::Vector3<double> p1(0.94, 0.93, 0.9);
	ASSERT_EQ(octree_->Nearest(p1), 4);
	fedes::Vector3<double> p2(-0.9, -0.82, -0.5);
	ASSERT_EQ(octree_->Nearest(p2), 0);
}