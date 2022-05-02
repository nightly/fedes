#include <gtest/gtest.h>
#include "fedes/indexing/octree/traversals.h"

#include "fedes/indexing/octree/octree.h"
#include "fedes/indexing/octree/octant.h"
#include "fedes/model/examples.h"

/*
 * @brief Ensure that all nodes are visited during the traversal.
 */
TEST(MxOctreePostOrder, DISABLED_FullTraversal) {
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
	ASSERT_EQ(count, 28985);
}

/*
 * @brief Assert that all leaves are returned before any of the branches/interior nodes. 
 */
TEST(MxOctreePostOrder, DISABLED_AllLeavesFirst) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 2);
	fedes::Octree<double> octree(source.nodes, 10, 8);
	bool encountered_first_non_leaf = false;

	fedes::Octree<double>::post_order_iterator it = octree.post_begin();
	while (it != octree.post_end()) {
		fedes::internal::Octant<double>* octant = it.operator->();
		if (octant->IsLeaf()) {
			if (encountered_first_non_leaf) {
				ADD_FAILURE() << "Previously encountered a non-leaf during post-order traversal, but found a leaf afterwards";
			}
		} else {
			encountered_first_non_leaf = true;
		}
		delete octant;
		++it;
	}
}


/*
 * @brief Assert that the last node returned during post-order traversal is the root. 
 */
TEST(MxOctreePostOrder, DISABLED_RootLast) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 2);
	fedes::Octree<double> octree(source.nodes, 10, 8);
	bool encountered_first_non_leaf = false;

	fedes::Octree<double>::post_order_iterator it = octree.post_begin();
	while (it != octree.post_end()) {
		fedes::internal::Octant<double>* octant = it.operator->();
		if (++it == octree.post_end()) {
			// ASSERT_EQ(it.operator->(), octant.root);
		}
		++it;
	}
}