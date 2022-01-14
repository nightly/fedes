#include "fedes/model/model.hpp"

#include <string>
#include <filesystem>
#include <fstream>

#include "fedes/model/writers.hpp"

namespace fedes {

	/*
	 * @brief Transfers Finite Element Data between two models given the corresponding index
	 *
	 * @param node_index: the node index to map Finite Element data into
	 * @param source_model: the source model where the FEA/FEM data will be retrieved from
	 * @param source_node_index: the node index of the source mesh/model where FE data will be transferred to the target model at the given node_index
	 * @example (20, source, 40) will copy all FE data into (this/target[node 20] from source[node 40])
	 */
	void Model::NearestPointTransfer(size_t node_index, const fedes::Model& source_model, size_t source_node_index) {
		if (!source_model.displacement.empty()) {
			displacement[node_index] = source_model.displacement[source_node_index];
		}
		if (!source_model.stress.empty()) {
			stress[node_index] = source_model.stress[source_node_index];
		}
		if (!source_model.total_strain.empty()) {
			total_strain[node_index] = source_model.total_strain[source_node_index];
		}
		if (!source_model.plastic_strain.empty()) {
			plastic_strain[node_index] = source_model.plastic_strain[source_node_index];
		}
		if (!source_model.accumulated_strain.empty()) {
			accumulated_strain[node_index] = source_model.accumulated_strain[source_node_index];
		}
	}

	
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
			stress.resize(this->nodes.size());
		}
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

}