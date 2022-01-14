#include <gtest/gtest.h>
#include "fedes/indexing/octree/interpolations.hpp"

#include <string>

#include "fedes/model/model.hpp"
#include "fedes/model/writers.hpp"
#include "fedes/model/parsers.hpp"
#include "fedes/indexing/octree/octree.hpp"
#include "fedes/common/files.hpp"



TEST(OctreeInterpolations, NPM1) {
	fedes::Model source, target;
	try {
		fedes::AnsysInputReadLis("../../../models/Example1-Vane-big/Model1_Input-Ansys.txt", source);
		fedes::AnsysOutputRead("../../../models/Example1-Vane-big/Model1_output-Ansys.txt", source);
		fedes::AbaqusInputRead("../../models/Example1-Vane-big/Model2-Input-Abaqus.inp", target);
	}
	catch (const std::ifstream::failure& e) {
		FAIL() << e.what();
	}
	target.ResizeIndexes(source);

	fedes::Octree<double> octree(source.nodes, 8, 10);
	fedes::OctreeNearestPointMethod<double>(octree, source, target);

	try {
		fedes::CreateXML("Test_Octree_NPM1.vtu", target);
	}
	catch (const std::ofstream::failure& e) {
		FAIL() << e.what();
	}

	std::string got;
	std::string expected;
	try {
		got = fedes::StringFromFile("Test_Octree_NPM1.vtu");
		expected = fedes::StringFromFile("../../../tests/indexing/octree/testdata/Octree_NPM1.vtu");
	}
	catch (const std::ofstream::failure& e) {
		FAIL() << e.what();
	}
	// ASSERT_EQ(got, expected);
}

TEST(OctreeInterpolations, NPM2) {
	fedes::Model source, target;
	try {
		fedes::AbaqusInputRead("../../models/Example-3D-medium/model1-input-Abaqus.inp", source);
		fedes::AbaqusOutputRead("../../models/Example-3D-medium/model1-output-Abaqus.dat", source);
		fedes::AbaqusInputRead("../../models/Example-3D-medium/Model-input-Abaqus.inp", target);
	} catch (const std::ifstream::failure& e) {
		FAIL() << e.what();
	}
	target.ResizeIndexes(source);

	fedes::Octree<double> octree(source.nodes, 8, 10);
	fedes::OctreeNearestPointMethod<double>(octree, source, target);

	try {
		fedes::CreateXML("Test_Octree_NPM2.vtu", target);
	} catch (const std::ofstream::failure& e) {
		FAIL() << e.what();
	}

	std::string got;
	std::string expected;
	try {
		got = fedes::StringFromFile("Test_Octree_NPM2.vtu");
		expected = fedes::StringFromFile("../../../tests/indexing/octree/testdata/Octree_NPM2.vtu");
	}
	catch (const std::ofstream::failure& e) {
		FAIL() << e.what();
	}
	// ASSERT_EQ(got, expected);
}

TEST(OctreeInterpolations, NPM3) {
	fedes::Model source, target;
	try {
		fedes::MorpheoInputOutputRead("../../models/Example1-ManufacturingProcessChain-2ndLoop/Process1-HeatTreatment-XML-format.vtu", source);
		fedes::AbaqusInputRead("../../models/Example1-ManufacturingProcessChain-2ndLoop/Process2-ShotPeening-Abaqus.inp", target);
	}
	catch (const std::ifstream::failure& e) {
		FAIL() << e.what();
	}
	target.ResizeIndexes(source);

	fedes::Octree<double> octree(source.nodes, 10, 15);
	fedes::OctreeNearestPointMethod<double>(octree, source, target);

	try {
		fedes::CreateXML("Test_Octree_NPM3.vtu", target);
	}
	catch (const std::ofstream::failure& e) {
		FAIL() << e.what();
	}

	std::string got;
	std::string expected;
	try {
		got = fedes::StringFromFile("Test_Octree_NPM3.vtu");
		expected = fedes::StringFromFile("../../../tests/indexing/octree/testdata/Octree_NPM3.vtu");
	}
	catch (const std::ofstream::failure& e) {
		FAIL() << e.what();
	}
	// ASSERT_EQ(got, expected);
}