#include "fedes/maths/vector3.hpp"

#include <vector>

namespace fedes {

	struct Model {
		std::vector<Vector3> nodes;
		std::vector<std::vector<int>> elements;
		std::vector<Vector3> displacement;
		std::vector<Vector3> stress;
		std::vector<Vector3> total_strain;
		std::vector<Vector3> plastic_strain;
		std::vector<Vector3> accumulated_strain;
	};

}