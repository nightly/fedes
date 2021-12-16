#include <gtest/gtest.h>
#include "fedes/interop/dynamic_array.h"

#include <vector>

TEST(Dyn1DArrayAsVec, HandlesBuffer) {
	std::vector<double> test = { 1.0, 2.0, 5.5 };
	std::vector<double> result = fedes::Dyn1DArrayAsVec(&test[0], test.size());
	std::vector<double> expected = { 1.0, 2.0, 5.5 };
	EXPECT_EQ(result, expected);
}