#include <gtest/gtest.h>
#include "fedes/indexing/octree/octant_comparator.h"

#include "fedes/indexing/octree/octant.h"
#include "fedes/maths/vector3.h"

class OctantComparatorTest : public ::testing::Test {
protected:
	std::unique_ptr<fedes::internal::Octant<double>> octant_a;
	std::unique_ptr<fedes::internal::Octant<double>> octant_b;

	void SetUp() override {
		fedes::Vector3<double> center_a(0.0, 0.0, 0.0);
		fedes::Vector3<double> extent_a(0.25, 0.25, 0.25);
		octant_a = std::make_unique<fedes::internal::Octant<double>>(center_a, extent_a, nullptr);

		fedes::Vector3<double> center_b(0.75, 0.75, 0.75);
		fedes::Vector3<double> extent_b(0.25, 0.25, 0.25);
		octant_b = std::make_unique<fedes::internal::Octant<double>>(center_b, extent_b, nullptr);
	}
};

TEST_F(OctantComparatorTest, Comparator) {

}