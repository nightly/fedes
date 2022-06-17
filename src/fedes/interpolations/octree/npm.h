#pragma once

#include <concepts>

#include <thread_pool.hpp>

#include "fedes/indexing/octree/octree.h"
#include "fedes/indexing/octree/octant.h"
#include "fedes/model/model.h"
#include "fedes/common/log.h"

namespace fedes {

	/*
	 * @brief Performs nearest point method using an Octree with mappings done in parallel via splitting workload
	 * @tparam T: point data type of the Octree, i.e. double, float
	 * @param octree: built Octree Index to use when performing the mapping
	 * @param source: source model with nodal and FE data
	 * @param target: target model to use to fill FE data with from the source with indexes preset using SetIndexes()
	 * @param pool: the thread pool to parallelize with
	 */
	template <std::floating_point T = double>
	void ParallelNearestPointMethod(const fedes::Octree<T>& octree, const fedes::Model& source, fedes::Model& target, thread_pool& pool) {
		if (source.ByNode()) {
			std::cout << "Target node count: " << target.nodes.size() << '\n';

			pool.parallelize_loop(0, target.nodes.size(),
				[&](const uint32_t& a, const uint32_t& b)
				{
					for (uint32_t i = a; i < b; i++) {
						size_t source_node_idx = octree.Nearest(target.nodes[i]);
						FEDES_INFO("Target node index {} ({}). Found nearest node at Source index {} ({})", i, target.nodes[i],
							source_node_idx, source.nodes[source_node_idx]);
						target.displacement[i] = source.displacement[source_node_idx];
					}
				});
		}

		if (source.ByIntegration()) {
			std::cout << "Target integration count: " << target.integration.size() << '\n';
			bool stress_map = !source.stress.empty();
			bool plastic_strain_map = !source.plastic_strain.empty();
			bool accumulated_strain_map = !source.accumulated_strain.empty();
			bool total_strain_map = !source.total_strain.empty();

			pool.parallelize_loop(0, target.integration.size(),
				[&](const uint32_t& a, const uint32_t& b)
				{
					for (uint32_t i = a; i < b; i++) {
						size_t source_node_idx = octree.Nearest(target.integration[i]);
						FEDES_INFO("Target integration index {} ({}). Found nearest node at Source index {} ({})", i, target.integration[i],
							source_node_idx, source.nodes[source_node_idx]);
						if (stress_map) {
							target.stress[i] = source.stress[source_node_idx];
						}
						if (plastic_strain_map) {
							target.plastic_strain[i] = source.plastic_strain[source_node_idx];
						}
						if (accumulated_strain_map) {
							target.accumulated_strain[i] = source.accumulated_strain[source_node_idx];
						}
						if (total_strain_map) {
							target.total_strain[i] = source.total_strain[source_node_idx];
						}
					}
				});
		}
	}


	/*
	 * @brief Performs nearest point method using an Octree
	 * @tparam T: point data type of the Octree, i.e. double, float
	 * @param octree: built Octree Index to use when performing the mapping
	 * @param source: source model with nodal and FE data
	 * @param target: target model to use to fill FE data with from the source with indexes preset using SetIndexes()
	 */
	template <std::floating_point T = double>
	void NearestPointMethod(const fedes::Octree<T>& octree, const fedes::Model& source, fedes::Model& target) {
		if (source.ByNode()) {
			for (size_t i = 0; i != target.nodes.size(); i++) {
				size_t source_node_idx = octree.Nearest(target.nodes[i]);
				FEDES_INFO("Target node index {} ({}). Found nearest node at Source index {} ({})", i, target.nodes[i],
					source_node_idx, source.nodes[source_node_idx]);
				target.displacement[i] = source.displacement[source_node_idx];
			}
		}

		if (source.ByIntegration()) {
			bool stress_map = !source.stress.empty();
			bool plastic_strain_map = !source.plastic_strain.empty();
			bool accumulated_strain_map = !source.accumulated_strain.empty();
			bool total_strain_map = !source.total_strain.empty();

			for (size_t i = 0; i != target.integration.size(); i++) {
				size_t source_node_idx = octree.Nearest(target.integration[i]);
				FEDES_INFO("Target integration index {} ({}). Found nearest node at Source index {} ({})", i, target.integration[i],
					source_node_idx, source.nodes[source_node_idx]);
				if (stress_map) {
					target.stress[i] = source.stress[source_node_idx];
				}
				if (plastic_strain_map) {
					target.plastic_strain[i] = source.plastic_strain[source_node_idx];
				}
				if (accumulated_strain_map) {
					target.accumulated_strain[i] = source.accumulated_strain[source_node_idx];
				}
				if (total_strain_map) {
					target.total_strain[i] = source.total_strain[source_node_idx];
				}
			}
		}
	}


}