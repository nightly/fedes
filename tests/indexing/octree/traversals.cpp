#include <gtest/gtest.h>
#include "fedes/indexing/octree/traversals.h"

#include <vector>

#include "fedes/indexing/octree/octree.h"
#include "fedes/indexing/octree/octant.h"

class OctreeTraversalTest : public ::testing::Test {
protected:
	std::unique_ptr<fedes::Octree<double>> octree_;

	void SetUp() override {
		std::vector<fedes::Vector3<double>> points = { fedes::Vector3<double>(-1.0, -1.0, -1.0), fedes::Vector3<double>(0.0, 0.0, 0.0),
		fedes::Vector3<double>(0.5, 0.25, 0.75), fedes::Vector3<double>(1.0, 1.0, 1.0) };
		octree_ = std::make_unique<fedes::Octree<double>>(points, 10, 0);
	};
};

TEST_F(OctreeTraversalTest, PostOrderIterator) {
	std::vector<fedes::internal::Octant<double>*> got;
	fedes::Octree<double>::post_order_iterator it = octree_->post_begin();
	while (it != octree_->post_end()) {
		fedes::internal::Octant<double>* octant = it.operator->();
		got.push_back(octant);
		++it;
	}
	std::vector<fedes::internal::Octant<double>*> expected;
	// ASSERT_EQ(got, expected); @Todo: add assertion
}