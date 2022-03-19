#include "fedes/indexing/mx-octree/octant.hpp"

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

TEST_F(OctantTest, IsLeafFromFixture) {
	bool leaf = octant_->IsLeaf();
	ASSERT_EQ(leaf, true) << "Expected original fixture to be a leaf node";
}

TEST_F(OctantTest, IsLeafWithChildren) {
	fedes::internal::Octant<double> child(fedes::Vector3<double>(0.5, 0.5, 0.5), fedes::Vector3<double>(0.5,0.5,0.5));
	for (int i = 0; i < 7; i++) {
		octant_->child[i] = &child;
	}
	bool leaf = octant_->IsLeaf();
	ASSERT_EQ(leaf, false) << "Expected fixture with children added to not be classified as a leaf";
}

TEST_F(OctantTest, IsEmpty) {
	ASSERT_EQ(octant_->IsEmpty(), true);
	octant_->points.emplace_back(1);
	ASSERT_EQ(octant_->IsEmpty(), false);
}

TEST_F(OctantTest, MinimumDistanceZero) {
	// Point is contained inside the Octant, thus minimum distance should be zero
	fedes::Vector3<double> p(0.5, 0.5, 0.5); 
	double minimum_distance = sqrt(octant_->MinimumDistanceSq(p));
	ASSERT_EQ(minimum_distance, 0);
}

TEST_F(OctantTest, MinimumDistance) {

}