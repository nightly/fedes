#include "fedes/indexing/mx-octree/interpolations.h"

#include "fedes/maths/vector3.h"
#include "fedes/indexing/mx-octree/octree.h"
#include "fedes/indexing/mx-octree/octant.h"
#include "fedes/model/model.h"

#include <iostream>

namespace fedes {

	/*
	 * @brief Performs nearest point method using an Octree
	 * @tparam T: point data type of the Octree, i.e. double, float
	 * @param octree: built Octree Index to use when performing the mapping
	 * @param source: source model with nodal and FE data
	 * @param target: target model to use to fill FE data with from the source with indexes preset using SetIndexes()
	 */
	template <typename T>
	void OctreeNearestPointMethod(const fedes::Octree<T>& octree, const fedes::Model& source, fedes::Model& target) {
		if (source.ByNode()) {
			for (size_t i = 0; i != target.nodes.size(); i++) {
				size_t source_node_idx = octree.Nearest(target.nodes[i]);
				target.displacement[i] = source.displacement[source_node_idx];
			}
		}
		if (source.ByIntegration()) { 
			for (size_t i = 0; i != target.integration.size(); i++) {
				size_t source_node_idx = octree.Nearest(target.integration[i]);
				target.stress[i] = source.stress[source_node_idx];
			}
		}
	}

	/*
	 * @brief Performs Distance Method using Field of Points with an Octree, with proportional distance coefficent
	 * @tparam T: point data type of the Octree, i.e. double, float
	 * @param octree: built Octree Index to use when performing the mapping
	 * @param source: source model with nodal and FE data
	 * @param target: target model to use to fill FE data with from the source with indexes preset using SetIndexes()
	 */
	template <typename T>
	void OctreeFieldOfPoints(const fedes::Octree<T>& octree, const fedes::Model& source, fedes::Model& target) {
		// Proportional distance coefficent
		if (source.ByNode()) {
			for (size_t i = 0; i != target.nodes.size(); i++) {

			}
		}
		if (source.ByIntegration()) {
			for (size_t i = 0; i != target.integration.size(); i++) {

			}
		}
	}

	/*
	 * @brief Performs Distance Method using Elements with an Octree, with proportional distance coefficent
	 * @tparam T: point data type of the Octree, i.e. double, float
	 * @param octree: built Octree Index to use when performing the mapping
	 * @param source: source model with nodal and FE data
	 * @param target: target model to use to fill FE data with from the source with indexes preset using SetIndexes()
	 */
	template <typename T>
	void OctreeDistanceUsingElements(const fedes::Octree<T>& octree, const fedes::Model& source, fedes::Model& target) {
		if (source.ByNode()) {
			for (size_t i = 0; i != target.nodes.size(); i++) {

			}
		}
		if (source.ByIntegration()) {
			for (size_t i = 0; i != target.integration.size(); i++) {

			}
		}
	}

	/*
	 * @brief Performs Element Shape Function interpolation method using an Octree
	 * @tparam T: point data type of the Octree, i.e. double, float
	 * @param octree: built Octree Index to use when performing the mapping
	 * @param source: source model with nodal and FE data
	 * @param target: target model to use to fill FE data with from the source with indexes preset using SetIndexes()
	 */
	template <typename T>
	void OctreeElementShapeFunction(const fedes::Octree<T>& octree, const fedes::Model& source, fedes::Model& target) {
		if (source.ByNode()) {
			for (size_t i = 0; i != target.nodes.size(); i++) {

			}
		}
		if (source.ByIntegration()) {
			for (size_t i = 0; i != target.integration.size(); i++) {

			}
		}
	}

	/* Explicit template instantiations */
	template void OctreeNearestPointMethod<double>(const fedes::Octree<double>&, const fedes::Model&, fedes::Model&);
	template void OctreeFieldOfPoints<double>(const fedes::Octree<double>& octree, const fedes::Model& source, fedes::Model& target);
	template void OctreeDistanceUsingElements<double>(const fedes::Octree<double>&octree, const fedes::Model & source, fedes::Model & target);
	template void OctreeElementShapeFunction<double>(const fedes::Octree<double>&octree, const fedes::Model & source, fedes::Model & target);

}

// @Todo: cover other mappings, e.g. accumulated strain