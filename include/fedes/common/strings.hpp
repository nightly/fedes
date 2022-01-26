#pragma once

#include <string>
#include <regex>
#include <vector>

namespace fedes {
	std::string LeftTrim(const std::string& string);
	std::string RightTrim(const std::string& string);
	std::string Trim(const std::string& string);

	size_t StringSplit(std::vector<std::string>& splits, const std::string& string, const char delimiter);
	std::string ToLower(const std::string& string);
}