#pragma once

#include <concepts>

#include <thread_pool.hpp>

#include "fedes/indexing/octree/octree.h"
#include "fedes/indexing/octree/octant.h"
#include "fedes/model/model.h"

namespace fedes {

	template <std::floating_point T = double>
	static std::pair<std::vector<T>, T> ProportionalDistanceCoefficents(const std::vector<size_t>& element, const fedes::Model& source,
		    const fedes::Vector3<T>& query) {
		std::vector<T> prop;
		std::vector<T> distances;
		prop.resize(element.size());
		distances.resize(element.size());

		T dist_total = 0;
		T prop_sum = 0;

		for (size_t n = 0; n < element.size(); ++n) {
			T dist = fedes::Distance(query, source.nodes[element[n]]);
			distances[n] = dist;
			dist_total += dist;
		}

		// Coefficent
		for (size_t i = 0; i < element.size(); ++i) {
			T prop_coefficent = dist_total / distances[i];
			prop[i] = prop_coefficent;
			prop_sum += prop_coefficent;
		}

		return { prop, prop_sum };
	}


	template <std::floating_point T = double>
	void ParallelDMUE(const fedes::Octree<T>& octree, const fedes::Model& source, fedes::Model& target, thread_pool& pool, size_t scan_min = 50) {
		if (source.ByNode()) {
			std::cout << "Target node count: " << target.nodes.size() << '\n';
			std::cout << "Minimum element scans: " << scan_min << '\n';

			pool.parallelize_loop(0, target.nodes.size(),
				[&](const uint32_t& a, const uint32_t& b)
				{
					for (uint32_t i = a; i < b; i++) {
						int_fast64_t coincide = octree.Find(target.nodes[i]);
						if (coincide != -1) {
							target.displacement[i] = source.displacement[coincide];
							continue;
						}

						size_t best_element = octree.DMUE(target.nodes[i], scan_min);
						FEDES_INFO("Target node index {} ({}) selected element = {}", i, target.nodes[i], best_element);
						std::pair<std::vector<T>, T> coefficents = ProportionalDistanceCoefficents(source.elements[best_element], source, target.nodes[i]);
						

						for (size_t n = 0; n < source.elements[best_element].size(); ++n) {
							for (uint_fast8_t k = 0; k < 3; ++k) {
								target.displacement[i][k] += source.displacement[source.elements[best_element][n]][k] * (coefficents.first[n] / coefficents.second);
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
						int_fast64_t coincide = octree.Find(target.integration[i]);
						if (coincide != -1) {
							if (stress_map) {
								target.stress[i] = source.stress[coincide];
							}
							if (plastic_strain_map) {
								target.plastic_strain[i] = source.plastic_strain[coincide];
							}
							if (total_strain_map) {
								target.total_strain[i] = source.total_strain[coincide];
							}
							if (accumulated_strain_map) {
								target.accumulated_strain[i] = source.accumulated_strain[coincide];
							}
							continue;
						}

						size_t best_element = octree.DMUE(target.integration[i], scan_min);
						FEDES_INFO("Target integration index {} ({}) selected element = {}", i, target.integration[i], best_element);
						std::pair<std::vector<T>, T> coefficents = ProportionalDistanceCoefficents(source.elements[best_element], source, target.integration[i]);


						if (stress_map) {
							for (size_t n = 0; n < source.elements[best_element].size(); ++n) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.stress[i][k] += source.stress[source.elements[best_element][n]][k] * (coefficents.first[n] / coefficents.second);
								}
							}
						}

						if (plastic_strain_map) {
							for (size_t n = 0; n < source.elements[best_element].size(); ++n) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.plastic_strain[i][k] += source.plastic_strain[source.elements[best_element][n]][k] * (coefficents.first[n] / coefficents.second);
								}
							}
						}

						if (total_strain_map) {
							for (size_t n = 0; n < source.elements[best_element].size(); ++n) {
								for (uint_fast8_t k = 0; k < 6; ++k) {
									target.total_strain[i][k] += source.total_strain[source.elements[best_element][n]][k] * (coefficents.first[n] / coefficents.second);
								}
							}
						}

						if (accumulated_strain_map) {
							for (size_t n = 0; n < source.elements[best_element].size(); ++n) {
								for (uint_fast8_t k = 0; k < 3; ++k) {
									target.accumulated_strain[i] += source.accumulated_strain[source.elements[best_element][n]] * (coefficents.first[n] / coefficents.second);
								}
							}
						}

					}
				});
		}
	}

	template <std::floating_point T = double>
	void DMUE(const fedes::Octree<T>& octree, const fedes::Model& source, fedes::Model& target, thread_pool& pool, size_t scan_min = 50) {
		if (source.ByNode()) {
			std::cout << "Target node count: " << target.nodes.size() << '\n';
			std::cout << "Target displacement count: " << target.displacement.size() << '\n';

			for (size_t i = 0; i < target.nodes.size(); ++i) {
				int_fast64_t coincide = octree.Find(target.nodes[i]);
				if (coincide != -1) {
					target.displacement[i] = source.displacement[coincide];
					continue;
				}

				size_t best_element = octree.DMUE(target.nodes[i], scan_min);
				FEDES_INFO("Target node index {} ({}) selected element = {}", i, target.nodes[i], best_element);
				std::pair<std::vector<T>, T> coefficents = ProportionalDistanceCoefficents(source.elements[best_element], source, target.nodes[i]);


				for (size_t n = 0; n < source.elements[best_element].size(); ++n) {
					for (uint_fast8_t k = 0; k < 3; ++k) {
						target.displacement[i][k] += source.displacement[source.elements[best_element][n]][k] * (coefficents.first[n] / coefficents.second);
					}
				}

			}
		}

		if (source.ByIntegration()) {
			std::cout << "Target integration count: " << target.integration.size() << '\n';
			bool stress_map = !source.stress.empty();
			bool plastic_strain_map = !source.plastic_strain.empty();
			bool accumulated_strain_map = !source.accumulated_strain.empty();
			bool total_strain_map = !source.total_strain.empty();

			for (size_t i = 0; i < target.integration.size(); ++i) {
				int_fast64_t coincide = octree.Find(target.integration[i]);
				if (coincide != -1) {
					if (stress_map) {
						target.stress[i] = source.stress[coincide];
					}
					if (plastic_strain_map) {
						target.plastic_strain[i] = source.plastic_strain[coincide];
					}
					if (total_strain_map) {
						target.total_strain[i] = source.total_strain[coincide];
					}
					if (accumulated_strain_map) {
						target.accumulated_strain[i] = source.accumulated_strain[coincide];
					}
					continue;
				}

				size_t best_element = octree.DMUE(target.integration[i], scan_min);
				FEDES_INFO("Target integration index {} ({}) selected element = {}", i, target.integration[i], best_element);
				std::pair<std::vector<T>, T> coefficents = ProportionalDistanceCoefficents(source.elements[best_element], source, target.integration[i]);


				if (stress_map) {
					for (size_t n = 0; n < source.elements[best_element].size(); ++n) {
						for (uint_fast8_t k = 0; k < 6; ++k) {
							target.stress[i][k] += source.stress[source.elements[best_element][n]][k] * (coefficents.first[n] / coefficents.second);
						}
					}
				}

				if (plastic_strain_map) {
					for (size_t n = 0; n < source.elements[best_element].size(); ++n) {
						for (uint_fast8_t k = 0; k < 6; ++k) {
							target.plastic_strain[i][k] += source.plastic_strain[source.elements[best_element][n]][k] * (coefficents.first[n] / coefficents.second);
						}
					}
				}

				if (total_strain_map) {
					for (size_t n = 0; n < source.elements[best_element].size(); ++n) {
						for (uint_fast8_t k = 0; k < 6; ++k) {
							target.total_strain[i][k] += source.total_strain[source.elements[best_element][n]][k] * (coefficents.first[n] / coefficents.second);
						}
					}
				}

				if (accumulated_strain_map) {
					for (size_t n = 0; n < source.elements[best_element].size(); ++n) {
						for (uint_fast8_t k = 0; k < 3; ++k) {
							target.accumulated_strain[i] += source.accumulated_strain[source.elements[best_element][n]] * (coefficents.first[n] / coefficents.second);
						}
					}
				}

			}
		}
	}

} //namespace end