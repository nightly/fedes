#include <gtest/gtest.h>
#include "fedes/indexing/mx-octree/interpolations.hpp"

#include <string>

#include "fedes/model/model.hpp"
#include "fedes/model/writers.hpp"
#include "fedes/model/parsers.hpp"
#include "fedes/indexing/mx-octree/octree.hpp"
#include "fedes/common/files.hpp"


TEST(OctreeInterpolations, NPM1) {
	fedes::Model source, target;
	try {
		fedes::AnsysInputReadLis("../../models/Example1-Vane-big/Model1_Input-Ansys.txt", source);
		fedes::AnsysOutputRead("../../models/Example1-Vane-big/Model1_output-Ansys.txt", source);
		fedes::AbaqusInputRead("../../models/Example1-Vane-big/Model2-Input-Abaqus.inp", target);
	} catch (const std::ifstream::failure& e) {
		FAIL() << e.what();
	}
	fedes::Octree<double> octree(source.nodes, 8, 10);
	target.SetTargetIndexes(source);
	fedes::OctreeNearestPointMethod<double>(octree, source, target);
	ASSERT_EQ(target.displacement[2], source.displacement[92]);
	ASSERT_EQ(target.displacement[14], source.displacement[2015]);
	// @Todo
	// ASSERT_EQ(target.displacement[205], std::vector<double>({ 0.012356000000000001, -0.0055199999999999997, 0.33613999999999999 }));
	ASSERT_EQ(target.displacement[25000], std::vector<double>({ 0.059610000000000003 , -0.028618000000000001,  0.44091999999999998 }));
	ASSERT_EQ(target.stress[100000], std::vector<double>({ 144810000.00000000, 429390000.00000000, 77178000.000000000, -78032000.000000000, -89415000.000000000, -36849000.000000000 }));
}