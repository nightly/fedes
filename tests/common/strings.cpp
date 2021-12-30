#include "fedes/common/strings.hpp"
#include <gtest/gtest.h>

#include <string>
#include <vector>

TEST(Strings, Trim) {
	std::string string("  ..     ");
	ASSERT_EQ(fedes::Trim(string), "..");
	std::string only_spaces("         ");
	ASSERT_EQ(fedes::Trim(only_spaces), "");
}

TEST(Strings, SplitString) {
	std::string string("1    313.50      70.000      570.37");
	std::vector<std::string> splits;
	int result = fedes::StringSplit(splits, string, ' ');
	ASSERT_EQ(result, 17);
	ASSERT_EQ(splits[16], "570.37");
}

TEST(Strings, ToLower) {
	std::string string = "StrInG";
	std::string expected = "string";
	ASSERT_EQ(fedes::ToLower(string), expected);
	ASSERT_EQ(string, string);
}