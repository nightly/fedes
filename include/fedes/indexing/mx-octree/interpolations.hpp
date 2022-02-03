#pragma once 

#include "fedes/indexing/mx-octree/octree.hpp"
#include "fedes/indexing/mx-octree/octant.hpp"
#include "fedes/model/model.hpp"

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