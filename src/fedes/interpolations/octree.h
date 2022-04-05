#pragma once 

#include "fedes/indexing/mx-octree/octree.h"
#include "fedes/indexing/mx-octree/octant.h"
#include "fedes/model/model.h"

#include <concepts>

namespace fedes {

	template<typename T>
	void OctreeNearestPointMethod(const fedes::Octree<T>& octree, const fedes::Model& source_model, fedes::Model& target_model);

	template<typename T>
	void OctreeFieldOfPoints(const fedes::Octree<T>& octree, const fedes::Model& source_model, fedes::Model& target_model);

	template<typename T>
	void OctreeDistanceUsingElements(const fedes::Octree<T>& octree, const fedes::Model& source_model, fedes::Model& target_model);

	template<typename T>
	void OctreeElementShapeFunction(const fedes::Octree<T>& octree, const fedes::Model& source_model, fedes::Model& target_model);
}