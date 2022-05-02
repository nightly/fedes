#include <benchmark/benchmark.h>

#include <vector>

#include "fedes/indexing/octree/octree.h"
#include "fedes/model/model.h"
#include "fedes/maths/random.h"
#include "fedes/maths/Vector3.h"

static void BM_OctreeNodeIndexMillion(benchmark::State& state) {
	fedes::Model source, target;
	std::vector<fedes::Vector3<double>> vec = fedes::GenerateRandomArray<double>(-1.00, 1.00, 1000000);
	source.nodes = vec;

	for (auto _ : state) {
		fedes::Octree<double> octree(source.nodes, 15, 8);
		benchmark::DoNotOptimize(octree);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_OctreeNodeIndexMillion); // 0.7