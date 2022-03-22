#pragma once 

#include <filesystem>
#include <string>

#include "fedes/model/parsers.hpp"
#include "fedes/model/writers.hpp"


void SetModels(fedes::Model& source, fedes::Model& target, unsigned int id = 1);
void ExportModels(fedes::Model& source, fedes::Model& target, const std::string& file_suffix);