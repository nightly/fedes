#pragma once 

#include "fedes/model/model.h"
#include "fedes/indexing/mx-octree/octree.h"

fedes::Octree<double> OctreeBuild(fedes::Model& source, size_t points_per_leaf, size_t max_depth);

void OctreeNPM(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth);
void OctreeDMUFOP(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth);
void OctreeDMUE(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth);
void OctreeESF(fedes::Model& source, fedes::Model& target, size_t points_per_leaf, size_t max_depth);
