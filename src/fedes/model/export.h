#pragma once 

#include <filesystem>
#include <string>

#include "fedes/model/parsers.h"
#include "fedes/model/writers.h"

namespace fedes {
	void ExportModels(fedes::Model& source, fedes::Model& target, const std::string& file_source_suffix, 
		              const std::string& file_target_suffix);
}