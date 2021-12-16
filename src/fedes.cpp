// fedes.cpp : Defines the entry point for the application.

#include <iostream>

#include "fedes/data/vector3.h"
#include "fedes/indexing/octree.h"

void t1() {
	fedes::Vector3<float> t0 = fedes::Vector3<float>(1.0f, 1.0f, 1.0f);
	fedes::Vector3<float> t1 = fedes::Vector3<float>(0.0f, 0.0f, 0.0f);
	fedes::Vector3<float> t2 = fedes::Vector3<float>(-1.0f, -1.0f, -1.0f);
	fedes::Vector3<float> f[3] = { t0, t1, t2 };

	// fedes::Octree<float> o = fedes::Octree<float>(f);

}

int main() {
	t1();
	std::cin.get();
	return 0;
}
