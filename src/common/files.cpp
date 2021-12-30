#include "fedes/common/files.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>


namespace fedes {

	/**
	 * @brief Takes a filepath and sets the stream to buffered input mode
	 * 
	 * @param path: File path that is relative from the execution binary
	 * @param input_stream: Input stream to open up
	 * @exception Propagates std::ifstream::failure
	 */
	void SetInputFileStream(const std::filesystem::path& path, std::ifstream& input_stream) {
		input_stream.exceptions(std::ifstream::badbit);
		const size_t buffer_size = 256 * 1024;
		char buffer[buffer_size];
		try {
			input_stream.rdbuf()->pubsetbuf(buffer, buffer_size);
			input_stream.open(path, std::ios::in);
		}
		catch (const std::ifstream::failure& e) {
			throw;
		}
	}

	/**
	 * @brief Closes an input file stream if currently open
	 * 
	 * @param stream: Reference to an input file stream
	 * @exception Propagates std::ifstream::failure
	 */
	void CloseInputFileStream(std::ifstream& input_stream) {
		input_stream.exceptions(std::ifstream::badbit);
		try {
			if (!(input_stream.is_open())) {
				return;
			}
			input_stream.close();
		}
		catch (const std::ifstream::failure& e) {
			throw;
		}
	}

	/*
	 * @brief RewindInputStream will reset an input stream to the first line
	 * @param input_stream: The stream that should be reset to the start of file
	 * @exception Propagates std::ifstream::failure
	 */
	void RewindInputStream(std::ifstream& input_stream) {
		input_stream.exceptions(std::ifstream::badbit);
		input_stream.clear();
		try {
			input_stream.seekg(0);
		}
		catch (const std::ifstream::failure& e) {
			throw;
		}
	}

	/*
	 * @brief StringFromFile opens a file and reads its contents into a string
	 * @param path: The path to the file
	 * @return string with the file contents copied into it
	 * @exception Propagates std::ifstream::failure
	 */
	std::string StringFromFile(const std::filesystem::path& path) {
		std::ifstream stream;
		std::stringstream buf;
		try {
			fedes::SetInputFileStream(path, stream);
			buf << stream.rdbuf();
			fedes::CloseInputFileStream(stream);
		} catch (const std::ifstream::failure& e) {
			throw;
		}
		return buf.str();
	}

	/**
	 * @brief Returns an output file stream ready for writing to based on provided output path
	 * 
	 * @param output_file: Path and name of the file to create and write to
	 * @param output_stream: Output stream to set
	 * @exception Propagates std::ofstream::failure
	 */
	void SetOutputFileStream(const std::filesystem::path& output_file_path, std::ofstream& output_stream) {
		output_stream.exceptions(std::ifstream::badbit);
		try {
			output_stream.open(output_file_path, std::ios::out | std::ios::trunc);
		}
		catch (const std::ofstream::failure& e) {
			throw;
		}
	}

	/**
	 * @brief Closes an output file stream if open
	 * 
	 * @param output_stream: The output stream in question to close
	 * @exception Propagates std::ofstream::failure
	 */
	void CloseOutputFileStream(std::ofstream& output_stream) {
		output_stream.exceptions(std::ifstream::badbit);
		try {
			if (!(output_stream.is_open())) {
				return;
			}
			output_stream.close();
		}
		catch (const std::ofstream::failure& e) {
			throw;
		}
	}
}