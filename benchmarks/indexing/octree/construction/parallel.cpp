#include <benchmark/benchmark.h>

#include <thread_pool.hpp>

#include "fedes/indexing/octree/octree.h"
#include "fedes/model/model.h"
#include "fedes/model/examples.h"
#include "fedes/model/parsers.h"

static void BM_OctreeParallelNodeIndex_Model1(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 1);
	thread_pool pool;

	for (auto _ : state) {
		fedes::Octree<double> octree(source.nodes, 15, 10, &pool);
		benchmark::DoNotOptimize(octree);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_OctreeParallelNodeIndex_Model1)->Iterations(1000);

static void BM_OctreeParallelElementIndex_Model1(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 1);
	thread_pool pool;

	for (auto _ : state) {
		fedes::Octree<double> octree(source.nodes, 15, 10, &pool);
		benchmark::DoNotOptimize(octree);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_OctreeParallelElementIndex_Model1)->Iterations(1000);