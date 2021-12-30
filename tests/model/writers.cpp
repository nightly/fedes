#include "fedes/model/writers.hpp"
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "fedes/model/model.hpp"
#include "fedes/model/parsers.hpp"
#include "fedes/common/files.hpp"


TEST(Writer, CreateXML) {
	fedes::Model model;
	try {
		fedes::AnsysInputReadLis("../../../models/Example1-Vane-big/Model1_Input-Ansys.txt", model);
		fedes::AnsysOutputRead("../../../models/Example1-Vane-big/Model1_output-Ansys.txt", model);
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	try {
		fedes::CreateXML("CreateXMLTest.vtu", model);
	} catch (const std::ofstream::failure& e) {
		throw;
	}

	std::string got;
	std::string expected;
	try {
		got = fedes::StringFromFile("CreateXMLTest.vtu");
		expected = fedes::StringFromFile("../../../tests/model/testdata/writers/model_1_ansys.vtu");
	} catch (const std::ifstream::failure& e) {
		throw;
	}
	std::cout << std::filesystem::current_path(); 
	ASSERT_EQ(got, expected) << "CreateXML writer doesn't produce expected output";
}