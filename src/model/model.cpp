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
	 * @param path: the directory to write to. @Default: ""../../../exports"
	 * 
	 * @exception Propagates std::ofstream::failure
	 */
	void Model::Export(const std::string& file_name, const std::filesystem::path& path) {
		try {
			std::string file_name_with_ext = file_name + ".vtu";
			fedes::CreateXML(path / file_name_with_ext, *this);
		} catch (const std::ofstream::failure& e) {
			throw;
		}
	}

	/* 
	 * @brief Resizes and pre-allocates all indexes for FE data allowing the arbitrary use of operator[] when mapping data
	 * 
	 * @param source: the source model so that target FE indexes can be set correctly corresponding to the source model
	 */
	void Model::ResizeIndexes(const fedes::Model& source) {
		if (!source.displacement.empty()) {
			displacement.resize(this->nodes.size());
		}
		if (!source.stress.empty()) {
			stress.resize(this->elements.size());
		}
		// @Todo: correctly resize the rest like this one, as some mappings depend on node whilst others on integration points
		if (!source.total_strain.empty()) {
			total_strain.resize(this->nodes.size());
		}
		if (!source.plastic_strain.empty()) {
			plastic_strain.resize(this->nodes.size());
		}
		if (!source.accumulated_strain.empty()) {
			accumulated_strain.resize(this->nodes.size());
		}
	}

	/*
	 * @brief Provides the integration data for the model
	 * 
	 * @port Port of "convert_coordinates" from FEDES v2 interpolations.pas
	 */
	void Model::ConvertCoordinates() {
		integration.reserve(elements.size());
		for (size_t i = 0; i < elements.size(); i++) {
			double x = 0;
			double y = 0;
			double z = 0;
			size_t br = 0;
			for (size_t j = 0; j < elements[i].size(); j++) {
				if (j > 0) {
					if (elements[i][j] != 0) {
						x = x + nodes[elements[i][j]].x;
						y = y + nodes[elements[i][j]].y;
						z = z + nodes[elements[i][j]].z;
						br++;
					}
				}
			}
			integration.emplace_back(x / br, y / br, z / br);
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