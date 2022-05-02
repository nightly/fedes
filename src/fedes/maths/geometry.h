#pragma once

#include <concepts>
#include <span>
#include <vector>
#include <optional>

#include "fedes/maths/vector3.h"
#include "fedes/maths/distance.h"
#include "fedes/common/log.h"

namespace fedes {

	struct MuesfData {
	public:
		double g;
		double h;
		double r;
	};

	/*
	 * @port From FEDES v2 
	 */
	inline std::optional<MuesfData> TetrahedronContainment(const std::span<fedes::Vector3<double>>& source_nodes, const std::vector<size_t>& element,
		                                                   const fedes::Vector3<double> query_point, double g3, double h3, double r3, double g_h_r3) {
		double xn = query_point.x;
		double yn = query_point.y;
		double zn = query_point.z;
		double cx1cx0 = source_nodes[element[1]].x - source_nodes[element[0]].x;
		double cy2cy0 = source_nodes[element[2]].y - source_nodes[element[0]].y;
		double cz3cz0 = source_nodes[element[3]].z - source_nodes[element[0]].z;
		double cy3cy0 = source_nodes[element[3]].y - source_nodes[element[0]].y;
		double cz2cz0 = source_nodes[element[2]].z - source_nodes[element[0]].z;
		double cy1cy0 = source_nodes[element[1]].y - source_nodes[element[0]].y;
		double cx2cx0 = source_nodes[element[2]].x - source_nodes[element[0]].x;
		double cx3cx0 = source_nodes[element[3]].x - source_nodes[element[0]].x;
		double cz1cz0 = source_nodes[element[1]].z - source_nodes[element[0]].z;
		double xncx0 = xn - source_nodes[element[0]].x;
		double yncy0 = yn - source_nodes[element[0]].y;
		double zncz0 = zn - source_nodes[element[1]].z;

	    double det = cx1cx0 * cy2cy0 * cz3cz0 - cx1cx0 * cy3cy0 * cz2cz0 - cy1cy0 * cx2cx0 * cz3cz0 +
		cy1cy0 *
		cx3cx0 *
		cz2cz0 + cz1cz0 * cx2cx0 * cy3cy0 - cz1cz0 * cx3cx0 * cy2cy0;
	    
		double g = (xncx0 * cy2cy0 * cz3cz0 - xncx0 * cy3cy0 * cz2cz0 - yncy0 * cx2cx0 * cz3cz0 + yncy0 *
		cx3cx0
		* cz2cz0
		+ zncz0 * cx2cx0 * cy3cy0 - zncz0 * cx3cx0 * cy2cy0) / det;
		
		double h  = (cx1cx0 * yncy0 * cz3cz0 - cx1cx0 * cy3cy0 * zncz0 - cy1cy0 * xncx0 * cz3cz0 + cy1cy0 *
		cx3cx0 * zncz0
		+ cz1cz0 * xncx0 * cy3cy0 - cz1cz0 * cx3cx0 * yncy0) / det;
	    
		double r =  (cx1cx0 * cy2cy0 * zncz0 - cx1cx0 * yncy0 * cz2cz0 - cy1cy0 * cx2cx0 * zncz0 + cy1cy0 *
		xncx0
		* cz2cz0
		+ cz1cz0 * cx2cx0 * yncy0 - cz1cz0 * xncx0 * cy2cy0) / det;

		if ((g > g3) && (h > h3) && (r > r3) && (g + h + r < g_h_r3)) {
			FEDES_INFO("[Geometry] Query point {} is contained inside Tetrahedron Element of [1] ({}) [2] ({}) [3] ({}) [4] ({})", query_point,
				source_nodes[element[0]], source_nodes[element[1]], source_nodes[element[2]], source_nodes[element[3]]);
			return { MuesfData{.g = g, .h = h, .r = r} };
		} else {
			return {};
		}

	}

	/*
	 * @port From FEDES v2
	 */
	inline std::optional<MuesfData> WedgeContainment(const std::span<fedes::Vector3<double>>& source_nodes, const std::vector<size_t>& elements,
		                                             const fedes::Vector3<double> query_point, double g4, double h4, double r4, 
		                                             double gg4, double hh4, double rr4) {
		
		const double& cx1 = source_nodes[elements[0]].x;
		const double& cx2 = source_nodes[elements[1]].x;
		const double& cx3 = source_nodes[elements[2]].x;
		const double& cx4 = source_nodes[elements[3]].x;
		const double& cx5 = source_nodes[elements[4]].x;
		const double& cx6 = source_nodes[elements[5]].x;
		const double& cy1 = source_nodes[elements[0]].y;
		const double& cy2 = source_nodes[elements[1]].y;
		const double& cy3 = source_nodes[elements[2]].y;
		const double& cy4 = source_nodes[elements[3]].y;
		const double& cy5 = source_nodes[elements[4]].y;
		const double& cy6 = source_nodes[elements[5]].y;
		const double& cz1 = source_nodes[elements[0]].z;
		const double& cz2 = source_nodes[elements[1]].z;
		const double& cz3 = source_nodes[elements[2]].z;
		const double& cz4 = source_nodes[elements[3]].z;
		const double& cz5 = source_nodes[elements[4]].z;
		const double& cz6 = source_nodes[elements[5]].z;

		double f1 = -(0.25 * (cx2 + cx3) + 0.75 * (cx5 + cx6) - 2 * query_point.x);
		double f2 = -(0.25 * (cy2 + cy3) + 0.75 * (cy5 + cy6) - 2 * query_point.y);
		double f3 = -(0.25 * (cz2 + cz3) + 0.75 * (cz5 + cz6) - 2 * query_point.z);
		double f1g = -cx1 + cx2 - cx4 + cx5;
		double f1h = -cx1 + cx3 - cx4 + cx6;
		double f1r = 0.5 * (-cx2 - cx3 + cx5 + cx6);
		double f2g = -cy1 + cy2 - cy4 + cy5;
		double f2h = -cy1 + cy3 - cy4 + cy6;
		double f2r = 0.5 * (-cy2 - cy3 + cy5 + cy6);
		double f3g = -cz1 + cz2 - cz4 + cz5;
		double f3h = -cz1 + cz3 - cz4 + cz6;
		double f3r = 0.5 * (-cz2 - cz3 + cz5 + cz6);
		double det = f1g * f2h * f3r - f1g * f2r * f3h - f2g * f1h * f3r + f2g * f1r * f3h + f3g * f1h * f2r - f3g * f1r * f2h;
		double g = (f1 * f2h * f3r - f1 * f2r * f3h - f2 * f1h * f3r + f2 * f1r * f3h + f3 * f1h * f2r - f3 * f1r * f2h) / det;
		double h = (f1g * f2 * f3r - f1g * f2r * f3 - f2g * f1 * f3r + f2g * f1r * f3 + f3g * f1 * f2r - f3g * f1r * f2) / det;
		double r = (f1g * f2h * f3 - f1g * f2 * f3h - f2g * f1h * f3 + f2g * f1 * f3h + f3g * f1h * f2 - f3g * f1 * f2h) / det;


		if ((g > g4) && (g < gg4) && (h > h4) && (h < hh4) && (r > r4) && (r < rr4)) {
			FEDES_INFO("[Geometry] Found query point {} contained within Wedge Element element with nodes of:", query_point);
			return { MuesfData{.g = g, .h = h, .r = r} };
		}
		else {
			return {};
		}

	}

	/*
	 * @port From FEDES v2
	 */
	inline std::optional<MuesfData> HexahedronContainment(const std::span<fedes::Vector3<double>>& source_nodes, const std::vector<size_t>& elements,
		                                                  const fedes::Vector3<double> query_point, double g5, double h5, double r5,
		                                                  double gg5, double hh5, double rr5) {
		const double& cx1 = source_nodes[elements[0]].x;
		const double& cx2 = source_nodes[elements[1]].x;
		const double& cx3 = source_nodes[elements[2]].x;
		const double& cx4 = source_nodes[elements[3]].x;
		const double& cx5 = source_nodes[elements[4]].x;
		const double& cx6 = source_nodes[elements[5]].x;
		const double& cx7 = source_nodes[elements[6]].x;
		const double& cx8 = source_nodes[elements[7]].x;
		const double& cy1 = source_nodes[elements[0]].y;
		const double& cy2 = source_nodes[elements[1]].y;
		const double& cy3 = source_nodes[elements[2]].y;
		const double& cy4 = source_nodes[elements[3]].y;
		const double& cy5 = source_nodes[elements[4]].y;
		const double& cy6 = source_nodes[elements[5]].y;
		const double& cy7 = source_nodes[elements[6]].y;
		const double& cy8 = source_nodes[elements[7]].y;
		const double& cz1 = source_nodes[elements[0]].z;
		const double& cz2 = source_nodes[elements[1]].z;
		const double& cz3 = source_nodes[elements[2]].z;
		const double& cz4 = source_nodes[elements[3]].z;
		const double& cz5 = source_nodes[elements[4]].z;
		const double& cz6 = source_nodes[elements[5]].z;
		const double& cz7 = source_nodes[elements[6]].z;
		const double& cz8 = source_nodes[elements[7]].z;

		double f1 = -cx1 - cx2 - cx3 - cx4 - cx5 - cx6 - cx7 - cx8 + 8 * query_point.x;
		double f2 = -cy1 - cy2 - cy3 - cy4 - cy5 - cy6 - cy7 - cy8 + 8 * query_point.y;
		double f3 = -cz1 - cz2 - cz3 - cz4 - cz5 - cz6 - cz7 - cz8 + 8 * query_point.z;
		double f1g = -cx1 + cx2 + cx3 - cx4 - cx5 + cx6 + cx7 - cx8;
		double f1h = -cx1 - cx2 + cx3 + cx4 - cx5 - cx6 + cx7 + cx8;
		double f1r = -cx1 - cx2 - cx3 - cx4 + cx5 + cx6 + cx7 + cx8;
		double f2g = -cy1 + cy2 + cy3 - cy4 - cy5 + cy6 + cy7 - cy8;
		double f2h = -cy1 - cy2 + cy3 + cy4 - cy5 - cy6 + cy7 + cy8;
		double f2r = -cy1 - cy2 - cy3 - cy4 + cy5 + cy6 + cy7 + cy8;
		double f3g = -cz1 + cz2 + cz3 - cz4 - cz5 + cz6 + cz7 - cz8;
		double f3h = -cz1 - cz2 + cz3 + cz4 - cz5 - cz6 + cz7 + cz8;
		double f3r = -cz1 - cz2 - cz3 - cz4 + cz5 + cz6 + cz7 + cz8;
		double det = f1g * f2h * f3r - f1g * f2r * f3h - f2g * f1h * f3r + f2g * f1r * f3h + f3g * f1h * f2r - f3g * f1r * f2h;
		double g = (f1 * f2h * f3r - f1 * f2r * f3h - f2 * f1h * f3r + f2 * f1r * f3h + f3 * f1h * f2r - f3 * f1r * f2h) / det;
		double h = (f1g * f2 * f3r - f1g * f2r * f3 - f2g * f1 * f3r + f2g * f1r * f3 + f3g * f1 * f2r - f3g * f1r * f2) / det;
		double r = (f1g * f2h * f3 - f1g * f2 * f3h - f2g * f1h * f3 + f2g * f1 * f3h + f3g * f1h * f2 - f3g * f1 * f2h) / det;

		if ((g > g5) && (g < gg5) && (h > h5)  && (h < hh5) &&  (r > r5) &&  (r < rr5)) {
			FEDES_INFO("[Geometry] Found query point {} contained within Hexahedron Element with nodes of: [1] ({}) [2] ({}) [3] ({}) [4] ({}) [5] ({}) [6] ({}) [7] ({}) [8] ({})", 
				query_point, source_nodes[elements[0]], source_nodes[elements[1]], source_nodes[elements[2]], source_nodes[elements[3]], source_nodes[elements[4]],
				source_nodes[elements[5]], source_nodes[elements[6]], source_nodes[elements[7]]);
			return { MuesfData{.g = g, .h = h, .r = r} };
		} else {
			return {};
		}

	}

	/*
	 * @brief Returns the average distance for a query point against all of a given element's nodes.
	 * This can then be used to decide which element a node belongs to for DMUE.
	 * @param elements: corresponds to the 2D inner array of elements, containing only the nodes for an element
	 */
	template <std::floating_point T = double>
	T AverageDistanceSq(const std::vector<size_t>& elements, const std::span<Vector3<T>>& point_cloud, const Vector3<T>& query) {
		T total = 0.00;
		for (const auto& n : elements) {
			total += fedes::DistanceSquared(query, point_cloud[n]);
		}
		return (total / elements.size());
	}
}