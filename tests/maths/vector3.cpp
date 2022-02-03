#include "fedes/maths/vector3.hpp"
#include <gtest/gtest.h>


class Vector3Test : public ::testing::Test {
protected:
	fedes::Vector3<double> vec_;

	void SetUp() override {
		vec_ = fedes::Vector3<double>(5.0, 2.0, 10.5);
	}
};

TEST_F(Vector3Test, Equality) {
	fedes::Vector3<double> second_vec(5.0, 2.0, 10.5);
	ASSERT_EQ(vec_, second_vec);
}

TEST_F(Vector3Test, NotEqual) {
	fedes::Vector3<double> second_vec(4.99, 2.0, 10.5);
	ASSERT_NE(vec_, second_vec);
}

TEST_F(Vector3Test, ScalarMultiplication) {
	double scalar = 1.35;
	fedes::Vector3<double> result = vec_ * scalar;
	fedes::Vector3<double> expected(6.75, 2.7, 14.175);
	ASSERT_EQ(result, expected);
}

TEST_F(Vector3Test, ScalarDivision) {
	double scalar = 2;
	fedes::Vector3<double> result = vec_ / scalar;
	fedes::Vector3<double> expected(2.5, 1.0, 5.25);
	ASSERT_EQ(result, expected);
}