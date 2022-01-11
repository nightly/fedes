#include "fedes/indexing/octree/octree.hpp"

#include <memory>
#include <array>

#include "fedes/maths/vector3.hpp"

#include <gtest/gtest.h>

class OctreeTest : public ::testing::Test {
protected:
	std::unique_ptr<fedes::Octree<double>> octree_;
	
	void SetUp() override {
		std::vector<fedes::Vector3<double>> points = { fedes::Vector3<double>(-1.0, -1.0, -1.0), fedes::Vector3<double>(0.0, 0.0, 0.0),
		fedes::Vector3<double>(0.5, 0.25, 0.75), fedes::Vector3<double>(1.0, 1.0, 1.0) };
		octree_ = std::make_unique<fedes::Octree<double>>(points, 8, 10);
	};
};

TEST_F(OctreeTest, Find) {
	ASSERT_EQ(octree_->Find(fedes::Vector3<double>(0.3, 0.3, 0.3)), false);
	ASSERT_EQ(octree_->Find(fedes::Vector3<double>(0.5, 0.25, 0.75)), true);
}