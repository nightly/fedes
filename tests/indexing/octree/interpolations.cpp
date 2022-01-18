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
		fedes::AnsysInputReadLis("../../models/Example1-Vane-big/Model1_Input-Ansys.txt", source);
		fedes::AnsysOutputRead("../../models/Example1-Vane-big/Model1_output-Ansys.txt", source);
		fedes::AbaqusInputRead("../../models/Example1-Vane-big/Model2-Input-Abaqus.inp", target);
	}
	catch (const std::ifstream::failure& e) {
		FAIL() << e.what();
	}
	fedes::Octree<double> octree(source.nodes, 8, 10);
	fedes::OctreeNearestPointMethod<double>(octree, source, target);
	ASSERT_EQ(target.displacement[24112], std::vector<double>({ 0.036185000000000002, 0.0056962999999999996, 0.47421999999999997 }));
	ASSERT_EQ(target.stress[100000], std::vector<double>({ 144810000.00000000, 429390000.00000000, 77178000.000000000, -78032000.000000000, -89415000.000000000, -36849000.000000000 }));
}