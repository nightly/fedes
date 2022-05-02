#include <benchmark/benchmark.h>
#include "fedes/interpolations/octree/octree.h"

#include "fedes/indexing/octree/octree.h"
#include "fedes/model/model.h"
#include "fedes/model/examples.h"

static void BM_OctreeNPM_Model1(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 1);
	fedes::Octree<double> octree(source.nodes, 8, 10);

	for (auto _ : state) {
		fedes::NearestPointMethod(octree, source, target);
		benchmark::DoNotOptimize(target);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_OctreeNPM_Model1)->Iterations(1000);

static void BM_OctreeNPM_Model2(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 2);
	fedes::Octree<double> octree(source.nodes, 8, 10);

	for (auto _ : state) {
		fedes::NearestPointMethod(octree, source, target);
		benchmark::DoNotOptimize(target);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_OctreeNPM_Model2)->Iterations(1000);

