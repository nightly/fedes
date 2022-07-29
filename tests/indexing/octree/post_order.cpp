#include <gtest/gtest.h>
#include "fedes/indexing/octree/traversals.h"

#include "fedes/indexing/octree/octree.h"
#include "fedes/indexing/octree/octant.h"
#include "fedes/model/examples.h"

/*
 * @brief Ensure that all nodes are visited during the traversal.
 */
TEST(OctreePostOrder, FullTraversal) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 2);
	fedes::Octree<double> octree(source.nodes, 10, 8);
	size_t count = 0;

	fedes::Octree<double>::post_order_iterator it = octree.post_begin();
	while (it != octree.post_end()) {
		fedes::internal::Octant<double>* octant = it.operator->();
		++count;
		++it;
	}
	ASSERT_EQ(count, 29841);
}

/*
 * @brief Assert that the last node returned during post-order traversal is the root, since its the first added.
 */
TEST(OctreePostOrder, RootLast) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 2);
	fedes::Octree<double> octree(source.nodes, 10, 8);
	size_t count{ 0 };

	fedes::Octree<double>::post_order_iterator it = octree.post_begin();
	while (it != octree.post_end()) {
		if (count == 29840) {
			fedes::internal::Octant<double>* octant = it.operator->();
			const auto& [center, extent] = octree.root();
			ASSERT_EQ(octant->center, center);
			ASSERT_EQ(octant->extent, extent);
		}
		++count;
		++it;
	}
}