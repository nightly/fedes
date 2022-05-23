#include "fedes/model/export.h"

#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>

#include "fedes/model/parsers.h"
#include "fedes/model/writers.h"
#include "fedes/instrumentation/timer.h"

namespace fedes {

	/*
	 * @brief Will export the source and target models in .xml/.vtu format (visualisable with ParaView). Overwrites existing files.
	 * @param source: starting model
	 * @param target: model where data will/has been mapped to
	 * @param file_suffix: filename suffixes (before file extension) to "source" and "target". E.g. "oct-npm" = "target-oct-npm.xml"
	 * @exception Propagtes std::ofstream::failure
	 */
	void ExportModels(fedes::Model& source, fedes::Model& target,
		              const std::string& file_suffix_source, const std::string& file_suffix_target) {
		try {
			source.Export("source-" + file_suffix_source, false, true, "../../exports");
			target.Export("target-" + file_suffix_target, true, true, "../../exports");
		} catch (const std::ofstream::failure& e) {
			throw;
		}
	}

	/*
	 * @brief Like ExportModels, but only takes one model & exports the raw model (with no FEA data)
	 * @exception Propagtes std::ofstream::failure
	 */
	void ExportRawModel(fedes::Model& model, const std::string& file_suffix) {
		try {
			model.Export("target-raw-" + file_suffix, false, false, "../../exports");
		} catch (const std::ofstream::failure& e) {
			throw;
		}
	}

}