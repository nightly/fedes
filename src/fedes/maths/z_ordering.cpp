#include "fedes/maths/z_ordering.h"

#include <bitset>
#include <tuple>

#include <matchit.h>

#include "fedes/maths/vector3.h"

namespace fedes {

	/* @brief Determines the direction (0—7, Z-Ordering/Morton Encoding) of a point against a given origin point.
	 * 
	 *		3------7.
	 *		|`.    | `.
	 *		|  `2--+---6
	 *		|   |  |   |
	 *		1---+--5.  |
	 *		 `. |    `.|
	 *		   `0------4
	 * 
	 * @tparam T: point type, e.g. double, float
	 * @param origin: 3-dimensional point to use as the center
	 * @param p: 3-dimensional point to find direction code for respective to the given origin
	 * @returns directional code (0—7), based on Z-Ordering
	 */
	template <std::floating_point T>
	[[nodiscard]] uint_fast8_t DetermineDirection(const fedes::Vector3<T>& origin, const fedes::Vector3<T>& p) {
		using namespace matchit;

		bool x_larger = p.x >= origin.x;
		bool y_larger = p.y >= origin.y;
		bool z_larger = p.z >= origin.z;
		std::tuple<bool, bool, bool> tuple { x_larger, y_larger, z_larger };

		return match(tuple)(
			pattern | ds(true, true, true) = [] { return 7; },
			pattern | ds(true, true, false) = [] { return 6; },
			pattern | ds(true, false, true) = [] { return 5; },
			pattern | ds(true, false, false) = [] { return 4; },
			pattern | ds(false, true, true) = [] { return 3; },
			pattern | ds(false, true, false) = [] { return 2; },
			pattern | ds(false, false, true) = [] { return 1; },
			pattern | ds(false, false, false) = [] { return 0; }
		);
	}

	/* Explicit template instantiations */
	template uint_fast8_t DetermineDirection(const fedes::Vector3<double>& origin, const fedes::Vector3<double>& p);
	template uint_fast8_t DetermineDirection(const fedes::Vector3<float>& origin, const fedes::Vector3<float>& p);
}