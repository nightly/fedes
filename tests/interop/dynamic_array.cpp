
#include "fedes/interop/dynamic_array.hpp"

#include <vector>

#include <gtest/gtest.h>

TEST(DynArrayTest, 1D) {
	std::vector<double> test = { 1.0, 2.0, 5.5 };
	std::vector<double> result = fedes::Dyn1DArrayAsVec<double>(&test[0], test.size());
	std::vector<double> expected = { 1.0, 2.0, 5.5 };
	EXPECT_EQ(result, expected);
}