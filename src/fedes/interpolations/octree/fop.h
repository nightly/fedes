#pragma once

#include <concepts>
#include <cmath>
#include <vector>

#include <thread_pool.hpp>

#include "fedes/indexing/octree/octree.h"
#include "fedes/indexing/octree/octant.h"
#include "fedes/model/model.h"
#include "fedes/maths/vector3.h"

namespace fedes {

	template <std::floating_point T = double>
	static std::pair<std::array<T, 8>, T> ProportionalDistanceCoefficents(const std::array<std::pair<size_t, T>, 8>& field) {
		std::array<T, 8> coefficents;
		T dist_total = 0; 
		T coefficent_total = 0;

		for (const auto& pair : field) {
			if (pair != std::pair<size_t, T>()) {
				dist_total += pair.second;
			}
		}

		// Coefficent = total distance / current distance
		for (uint_fast8_t i = 0; i < 8; ++i) {
			if (field[i] == std::pair<size_t, T>()) {
				continue;
			}
			T data = dist_total / field[i].second;
			coefficents[i] = data;
			coefficent_total += data;
		}

		return {coefficents, coefficent_total};
	}



	template <std::floating_point T = double>
	void ParallelFieldOfPoints(const fedes::Octree<T>& octree, const fedes::Model& source, fedes::Model& target, thread_pool& pool, T radius = 10.0) {
		if (source.ByNode()) {
			std::cout << "Radius: " << radius << "\n";
			std::cout << "Target node count: " << target.nodes.size() << '\n';

			pool.parallelize_loop(0, target.nodes.size(),
				[&](const uint32_t& a, const uint32_t& b)
				{
					for (uint32_t i = a; i < b; i++) {
						int_fast64_t coincide = octree.Find(target.nodes[i]);
						if (coincide != -1) {
							target.displacement[i] = source.displacement[coincide];
							continue;
						}

						std::array<std::pair<size_t, T>, 8> field = octree.FieldOfPoints(target.nodes[i], radius);
						std::pair<std::array<T, 8>, T> coefficents = ProportionalDistanceCoefficents(field);

						FEDES_INFO("Target node index {} ({}): quadrants 0 ({}), 1 ({}), 2 ({}), 3 ({}), 4 ({}), 5 ({}), 6 ({}), 7 ({})",
							i, target.nodes[i], source.nodes[field[0].first], source.nodes[field[1].first], source.nodes[field[2].first], source.nodes[field[3].first], 
							source.nodes[field[4].first], source.nodes[field[5].first], source.nodes[field[6].first], source.nodes[field[7].first]);
						

						for (uint_fast8_t j = 0; j < 8; ++j) {
							if (field[j] != std::pair<size_t, T>()) {
								for (uint_fast8_t k = 0; k < 3; ++k) {
									target.displacement[i][k] += source.displacement[field[j].first][k] * (coefficents.first[j] / coefficents.second);
								}
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

						std::array<std::pair<size_t, T>, 8> field = octree.FieldOfPoints(target.integration[i], radius);
						std::pair<std::array<T, 8>, T> coefficents = ProportionalDistanceCoefficents(field);

						FEDES_INFO("Target integration index {} ({}): quadrants 0 ({}), 1 ({}), 2 ({}), 3 ({}), 4 ({}), 5 ({}), 6 ({}), 7 ({})",
							i, target.integration[i], source.nodes[field[0].first], source.nodes[field[1].first], source.nodes[field[2].first], source.nodes[field[3].first],
							source.nodes[field[4].first], source.nodes[field[5].first], source.nodes[field[6].first], source.nodes[field[7].first]);
						
						if (stress_map) {
							for (uint_fast8_t j = 0; j < 8; ++j) {
								if (field[j] != std::pair<size_t, T>()) {
									for (uint_fast8_t k = 0; k < 6; ++k) {
										target.stress[i][k] += source.stress[field[j].first][k] * (coefficents.first[j] / coefficents.second);
									}
								}
							}
						}

						if (plastic_strain_map) {
							for (uint_fast8_t j = 0; j < 8; ++j) {
								if (field[j] != std::pair<size_t, T>()) {
									for (uint_fast8_t k = 0; k < 6; ++k) {
										target.plastic_strain[i][k] += source.plastic_strain[field[j].first][k] * (coefficents.first[j] / coefficents.second);
									}
								}
							}
						}

						if (total_strain_map) {
							for (uint_fast8_t j = 0; j < 8; ++j) {
								if (field[j] != std::pair<size_t, T>()) {
									for (uint_fast8_t k = 0; k < 6; ++k) {
										target.total_strain[i][k] += source.total_strain[field[j].first][k] * (coefficents.first[j] / coefficents.second);
									}
								}
							}
						}

						if (accumulated_strain_map) {
							for (uint_fast8_t j = 0; j < 8; ++j) {
								if (field[j] != std::pair<size_t, T>()) {
									for (uint_fast8_t k = 0; k < 3; ++k) {
										target.accumulated_strain[i] += source.accumulated_strain[field[j].first] * (coefficents.first[j] / coefficents.second);
									}
								}
							}
						}


					}
				});
		}
	}

	template <std::floating_point T = double>
	void FieldOfPoints(const fedes::Octree<T>& octree, const fedes::Model& source, fedes::Model& target, thread_pool& pool, T radius = 10.0) {

		if (source.ByNode()) {
			std::cout << "Radius: " << radius << "\n";
			std::cout << "Target node count: " << target.nodes.size() << '\n';
			std::cout << "Target displacement count: " << target.displacement.size() << '\n';

			for (size_t i = 0; i < target.nodes.size(); ++i) {
				int_fast64_t coincide = octree.Find(target.nodes[i]);
				if (coincide != -1) {
					target.displacement[i] = source.displacement[coincide];
					continue;
				}

				std::array<std::pair<size_t, T>, 8> field = octree.FieldOfPoints(target.nodes[i], radius);
				std::pair<std::array<T, 8>, T> coefficents = ProportionalDistanceCoefficents(field);

				FEDES_INFO("Target node index {} ({}): quadrants 0 ({}), 1 ({}), 2 ({}), 3 ({}), 4 ({}), 5 ({}), 6 ({}), 7 ({})",
					i, target.nodes[i], source.nodes[field[0].first], source.nodes[field[1].first], source.nodes[field[2].first], source.nodes[field[3].first],
					source.nodes[field[4].first], source.nodes[field[5].first], source.nodes[field[6].first], source.nodes[field[7].first]);


				for (uint_fast8_t j = 0; j < 8; ++j) {
					if (field[j] != std::pair<size_t, T>()) {
						for (uint_fast8_t k = 0; k < 3; ++k) {
							target.displacement[i][k] += source.displacement[field[j].first][k] * (coefficents.first[j] / coefficents.second);
						}
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

				std::array<std::pair<size_t, T>, 8> field = octree.FieldOfPoints(target.integration[i], radius);
				std::pair<std::array<T, 8>, T> coefficents = ProportionalDistanceCoefficents(field);

				FEDES_INFO("Target integration index {} ({}): quadrants 0 ({}), 1 ({}), 2 ({}), 3 ({}), 4 ({}), 5 ({}), 6 ({}), 7 ({})",
					i, target.integration[i], source.nodes[field[0].first], source.nodes[field[1].first], source.nodes[field[2].first], source.nodes[field[3].first],
					source.nodes[field[4].first], source.nodes[field[5].first], source.nodes[field[6].first], source.nodes[field[7].first]);

				if (stress_map) {
					for (uint_fast8_t j = 0; j < 8; ++j) {
						if (field[j] != std::pair<size_t, T>()) {
							for (uint_fast8_t k = 0; k < 6; ++k) {
								target.stress[i][k] += source.stress[field[j].first][k] * (coefficents.first[j] / coefficents.second);
							}
						}
					}
				}

				if (plastic_strain_map) {
					for (uint_fast8_t j = 0; j < 8; ++j) {
						if (field[j] != std::pair<size_t, T>()) {
							for (uint_fast8_t k = 0; k < 6; ++k) {
								target.plastic_strain[i][k] += source.plastic_strain[field[j].first][k] * (coefficents.first[j] / coefficents.second);
							}
						}
					}
				}

				if (total_strain_map) {
					for (uint_fast8_t j = 0; j < 8; ++j) {
						if (field[j] != std::pair<size_t, T>()) {
							for (uint_fast8_t k = 0; k < 6; ++k) {
								target.total_strain[i][k] += source.total_strain[field[j].first][k] * (coefficents.first[j] / coefficents.second);
							}
						}
					}
				}

				if (accumulated_strain_map) {
					for (uint_fast8_t j = 0; j < 8; ++j) {
						if (field[j] != std::pair<size_t, T>()) {
							for (uint_fast8_t k = 0; k < 3; ++k) {
								target.accumulated_strain[i] += source.accumulated_strain[field[j].first] * (coefficents.first[j] / coefficents.second);
							}
						}
					}
				}
			}
		}
		
	}




} // namespace end