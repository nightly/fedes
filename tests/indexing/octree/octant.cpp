#include "fedes/indexing/octree/octant.hpp"

#include <memory>

#include "fedes/maths/vector3.hpp"

#include <gtest/gtest.h>

class OctantTest : public ::testing::Test {
protected:
	std::unique_ptr<fedes::internal::Octant<double>> octant_;

	void SetUp() override {
		fedes::Vector3<double> center(0.0, 0.0, 0.0);
		fedes::Vector3<double> extent(1.0, 1.0, 1.0);
		octant_ = std::make_unique<fedes::internal::Octant<double>>(center, extent);
	}
};


TEST_F(OctantTest, DetermineChildOctant) {
	fedes::Vector3<double> insertion_point(0.8, 0.7, 0.8);
	int child = octant_->DetermineChildOctant(insertion_point);
	ASSERT_EQ(child, 7) << "Expected point 1 to be placed at index 7";

	fedes::Vector3<double> second_insertion_point(0.5, -0.1, 0.9);
	int second_child = octant_->DetermineChildOctant(second_insertion_point);
	ASSERT_EQ(second_child, 5) << "Expected point 2 to be placed at index 5";
}


TEST_F(OctantTest, IsLeafFromFixture) {
	bool leaf = octant_->IsLeaf();
	ASSERT_EQ(leaf, true) << "Expected original fixture to be a leaf node";
}