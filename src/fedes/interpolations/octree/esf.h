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
		const ElementType element_type = octree.element_type();

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
						FEDES_INFO("Target node index {} ({}) contained within element = {}", i, target.nodes[i], data.first);
						if (element_type == ElementType::Tetrahedron) {
							double ghr = (1 - g - h - r);
							for (uint_fast8_t k = 0; k < 3; ++k) {
								target.displacement[i][k] = ghr * source.displacement[source.elements[elem][0]][k] + g * source.displacement[source.elements[elem][1]][k] +
									h * source.displacement[source.elements[elem][2]][k] + r * source.displacement[source.elements[elem][3]][k];
							}
						} else if (element_type == ElementType::Wedge) {
							double a1 = (1 - g - h) * (1 - r);
							double a2 = g * (1 - r);
							double a3 = h * (1 - r);
							double a4 = (1 - g - h) * (1 + r);
							double a5 = g * (1 + r);
							double a6 = h * (1 + r);
							for (uint_fast8_t k = 0; k < 3; ++k) {
								target.displacement[i][k] = a1 * source.displacement[source.elements[elem][0]][k] + a2 * source.displacement[source.elements[elem][1]][k] +
									a3 * source.displacement[source.elements[elem][2]][k] + a4 * source.displacement[source.elements[elem][3]][k] +
									a5 * source.displacement[source.elements[elem][4]][k] + a6 * source.displacement[source.elements[elem][5]][k];
							}
						} else if (element_type == ElementType::Hexahedron) {
							double a1 = (1 - g) * (1 - h) * (1 - r);
							double a2 = (1 + g) * (1 - h) * (1 - r);
							double a3 = (1 + g) * (1 + h) * (1 - r);
							double a4 = (1 - g) * (1 + h) * (1 - r);
							double a5 = (1 - g) * (1 - h) * (1 + r);
							double a6 = (1 + g) * (1 - h) * (1 + r);
							double a7 = (1 + g) * (1 + h) * (1 + r);
							double a8 = (1 - g) * (1 + h) * (1 + r);
							for (uint_fast8_t k = 0; k < 3; ++k) {
								target.displacement[i][k] = a1 * source.displacement[source.elements[elem][0]][k] + a2 * source.displacement[source.elements[elem][1]][k] +
									a3 * source.displacement[source.elements[elem][2]][k] + a4 * source.displacement[source.elements[elem][3]][k] +
									a5 * source.displacement[source.elements[elem][4]][k] + a6 * source.displacement[source.elements[elem][5]][k] +
									a5 * source.displacement[source.elements[elem][6]][k] + a6 * source.displacement[source.elements[elem][7]][k];
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

						FEDES_INFO("Target integration index {} ({}) contained within element = {}", i, target.integration[i], data.first);

						if (element_type == ElementType::Tetrahedron) {
							double ghr = (1 - g - h - r);
							if (stress_map) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.stress[i][k] = ghr * source.stress[source.elements[elem][0]][k] + g * source.stress[source.elements[elem][1]][k] +
										h * source.stress[source.elements[elem][2]][k] + r * source.stress[source.elements[elem][3]][k];
								}
							}

							if (plastic_strain_map) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.plastic_strain[i][k] = ghr * source.plastic_strain[source.elements[elem][0]][k] + g * source.plastic_strain[source.elements[elem][1]][k] +
										h * source.plastic_strain[source.elements[elem][2]][k] + r * source.plastic_strain[source.elements[elem][3]][k];
								}
							}

							if (total_strain_map) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.total_strain[i][k] = ghr * source.total_strain[source.elements[elem][0]][k] + g * source.total_strain[source.elements[elem][1]][k] +
										h * source.total_strain[source.elements[elem][2]][k] + r * source.total_strain[source.elements[elem][3]][k];
								}
							}

							if (accumulated_strain_map) {
									target.accumulated_strain[i] = ghr * source.accumulated_strain[source.elements[elem][0]] + g * source.accumulated_strain[source.elements[elem][1]] +
										h * source.accumulated_strain[source.elements[elem][2]] + r * source.accumulated_strain[source.elements[elem][3]];
							}
						} else if (element_type == ElementType::Wedge) {
							double a1 = (1 - g - h) * (1 - r);
							double a2 = g * (1 - r);
							double a3 = h * (1 - r);
							double a4 = (1 - g - h) * (1 + r);
							double a5 = g * (1 + r);
							double a6 = h * (1 + r);

							if (stress_map) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.stress[i][k] = a1 * source.stress[source.elements[elem][0]][k] + a2 * source.stress[source.elements[elem][1]][k] +
										a3 * source.stress[source.elements[elem][2]][k] + a4 * source.stress[source.elements[elem][3]][k] +
										a5 * source.stress[source.elements[elem][4]][k] + a6 * source.stress[source.elements[elem][5]][k];
								}
							}

							if (plastic_strain_map) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.plastic_strain[i][k] = a1 * source.plastic_strain[source.elements[elem][0]][k] + a2 * source.plastic_strain[source.elements[elem][1]][k] +
										a3 * source.plastic_strain[source.elements[elem][2]][k] + a4 * source.plastic_strain[source.elements[elem][3]][k] +
										a5 * source.plastic_strain[source.elements[elem][4]][k] + a6 * source.plastic_strain[source.elements[elem][5]][k];
								}
							}

							if (total_strain_map) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.total_strain[i][k] = a1 * source.total_strain[source.elements[elem][0]][k] + a2 * source.total_strain[source.elements[elem][1]][k] +
										a3 * source.total_strain[source.elements[elem][2]][k] + a4 * source.total_strain[source.elements[elem][3]][k] +
										a5 * source.total_strain[source.elements[elem][4]][k] + a6 * source.total_strain[source.elements[elem][5]][k];
								}
							}

							if (accumulated_strain_map) {
								target.accumulated_strain[i] = a1 * source.accumulated_strain[source.elements[elem][0]] + a2 * source.accumulated_strain[source.elements[elem][1]] +
									a3 * source.accumulated_strain[source.elements[elem][2]] + a4 * source.accumulated_strain[source.elements[elem][3]] +
									a5 * source.accumulated_strain[source.elements[elem][4]] + a6 * source.accumulated_strain[source.elements[elem][5]];
							}

						} else if (element_type == ElementType::Hexahedron) {
							double a1 = (1 - g) * (1 - h) * (1 - r);
							double a2 = (1 + g) * (1 - h) * (1 - r);
							double a3 = (1 + g) * (1 + h) * (1 - r);
							double a4 = (1 - g) * (1 + h) * (1 - r);
							double a5 = (1 - g) * (1 - h) * (1 + r);
							double a6 = (1 + g) * (1 - h) * (1 + r);
							double a7 = (1 + g) * (1 + h) * (1 + r);
							double a8 = (1 - g) * (1 + h) * (1 + r);

							if (stress_map) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.stress[i][k] = a1 * source.stress[source.elements[elem][0]][k] + a2 * source.stress[source.elements[elem][1]][k] +
										a3 * source.stress[source.elements[elem][2]][k] + a4 * source.stress[source.elements[elem][3]][k] +
										a5 * source.stress[source.elements[elem][4]][k] + a6 * source.stress[source.elements[elem][5]][k] +
										a5 * source.stress[source.elements[elem][6]][k] + a6 * source.stress[source.elements[elem][7]][k];
								}
							}

							if (plastic_strain_map) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.plastic_strain[i][k] = a1 * source.plastic_strain[source.elements[elem][0]][k] + a2 * source.plastic_strain[source.elements[elem][1]][k] +
										a3 * source.plastic_strain[source.elements[elem][2]][k] + a4 * source.plastic_strain[source.elements[elem][3]][k] +
										a5 * source.plastic_strain[source.elements[elem][4]][k] + a6 * source.plastic_strain[source.elements[elem][5]][k] +
										a5 * source.plastic_strain[source.elements[elem][6]][k] + a6 * source.plastic_strain[source.elements[elem][7]][k];
								}
							}

							if (total_strain_map) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.total_strain[i][k] = a1 * source.total_strain[source.elements[elem][0]][k] + a2 * source.total_strain[source.elements[elem][1]][k] +
										a3 * source.total_strain[source.elements[elem][2]][k] + a4 * source.total_strain[source.elements[elem][3]][k] +
										a5 * source.total_strain[source.elements[elem][4]][k] + a6 * source.total_strain[source.elements[elem][5]][k] +
										a5 * source.total_strain[source.elements[elem][6]][k] + a6 * source.total_strain[source.elements[elem][7]][k];
								}
							}

							if (accumulated_strain_map) {
								target.accumulated_strain[i] = a1 * source.accumulated_strain[source.elements[elem][0]] + a2 * source.accumulated_strain[source.elements[elem][1]] +
									a3 * source.accumulated_strain[source.elements[elem][2]] + a4 * source.accumulated_strain[source.elements[elem][3]] +
									a5 * source.accumulated_strain[source.elements[elem][4]] + a6 * source.accumulated_strain[source.elements[elem][5]] +
									a5 * source.accumulated_strain[source.elements[elem][6]] + a6 * source.accumulated_strain[source.elements[elem][7]];
							}

						}


					}
				});
		}
	}




}