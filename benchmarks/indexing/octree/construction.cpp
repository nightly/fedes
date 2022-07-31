#include <benchmark/benchmark.h>

#include "fedes/indexing/octree/octree.h"
#include "fedes/model/model.h"
#include "fedes/model/examples.h"
#include "fedes/model/parsers.h"

/*
 * @param state.range(0): model ID
 * @param state.range(1): max depth
 * @param state.range(2): leaf split threshold
 */
static void BM_OctreeNodeIndex(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, state.range(0));

	for (auto _ : state) {
		fedes::Octree<double> octree(source.nodes, state.range(1), state.range(2));
		benchmark::DoNotOptimize(octree);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_OctreeNodeIndex)->Args({ 1, 15, 10 })->Iterations(1000)->Unit(benchmark::kMillisecond);

/*
 * @param state.range(0): model ID
 * @param state.range(1): max depth
 * @param state.range(2): leaf split threshold
 */
static void BM_OctreeElementIndex(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, state.range(0));

	for (auto _ : state) {
		fedes::Octree<double> octree(source.nodes, source.elements, state.range(1), state.range(2));
		benchmark::DoNotOptimize(octree);
		benchmark::ClobberMemory();
	}
}

// BENCHMARK(BM_OctreeElementIndex)->Args({ 1, 15, 10 })->Unit(benchmark::kMillisecond);

/*
 * @param state.range(0): model ID
 * @param state.range(1): max depth
 * @param state.range(2): leaf split threshold
 */
static void BM_OctreeParallelNodeIndex(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, state.range(0));
	BS::thread_pool pool;

	for (auto _ : state) {
		fedes::Octree<double> octree(source.nodes, state.range(1), state.range(2), &pool);
		benchmark::DoNotOptimize(octree);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_OctreeParallelNodeIndex)->Args({ 1, 15, 10 })->Iterations(1000)->Unit(benchmark::kMillisecond);

