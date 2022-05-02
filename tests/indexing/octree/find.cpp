#include <gtest/gtest.h>
#include "fedes/indexing/octree/octree.h"

#include <memory>
#include <array>

#include "fedes/maths/vector3.h"


class OctreeFindTest : public ::testing::Test {
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

		octree_ = std::make_unique<fedes::Octree<double>>(points, 10, 0);
	};
};

TEST_F(OctreeFindTest, Find) {
	ASSERT_EQ(octree_->Find(fedes::Vector3<double>(0.5, 0.25, 0.75)), 3);
}

TEST_F(OctreeFindTest, Missing) {
	ASSERT_EQ(octree_->Find(fedes::Vector3<double>(0.3, 0.3, 0.3)), -1);
}