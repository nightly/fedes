#pragma once

#include <filesystem>

#include "fedes/model/model.h"

namespace fedes {

	void AbaqusInputRead(const std::filesystem::path& path, fedes::Model& model);
	void AbaqusOutputRead(const std::filesystem::path& path, fedes::Model& model);
	
	void AnsysInputReadLis(const std::filesystem::path& path, fedes::Model& model);
	void AnsysOutputRead(const std::filesystem::path& path, fedes::Model& model);
	
	void MorpheoInputRead(const std::filesystem::path& path, fedes::Model& model);
	void MorpheoInputOutputRead(const std::filesystem::path& path, fedes::Model& model);
}