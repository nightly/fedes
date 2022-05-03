#include <gtest/gtest.h>
#include "fedes/indexing/octree/octree.h"

#include <memory>
#include <array>

#include "fedes/maths/vector3.h"


class OctreeRadiusTest : public ::testing::Test {
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

TEST_F(OctreeRadiusTest, R1) {
	std::vector<size_t> got;
	fedes::Vector3<double> center(0.11, 0.11, 0.11);
	double radius = 0.1;
	octree_->RadiusSearch(center, radius, got);

	std::vector<size_t> expected({2});
	ASSERT_EQ(got, expected);
}

TEST_F(OctreeRadiusTest, All) {
	std::vector<size_t> got;
	fedes::Vector3<double> center(0.25, 0.11, 0.3);
	double radius = 5;
	octree_->RadiusSearch(center, radius, got);

	std::vector<size_t> expected({0, 1, 2, 3, 4, 5, 6});
	ASSERT_EQ(got, expected);
}