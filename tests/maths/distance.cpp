#include <gtest/gtest.h>
#include "fedes/maths/distance.h"

#include "fedes/maths/vector3.h"

TEST(Distance, EuclideanDistance) {
	fedes::Vector3<double> p1(3.2, 5.5, 9.9);
	fedes::Vector3<double> p2(6.5, 2.05, 1);
	double distance = fedes::Distance<double>(p1, p2);
	ASSERT_DOUBLE_EQ(distance, 10.099628706046575);
}

TEST(Distance, EuclideanDistanceZero) {
	fedes::Vector3<double> p1(2.5, 2.5, 2.5);
	fedes::Vector3<double> p2(2.5, 2.5, 2.5);
	double distance = fedes::Distance<double>(p1, p2);
	ASSERT_DOUBLE_EQ(distance, 0.0);
}
