#include <gtest/gtest.h>
#include "fedes/maths/z_ordering.hpp"

#include "fedes/maths/vector3.hpp"

class DetermineDirectionTest : public ::testing::Test {
protected:
	fedes::Vector3<double> center;

	void SetUp() override {
		fedes::Vector3<double> center(0.0, 0.0, 0.0);
	}
};

TEST_F(DetermineDirectionTest, ZOrder7) {
	fedes::Vector3<double> point(0.8, 0.7, 0.8);
	uint_fast8_t dir = fedes::DetermineDirection(center, point);
	ASSERT_EQ(dir, 7) << "Expected point to have Morton Encoding of index 7";
}

TEST_F(DetermineDirectionTest, ZOrder5) {
	fedes::Vector3<double> point(0.5, -0.1, 0.9);
	uint_fast8_t dir = fedes::DetermineDirection(center, point);
	ASSERT_EQ(dir, 5) << "Expected point to have Morton Encoding of index 5";
}