#include "fedes/indexing/octree/interpolations.hpp"

#include "fedes/indexing/octree/octree.hpp"
#include "fedes/indexing/octree/octant.hpp"
#include "fedes/model/model.hpp"

namespace fedes {

	/*
	 * @brief Performs nearest point method using an Octree
	 * 
	 * @tparam T: the point data type of the Octree, i.e. double, float
	 * @param octree: the built Octree Index to use when performing the mapping
	 * @param source_model: the source model with nodal and FE data set to be copied
	 * @param target_model: the target model to use to fill stresses, displacements etc data with from the given index
	 */
	template <typename T>
	void OctreeNearestPointMethod(const fedes::Octree<T>& octree, const fedes::Model& source_model, fedes::Model& target_model) {
		for (size_t i = 0; i != target_model.nodes.size(); i++) {
			size_t source_node_index = octree.Nearest(target_model.nodes[i]);
			target_model.NearestPointTransfer(i, source_model, source_node_index);
		}
	}

	/* Explicit template instantiations */
	template void OctreeNearestPointMethod<double>(const fedes::Octree<double>&, const fedes::Model&, fedes::Model&);
}