#include <benchmark/benchmark.h>
#include "fedes/interpolations/octree/octree.h"

#include <BS_thread_pool.hpp>

#include "fedes/indexing/octree/octree.h"
#include "fedes/model/model.h"
#include "fedes/model/examples.h"

/*
 * @param state.range(0): model ID 
 * @param state.range(1): max depth
 * @param state.range(2): leaf split threshold
 */
static void BM_Octree_NPM(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, state.range(0));
	fedes::Octree<double> octree(source.nodes, state.range(1), state.range(2));
	BS::thread_pool pool;

	for (auto _ : state) {
		fedes::ParallelNearestPointMethod(octree, source, target, pool);
		benchmark::DoNotOptimize(target);
		benchmark::ClobberMemory();
	}
}

// BENCHMARK(BM_Octree_NPM)->Args({1, 15, 10})->Iterations(10)->Unit(benchmark::kMillisecond);

/*
 * @param state.range(0): model ID
 * @param state.range(1): max depth
 * @param state.range(2): leaf split threshold
*  radius set manually, otherwise use BENCHMARK_CAPTURE.
 */
static void BM_Octree_FOP(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, state.range(0));
	fedes::Octree<double> octree(source.nodes, state.range(1), state.range(2));
	BS::thread_pool pool;

	for (auto _ : state) {
		fedes::ParallelFieldOfPoints(octree, source, target, pool, 10.00);
		benchmark::DoNotOptimize(target);
		benchmark::ClobberMemory();
	}
}

// BENCHMARK(BM_Octree_FOP)->Args({1, 15, 10})->Iterations(1000)->Unit(benchmark::kMillisecond);

/*
 * @param state.range(0): model ID
 * @param state.range(1): max depth
 * @param state.range(2): leaf split threshold
 * @param state.range(3): minimum number of elements to scan
 */
static void BM_Octree_DMUE(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, state.range(0));
	fedes::Octree<double> octree(source.nodes, source.elements, 15, 10);
	BS::thread_pool pool;

	for (auto _ : state) {
		fedes::ParallelDMUE(octree, source, target, pool, state.range(3));
		benchmark::DoNotOptimize(target);
		benchmark::ClobberMemory();
	}
}

// BENCHMARK(BM_Octree_DMUE)->Args({1, 15, 10, 25})->Iterations(1000)->Unit(benchmark::kMillisecond);

/*
 * @param state.range(0): model ID
 * @param state.range(1): max depth
 * @param state.range(2): leaf split threshold
 * @param state.range(3): maximum number of leaves to scan
 */
static void BM_Octree_ESF(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, state.range(0));
	fedes::Octree<double> octree(source.nodes, source.elements, state.range(1), state.range(2));
	BS::thread_pool pool;

	for (auto _ : state) {
		fedes::ParallelElementShapeFunction(octree, source, target, pool, state.range(3));
		benchmark::DoNotOptimize(target);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_Octree_ESF)->Args({1, 15, 10, 1000})->Iterations(1000)->Unit(benchmark::kMillisecond);