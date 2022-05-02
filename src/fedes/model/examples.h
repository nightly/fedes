#pragma once 

#include <filesystem>
#include <string>

#include "fedes/model/parsers.h"
#include "fedes/model/writers.h"

namespace fedes {
	void SetExampleModels(fedes::Model& source, fedes::Model& target, size_t id);
}
