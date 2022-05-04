#pragma once 

#include <thread_pool.hpp>

#include "fedes/model/model.h"
#include "fedes/indexing/octree/octree.h"

void OctreeNPM(fedes::Model& source, fedes::Model& target, size_t max_depth, size_t points_per_leaf,
	            thread_pool& pool);

void OctreeDMUFOP(fedes::Model& source, fedes::Model& target, size_t max_depth, size_t points_per_leaf,
	             double radius, thread_pool& pool);

void OctreeDMUE(fedes::Model& source, fedes::Model& target, size_t max_depth, size_t points_per_leaf,
	           size_t min_leaf_scan_dmue, thread_pool& pool);

void OctreeESF(fedes::Model& source, fedes::Model& target, size_t max_depth, size_t points_per_leaf,
	          size_t max_leaf_scans_threshold, thread_pool& pool);
