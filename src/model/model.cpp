#include "fedes/model/model.hpp"

#include <string>
#include <filesystem>
#include <fstream>

#include "fedes/model/writers.hpp"

namespace fedes {
	
	/*
	 * @brief Exports the given model in .xml/.vtu format to the given directory with the provided file name. Overwrites existing files.
	 * 
	 * @param file_name: the file name to be used without the file extension
	 * @param by_integration: whether or not FE like stress is by integration point or by node
	 * @param path: the directory to write to. @Default: ""../../../exports"
	 * 
	 * @exception Propagates std::ofstream::failure
	 */
	void Model::Export(const std::string& file_name, bool by_integration, const std::filesystem::path& path) {
		try {
			std::string file_name_with_ext = file_name + ".vtu";
			fedes::CreateXML(path / file_name_with_ext, *this, by_integration ? true : false);
		} catch (const std::ofstream::failure& e) {
			throw;
		}
	}

	/* 
	 * @brief Resizes and pre-allocates all target indexes for FE data allowing arbitrary use of operator[] + assigns integration data
	 * 
	 * Displacement and forces are mapped by node, stresses and strains are mapped by integration points/elements, hence why some indexes are 
	 * relative to the element count as opposed to node count. Integration points are only calculated when the source model has data that
	 * needs to be mapped via integration point (e.g. stress).
	 * 
	 * @param source: the source model so that target FE indexes can be set correctly corresponding to the source model
	 */
	void Model::ResizeIndexes(const fedes::Model& source) {
		if (!source.displacement.empty()) {
			displacement.resize(this->nodes.size());
		}
		if (!source.stress.empty() || !source.total_strain.empty() || !source.plastic_strain.empty() || !source.accumulated_strain.empty()) {
			AssignIntegration();
			if (!source.stress.empty()) {
				stress.resize(this->elements.size());
			}
			if (!source.total_strain.empty()) {
				total_strain.resize(this->elements.size());
			}
			if (!source.plastic_strain.empty()) {
				plastic_strain.resize(this->elements.size());
			}
			if (!source.accumulated_strain.empty()) {
				accumulated_strain.resize(this->elements.size());
			}
		}
	}

	/*
	 * @brief Provides the integration data for the model
	 * 
	 * An integration point represents the average node value for each element.
	 * Some mapping methods are done via integration point instead of by node.
	 * 
	 * @note ResizeIndexes() will invoke this function automatically when it is required
	 * @port Port of "convert_coordinates" from FEDES v2 interpolations.pas
	 */
	void Model::AssignIntegration() {
		integration.reserve(elements.size());
		for (size_t i = 0; i < elements.size(); i++) {
			double x = 0;
			double y = 0;
			double z = 0;
			size_t element_count = elements[i].size();
			for (size_t j = 0; j < element_count; j++) {
				x = x + nodes[elements[i][j]].x;
				y = y + nodes[elements[i][j]].y;
				z = z + nodes[elements[i][j]].z;
			}
			integration.emplace_back(x / element_count, y / element_count, z / element_count);
		}
	}

	/*
	 * @brief Model equality operator, which is useful for unit tests (when dealing with small models)
	 */
	bool Model::operator==(const Model& other) const {
		return (nodes == other.nodes && elements == other.elements && displacement == other.displacement && stress == other.stress &&
		total_strain == other.total_strain && plastic_strain == other.plastic_strain && accumulated_strain == other.accumulated_strain &&
		integration == other.integration);
	}
}