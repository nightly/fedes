#include <gtest/gtest.h>
#include "fedes/indexing/octree/octree.h"

#include <memory>
#include <array>
#include <iostream>

#include "fedes/maths/vector3.h"


class OctreeNearestSearchTest : public ::testing::Test {
protected:
	std::unique_ptr<fedes::Octree<double>> octree_;
	std::vector<fedes::Vector3<double>> points_;

	void SetUp() override {
		points_.emplace_back(-1.0, -1.0, -1.0);
		points_.emplace_back(0.0, 0.0, 0.0);
		points_.emplace_back(0.1, 0.1, 0.1);
		points_.emplace_back(0.5, 0.25, 0.75);
		points_.emplace_back(0.95, 0.95, 0.95);
		points_.emplace_back(0.99, 0.99, 0.99);
		points_.emplace_back(1.0, 1.0, 1.0);

		octree_ = std::make_unique<fedes::Octree<double>>(points_, 10, 1);
	};
};

TEST_F(OctreeNearestSearchTest, DISABLED_Nearest) {
	fedes::Vector3<double> p1(0.94, 0.93, 0.9);
	size_t n1 = octree_->Nearest(p1);
	ASSERT_EQ(n1, 4);
	fedes::Vector3<double> p2(-0.9, -0.82, -0.5);
	size_t n2 = octree_->Nearest(p2);
	ASSERT_EQ(n2, 0);
}