#include <benchmark/benchmark.h>

#include "fedes/indexing/octree/octree.h"
#include "fedes/model/model.h"
#include "fedes/model/examples.h"
#include "fedes/model/parsers.h"

static void BM_OctreeNodeIndex_Model1(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 1);

	for (auto _ : state) {
		fedes::Octree<double> octree(source.nodes, 15, 10);
		benchmark::DoNotOptimize(octree);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_OctreeNodeIndex_Model1)->Iterations(1000);

static void BM_OctreeElementIndex_Model1(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 1);

	for (auto _ : state) {
		fedes::Octree<double> octree(source.nodes, source.elements, 10, 8);
		benchmark::DoNotOptimize(octree);
		benchmark::ClobberMemory();
	}
}

// BENCHMARK(BM_OctreeElementIndex_Model1);

static void BM_OctreeNodeIndex_Model2(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 2);

	for (auto _ : state) {
		fedes::Octree<double> octree(source.nodes, 15, 10);
		benchmark::DoNotOptimize(octree);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_OctreeNodeIndex_Model2)->Iterations(1000);

static void BM_OctreeElementIndex_Model2(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 2);

	for (auto _ : state) {
		fedes::Octree<double> octree(source.nodes, source.elements, 8, 10);
		benchmark::DoNotOptimize(octree);
		benchmark::ClobberMemory();
	}
}

// BENCHMARK(BM_OctreeElementIndex_Model2);