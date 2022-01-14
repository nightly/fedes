#include <gtest/gtest.h>
#include "fedes/indexing/octree/traversals.hpp"

#include <vector>

#include "fedes/indexing/octree/octree.hpp"
#include "fedes/indexing/octree/octant.hpp"


class OctreeTraversalTest : public ::testing::Test {
protected:
	std::unique_ptr<fedes::Octree<double>> octree_;

	void SetUp() override {
		std::vector<fedes::Vector3<double>> points = { fedes::Vector3<double>(-1.0, -1.0, -1.0), fedes::Vector3<double>(0.0, 0.0, 0.0),
		fedes::Vector3<double>(0.5, 0.25, 0.75), fedes::Vector3<double>(1.0, 1.0, 1.0) };
		octree_ = std::make_unique<fedes::Octree<double>>(points, 8, 10);
	};
};

TEST_F(OctreeTraversalTest, OctreePostOrderIterator) {
	std::vector<fedes::internal::Octant<double>*> got;
	fedes::Octree<double>::post_order_iterator it = octree_->post_begin();
	while (it != octree_->post_end()) {
		auto octant = it.operator->();
		if (!octant->IsEmpty() || !octant->IsLeaf()) {
			got.push_back(octant);
		}
		++it;
	}
	std::vector<fedes::internal::Octant<double>*> expected;
	// @Todo 
	// ASSERT_EQ(got, expected);
}