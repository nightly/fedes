#include <iostream>

#include "models.hpp"
#include "octree.hpp"


int main() {
	fedes::Model source, target;
	SetModels(source, target, 1);

	OctreeNPM(source, target);
	ExportModels(source, target, "oct-npm");



	return 0;
}