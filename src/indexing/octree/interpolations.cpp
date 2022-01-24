#include "fedes/indexing/octree/interpolations.hpp"

#include "fedes/maths/vector3.hpp"
#include "fedes/indexing/octree/octree.hpp"
#include "fedes/indexing/octree/octant.hpp"
#include "fedes/model/model.hpp"

#include <iostream>

namespace fedes {

	/*
	 * @brief Performs nearest point method using an Octree
	 * 
	 * @tparam T: the point data type of the Octree, i.e. double, float
	 * @param octree: the built Octree Index to use when performing the mapping
	 * @param source: the source model with nodal and FE data set to be copied
	 * @param target: the target model to use to fill stresses, displacements etc data with from the given index
	 */
	template <typename T>
	void OctreeNearestPointMethod(const fedes::Octree<T>& octree, const fedes::Model& source, fedes::Model& target) {
		target.ResizeIndexes(source);
		if (!source.displacement.empty()) {
			for (size_t i = 0; i != target.nodes.size(); i++) {
				size_t source_node_idx = octree.Nearest(target.nodes[i]);
				target.displacement[i] = source.displacement[source_node_idx];
			}
		}
		if (!source.stress.empty()) { 
			for (size_t i = 0; i != target.integration.size(); i++) {
				size_t source_node_idx = octree.Nearest(target.integration[i]);
				target.stress[i] = source.stress[source_node_idx];
			}
		}
	}

	/* Explicit template instantiations */
	template void OctreeNearestPointMethod<double>(const fedes::Octree<double>&, const fedes::Model&, fedes::Model&);
}
