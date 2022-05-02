#pragma once

#include <filesystem>
#include <fstream>
#include <ostream>
#include <iostream>
#include <string>

namespace fedes {
	void SetInputFileStream(const std::filesystem::path& path, std::ifstream& input_stream);
	void CloseInputFileStream(std::ifstream& input_stream);
	void RewindInputStream(std::ifstream& input_stream);
	std::string StringFromFile(const std::filesystem::path& path);

	void SetOutputFileStream(const std::filesystem::path& output_file_path, std::ofstream& output_stream);
	void CloseOutputFileStream(std::ofstream& output_stream);

	void PrintCurrentDir(std::ostream& os = std::cout);
}