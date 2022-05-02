#pragma once

#include <vector>
#include <ostream>

namespace fedes {
	
	// @brief ElementType considers the 3 distinct possibilities of the 3D element types supported
	enum class ElementType : unsigned int {
		Tetrahedron = 0, // 4 nodes
		Wedge = 1, // 6 nodes
		Hexahedron, // 8 nodes
	};

	ElementType DetermineElementType(const std::vector<std::vector<size_t>>& elements);
	std::ostream& operator<<(std::ostream& os, const ElementType& et);
}