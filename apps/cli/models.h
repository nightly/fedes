#pragma once 

#include <filesystem>
#include <string>

#include "fedes/model/parsers.h"
#include "fedes/model/writers.h"


void SetModels(fedes::Model& source, fedes::Model& target, unsigned int id = 1);
void ExportModels(fedes::Model& source, fedes::Model& target, const std::string& file_suffix);