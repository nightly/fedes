#include "fedes/model/model.h"

#include <string>
#include <filesystem>
#include <fstream>

#include "fedes/model/writers.h"
#include "fedes/maths/element_type.h"

namespace fedes {
	
	/*
	 * @brief Exports the given model in .xml/.vtu format to the given directory with the provided file name. Overwrites existing files.
	 * @param file_name: the file name to be used without the file extension
	 * @param by_integration: whether or not FE like stress is by integration point or by node
	 * @param path: the directory to write to. @Default: ""../../../exports"
	 * @exception Propagates std::ofstream::failure
	 */
	void Model::Export(const std::string& file_name, bool by_integration, const std::filesystem::path& path) {
		try {
			std::string file_name_with_ext = file_name + ".vtu";
			fedes::CreateXML(path / file_name_with_ext, *this, by_integration);
		} catch (const std::ofstream::failure& e) {
			throw;
		}
	}

	/* 
	 * @brief  For the target mesh: resizes and pre-allocates all target indexes for FE data,
	 * allowing arbitrary use of operator[] + assigns integration data (see: AssignIntegration).
	 *
	 * Displacement and forces are mapped by node, stresses and strains are mapped by integration points/elements.
	 * Integration points are only calculated if the source model has data that needs mapping via integration points.
	 * 
	 * @param source: the source model so that target FE indexes can be set correctly corresponding to the source model
	 */
	void Model::SetTargetIndexes(const fedes::Model& source) {
		if (!source.displacement.empty()) {
			displacement.resize(this->nodes.size(), std::vector<double>(3));
		}
		if (!source.stress.empty() || !source.total_strain.empty() || !source.plastic_strain.empty() || !source.accumulated_strain.empty()) {
			AssignIntegration();
			if (!source.stress.empty()) {
				stress.resize(this->elements.size(), std::vector<double>(6));
			}
			if (!source.total_strain.empty()) {
				total_strain.resize(this->elements.size(), std::vector<double>(6));
			}
			if (!source.plastic_strain.empty()) {
				plastic_strain.resize(this->elements.size(),std::vector<double>(6));
			}
			if (!source.accumulated_strain.empty()) {
				accumulated_strain.resize(this->elements.size());
			}
		}
	}

	/*
	 * @brief Provides the integration data for the model, called by default from SetTargetIndexes(). Idempotent.
	 * 
	 * An integration point represents the average/center node value for any given element.
	 * Some mapping methods are done via integration point instead of by node (e.g stresses and strains).
	 * 
	 * @port Port of "convert_coordinates" from FEDES v2 interpolations.pas
	 */
	void Model::AssignIntegration() {
		if (!integration.empty()) {
			return; 
		}
		integration.reserve(elements.size());
		for (size_t i = 0; i < elements.size(); i++) {
			double x = 0;
			double y = 0;
			double z = 0;
			size_t element_count = elements[i].size();
			for (size_t j = 0; j < element_count; j++) {
				x += nodes[elements[i][j]].x;
				y += nodes[elements[i][j]].y;
				z += nodes[elements[i][j]].z;
			}
			integration.emplace_back(x / element_count, y / element_count, z / element_count);
		}
	}

	/*
	 * @brief Determines whether there is any Finite Element Data contained that is to be mapped by node.
	 * @return True if there's any FE data to be mapped by node (i.e. displacement), false otherwise.
	 */
	bool Model::ByNode() const {
		return !displacement.empty() ? true : false;
	}
	
	/*
	 * @brief Determines whether there is any Finite Element Data contained that is to be mapped via integration point.
	 * @return True if there is (e.g. stress), false otherwise (meaning no FEA data or only data to be mapped by node).
	 */
	bool Model::ByIntegration() const {
		if (!stress.empty() || !accumulated_strain.empty() || !plastic_strain.empty() || !total_strain.empty()) {
			return true;
		}
		return false;
	}


	/*
	 * @brief Model equality operator, which is useful for unit tests (when dealing with small models)
	 */
	bool Model::operator==(const Model& other) const {
		return ((nodes == other.nodes) && (elements == other.elements) && (displacement == other.displacement) && (stress == other.stress) &&
		(total_strain == other.total_strain) && (plastic_strain == other.plastic_strain) && (accumulated_strain == other.accumulated_strain));
	}
}