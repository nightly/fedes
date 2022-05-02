#include "fedes/maths/element_type.h"

#include <vector>
#include <ostream>

namespace fedes {

	ElementType DetermineElementType(const std::vector<std::vector<size_t>>& elements) {
		size_t nodes_per_element = elements[0].size(); // This is uniform for a mesh
		switch (nodes_per_element) {
		case 4:
			return ElementType::Tetrahedron;
			break;
		case 6:
			return ElementType::Wedge;
			break;
		case 8:
			return ElementType::Hexahedron;
			break;
		}
	}

	std::ostream& operator<<(std::ostream& os, const ElementType& et) {
		switch (et) {
		case ElementType::Tetrahedron:
			os << "Tetrahedron";
			break;
		case ElementType::Wedge:
			os << "Wedge";
			break;
		case ElementType::Hexahedron:
			os << "Hexahedron";
			break;
		}
		return os;
	}

}