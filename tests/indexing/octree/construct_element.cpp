#include <gtest/gtest.h>
#include "fedes/indexing/octree/octree.h"

#include <memory>

#include <thread_pool.hpp>

#include "fedes/model/model.h"
#include "fedes/model/examples.h"
#include "fedes/model/parsers.h"
#include "fedes/maths/element_type.h"

TEST(OctreeConstructElement, ElementType) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 4);
	fedes::Octree<double> octree(source.nodes, source.elements, 10, 8);
	ASSERT_EQ(fedes::ElementType::Tetrahedron, octree.element_type());
}

TEST(OctreeConstructElement, NodeElementMap) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 1);
	fedes::Octree<double> octree(source.nodes, source.elements, 10, 8);

	std::vector<size_t> expected{ 94472, 94482, 101446, 126937, 130220 };
	ASSERT_EQ(octree.node_elements()[1], expected);
}


TEST(OctreeConstructElement, ParallelElementMap) {
	fedes::Model source, target;
	thread_pool pool;
	fedes::SetExampleModels(source, target, 1);

	fedes::Octree<double> octree(source.nodes, source.elements, 10, 8);
	fedes::Octree<double> par_octree(source.nodes, source.elements, 10, 8, &pool);

	const std::vector<std::vector<size_t>>& par_elems = par_octree.node_elements();
	const std::vector<std::vector<size_t>>& elems = octree.node_elements();

	// Sort them as the order doesn't actually matter here, but does for operator==
	std::vector<std::vector<size_t>> par_elems_sorted = par_elems;
	std::vector<std::vector<size_t>> elems_sorted = elems;
	for (size_t i = 0; i < elems_sorted.size(); ++i) {
		std::sort(elems_sorted[i].begin(), elems_sorted[i].end());
		std::sort(par_elems_sorted[i].begin(), par_elems_sorted[i].end());
	}

	ASSERT_EQ(par_elems_sorted, elems_sorted);
}