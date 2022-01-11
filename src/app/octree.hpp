#pragma once 

#include "fedes/model/model.hpp"

// @Todo: BuildOctree function, just for timing separately to interpolations

void OctreeNPM(fedes::Model& source, fedes::Model& target);

void OctreeDMUFOP(fedes::Model& source, fedes::Model& target);

void OctreeDMUE(fedes::Model& source, fedes::Model& target);

void OctreeESF(fedes::Model& source, fedes::Model& target);
