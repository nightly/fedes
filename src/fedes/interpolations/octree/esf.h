#pragma once

#include <concepts>

#include <thread_pool.hpp>

#include "fedes/indexing/octree/octree.h"
#include "fedes/indexing/octree/octant.h"
#include "fedes/model/model.h"
#include "fedes/maths/geometry.h"

namespace fedes {

	template <std::floating_point T>
	void ParallelElementShapeFunction(const fedes::Octree<T>& octree, const fedes::Model& source, fedes::Model& target, 
		thread_pool& pool, size_t max_leaf_scans_threshold = 1000) {
		ElementType element_type = octree.element_type();

		if (source.ByNode()) {
			std::cout << "Target node count: " << target.nodes.size() << '\n';
			std::cout << "Target displacement count: " << target.displacement.size() << '\n';

			pool.parallelize_loop(0, target.nodes.size(),
				[&](const uint32_t& a, const uint32_t& b)
				{
					for (uint32_t i = a; i < b; i++) {
						std::pair<size_t, MuesfData> data = octree.MUESF(target.nodes[i], max_leaf_scans_threshold);
						const double& g = data.second.g;
						const double& h = data.second.h;
						const double& r = data.second.r;
						const size_t& elem = data.first;
						double ghr = (1 - g - h - r);
						FEDES_INFO("Target node index {} ({}) contained within element = {}", i, target.nodes[i], data.first);
						if (element_type == ElementType::Tetrahedron) {
							for (uint_fast8_t k = 0; k < 3; ++k) {
								target.displacement[i][k] = ghr * source.displacement[source.elements[elem][0]][k] + g * source.displacement[source.elements[elem][1]][k] +
									h * source.displacement[source.elements[elem][2]][k] + r * source.displacement[source.elements[elem][3]][k];
							}

						}

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
						
						std::pair<size_t, MuesfData> data = octree.MUESF(target.integration[i], max_leaf_scans_threshold);
						const double& g = data.second.g;
						const double& h = data.second.h;
						const double& r = data.second.r;
						const size_t& elem = data.first;
						double ghr = (1 - g - h - r);

						FEDES_INFO("Target integration index {} ({}) contained within element = {}", i, target.integration[i], data.first);
						if (stress_map) {
							if (element_type == ElementType::Tetrahedron) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.stress[i][k] = ghr * source.stress[source.elements[elem][0]][k] + g * source.stress[source.elements[elem][1]][k] +
										h * source.stress[source.elements[elem][2]][k] + r * source.stress[source.elements[elem][3]][k];
								}
							}
						}

						if (plastic_strain_map) {
							if (element_type == ElementType::Tetrahedron) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.plastic_strain[i][k] = ghr * source.plastic_strain[source.elements[elem][0]][k] + g * source.plastic_strain[source.elements[elem][1]][k] +
										h * source.plastic_strain[source.elements[elem][2]][k] + r * source.plastic_strain[source.elements[elem][3]][k];
								}
							}
						}

						if (total_strain_map) {
							if (element_type == ElementType::Tetrahedron) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.total_strain[i][k] = ghr * source.total_strain[source.elements[elem][0]][k] + g * source.total_strain[source.elements[elem][1]][k] +
										h * source.total_strain[source.elements[elem][2]][k] + r * source.total_strain[source.elements[elem][3]][k];
								}
							}
						}

						if (accumulated_strain_map) {
							target.accumulated_strain[i] = ghr * source.accumulated_strain[source.elements[elem][0]] + g * source.accumulated_strain[source.elements[elem][1]] +
								h * source.accumulated_strain[source.elements[elem][2]] + r * source.accumulated_strain[source.elements[elem][3]];
						}

					}
				});
		}
	}




}