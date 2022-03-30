#include "fedes/model/writers.h"
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "fedes/model/model.h"
#include "fedes/model/parsers.h"
#include "fedes/common/files.h"


TEST(Writer, CreateXML) {
	fedes::Model model;
	try {
		fedes::AnsysInputReadLis("../../models/Example1-Vane-big/Model1_Input-Ansys.txt", model);
		fedes::AnsysOutputRead("../../models/Example1-Vane-big/Model1_output-Ansys.txt", model);
	} catch (const std::ifstream::failure& e) {
		throw;
	}

	try {
		fedes::CreateXML("Test_CreateXML.vtu", model, false);
	} catch (const std::ofstream::failure& e) {
		throw;
	}

	std::string got;
	std::string expected;
	try {
		got = fedes::StringFromFile("Test_CreateXML.vtu");
		expected = fedes::StringFromFile("../../tests/model/testdata/writers/model_1_ansys.vtu");
	} catch (const std::ofstream::failure& e) {
		throw;
	}
	ASSERT_EQ(got, expected) << "CreateXML writer doesn't produce expected output";
}