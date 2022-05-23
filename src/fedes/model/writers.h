#pragma once

#include <filesystem>

#include "fedes/model/model.h"

namespace fedes {

	void CreateXML(const std::filesystem::path& output_file_path, fedes::Model& model, bool by_integration, bool has_fea_data);

}