#pragma once 

#include "fedes/model/model.hpp"
#include "fedes/indexing/octree/octree.hpp"

void OctreeNPM(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth);

void OctreeDMUFOP(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth);

void OctreeDMUE(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth);

void OctreeESF(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth);
