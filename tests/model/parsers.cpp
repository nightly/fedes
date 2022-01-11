#include "fedes/model/parsers.hpp"
#include <gtest/gtest.h>

#include <fstream>
#include <array>
#include <vector>
#include <cstring>

#include "fedes/model/model.hpp"
#include "fedes/common/files.hpp"
#include "fedes/instrumentation/timer.hpp"

class ParserTest : public ::testing::Test {
protected:
	fedes::Model model_;
	void SetUp() override {
	}
};

TEST_F(ParserTest, AnsysReadLis) {
	try {
		fedes::AnsysInputReadLis("../../../models/Example1-Vane-big/Model1_Input-Ansys.txt", model_);
	} catch (std::ifstream::failure& e) {
		FAIL() << e.what();
	}
	
	ASSERT_EQ(model_.nodes.size(), 54128);
	ASSERT_EQ(model_.elements.size(), 171188);

	ASSERT_EQ(model_.nodes[12300].y, -69.125);
	ASSERT_EQ(model_.nodes[54127].x, 103.64);

	ASSERT_EQ(model_.elements[20000], std::vector<int>({52497, 3345, 3344, 40830}));
	ASSERT_EQ(model_.elements[171187], std::vector<int>({ 27638, 27635, 22109, 27642 }));
}

TEST_F(ParserTest, AnsysOutputRead) {
	try {
		// Only contains displacement and stress
		fedes::AnsysOutputRead("../../../models/Example1-Vane-big/Model1_output-Ansys.txt", model_);
	}
	catch (std::ifstream::failure& e) {
		FAIL() << e.what();
	}
	ASSERT_EQ(model_.displacement.size(), 54128);
	ASSERT_EQ(model_.stress.size(), 54128);

	ASSERT_EQ(model_.displacement[27000], std::vector<double>({ 0.064386000000, 0.070148000000, 0.463610000000 }));

	ASSERT_EQ(model_.stress[27000], std::vector<double>({ 481960000.00, 48065000.00, -54431000.00, -106780000.00, 18002000.00, 38856000.00 }));
}

TEST_F(ParserTest, AbaqusInputRead) {
	try {
		fedes::AbaqusInputRead("../../../models/Example-3D-medium/model1-input-Abaqus.inp", model_);
	} catch (std::ifstream::failure& e) {
		FAIL() << e.what();
	}

	ASSERT_EQ(model_.nodes.size(), 74183);
	ASSERT_EQ(model_.elements.size(), 66720);

	ASSERT_EQ(model_.nodes[0], fedes::Vector3<double>(-2, -0.0370841436, 0));
	ASSERT_EQ(model_.nodes[74182], fedes::Vector3<double>(1.41097403, -0.599422753, 1.5));

	ASSERT_EQ(model_.elements[0], std::vector<int>({71, 536, 41, 0, 2464, 2929, 2434, 2393}));
	ASSERT_EQ(model_.elements[30000], std::vector<int>({32586, 31411, 31410, 32583, 34979, 33804, 33803, 34976}));
	ASSERT_EQ(model_.elements[66719], std::vector<int>({69912, 69436, 69911, 71789, 72305, 71829, 72304, 74182}));
}

TEST_F(ParserTest, AbaqusOutputRead) {
	try {
		fedes::AbaqusInputRead("../../../models/Example1-ManufacturingProcessChain-2ndLoop/Process-1-Abaqus-Input.inp", model_);
		// Only contains stress and displacements
		fedes::AbaqusOutputRead("../../../models/Example1-ManufacturingProcessChain-2ndLoop/Process-1-Abaqus-Output.dat", model_);
	}
	catch (std::ifstream::failure& e) {
		FAIL() << e.what();
	}

	ASSERT_EQ(model_.stress.size(), 24543);
	ASSERT_EQ(model_.displacement.size(), 24543);

	ASSERT_EQ(model_.stress[3], std::vector<double>({ 1.295, 57.41, -5.615, -2.089, 22.53, 6.034}));
	ASSERT_EQ(model_.stress[24542], std::vector<double>({ 4.937,33.79, 0.1895,-13.14,-0.2374, 0.3043 }));

	ASSERT_EQ(model_.displacement[12000], std::vector<double>({-0.0040623, 0.0002119, -0.060071} ));
}

TEST_F(ParserTest, MorpheoInputRead) {
	try {
		fedes::MorpheoInputRead("../../../models/Example1-ManufacturingProcessChain-2ndLoop/Process2-Machining-XML-input.vtu", model_);
	}
	catch (std::ifstream::failure& e) {
		FAIL() << e.what();
	}
	ASSERT_EQ(model_.nodes.size(), 41637);
	ASSERT_EQ(model_.elements.size(), 154407);
	
	ASSERT_EQ(model_.nodes[0], fedes::Vector3<double>(232, 892, 27));
	ASSERT_EQ(model_.nodes[19000], fedes::Vector3<double>(216, 894.6055903, -25.64458887));
	ASSERT_EQ(model_.nodes[41636], fedes::Vector3<double>(227, 920, 0));

	ASSERT_EQ(model_.elements[0], std::vector<int>({5921, 16282, 16103, 16153}));
	ASSERT_EQ(model_.elements[100000], std::vector<int>({3782, 27775, 24940, 27693 }));
	ASSERT_EQ(model_.elements[154406], std::vector<int>({19004, 41564, 40473, 19074 }));
}

TEST_F(ParserTest, MorpheoInputOutputRead) {
	try {
		// Example contains no accumulated and total strain
		fedes::MorpheoInputOutputRead("../../../models/Example1-ManufacturingProcessChain-2ndLoop/Process1-HeatTreatment-XML-format.vtu", model_);
	}
	catch (std::ifstream::failure& e) {
		FAIL() << e.what();
	}
	ASSERT_EQ(model_.elements.size(), 83316);
	ASSERT_EQ(model_.nodes.size(), 24543);
	ASSERT_EQ(model_.stress.size(), 24543);
	ASSERT_EQ(model_.displacement.size(), 24543); 

	ASSERT_EQ(model_.nodes[24542], fedes::Vector3<double>({ 305.200439, 783.551758, -0.989621878 }));
	ASSERT_EQ(model_.elements[60000], std::vector<int>({8586, 17257, 8541, 17256}));

	ASSERT_EQ(model_.displacement[3], std::vector<double>({ 0.192687857932194, -0.411094398936378, 0.745349839557346 }));
	ASSERT_EQ(model_.displacement[24542], std::vector<double>({ 0.0761108402353308, -0.0440849347627563, 0.220086344388388 }));

	ASSERT_EQ(model_.stress[0], std::vector<double>({8.447,66.6, 2.618, -3.63, -26.6, -5.657}));
	ASSERT_EQ(model_.stress[1200], std::vector<double>({ -14.34, -1.49, 5.652, 0.1692, -2.791, -11.17 }));
}
