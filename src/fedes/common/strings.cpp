#include "fedes/common/strings.h"

#include <string>
#include <regex>
#include <vector>
#include <sstream>
#include <algorithm>

namespace fedes {
	constexpr char const* blank = " \n\t\r\f\v";

	/*
	* @brief Trims whitespaces and blank characters from the start/left side of the string.
	* @param string The string to evaluate
	* @return A string that's been trimmed from the left-hand/beginning side
	*/
	std::string LeftTrim(const std::string& string) {
		size_t left = string.find_first_not_of(blank);
		return (left == std::string::npos) ? "" : string.substr(left);
	}

	/*
	* @brief Trims whitespaces and blank characters from the end/right-side of a string.
	* @param string The string to evaluate
	* @return A string that's been trimmed from the right-hand/end side
	*/
	std::string RightTrim(const std::string& string) {
		size_t right = string.find_last_not_of(blank);
		return (right == std::string::npos) ? "" : string.substr(0, right + 1);
	}

	/*
	 * @brief Trims whitespaces and blank characters from start/end (left/right) side of the string.
	 * @param string The string to trim
	 * @return A string that is now trimmed
	 */
	std::string Trim(const std::string& string) {
		return LeftTrim(RightTrim(string));
	}

	/*
	 * @brief StringSplit will output to a vector of strings separated based on the provided delimiter.
	 * @param splits: each splitted string will be outputted as a new entry into this vector of strings
	 * @param string: the original string
	 * @param  delimiter: the delimiter to separate the string by
	 * @return Number of splits
	 */
	size_t StringSplit(std::vector<std::string>& splits, const std::string& string, const char delimiter) {
		size_t result{0};
		std::stringstream stream(string);
		std::string current;
		while (getline(stream, current, delimiter)) {
			splits.emplace_back(current);
			result++;
		}
		return result;
	}

	/*
	 * @brief Converts a string to lower-case and returns it
	 * @param string: The string to convert to lowercase
	 * @return Newly-created lower-cased string
	 */
	std::string ToLower(const std::string& string) {
		std::string ret = string;
		std::transform(ret.begin(), ret.end(), ret.begin(), [](unsigned char const& c) { return std::tolower(c); });
		return ret;
	}
}
