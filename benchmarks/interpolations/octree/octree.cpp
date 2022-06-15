#include <benchmark/benchmark.h>
#include "fedes/interpolations/octree/octree.h"

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
	fedes::Octree<double> octree(source.nodes, 15, 10);
	thread_pool pool;

	for (auto _ : state) {
		fedes::NearestPointMethod(octree, source, target);
		benchmark::DoNotOptimize(target);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_Octree_NPM)->Arg(1)->Iterations(1000)->Unit(benchmark::kMillisecond);

/*
 * @param state.range(0): model ID
 * @param state.range(1): max depth
 * @param state.range(2): leaf split threshold
 * @param state.range(3): radius
 */
static void BM_Octree_FOP(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, state.range(0));
	fedes::Octree<double> octree(source.nodes, 15, 10);
	thread_pool pool;

	for (auto _ : state) {
		fedes::ParallelFieldOfPoints(octree, source, target, pool, 10.00);
		benchmark::DoNotOptimize(target);
		benchmark::ClobberMemory();
	}
}

// BENCHMARK(BM_Octree_FOP)->Arg(1)->Iterations(1000)->Unit(benchmark::kMillisecond);

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
	thread_pool pool;

	for (auto _ : state) {
		fedes::NearestPointMethod(octree, source, target);
		benchmark::DoNotOptimize(target);
		benchmark::ClobberMemory();
	}
}

// BENCHMARK(BM_Octree_DMUE)->Arg(1)->Iterations(1000)->Unit(benchmark::kMillisecond);

/*
 * @param state.range(0): model ID
 * @param state.range(1): max depth
 * @param state.range(2): leaf split threshold
 * @param state.range(3): maximum number of leaves to scan
 */
static void BM_Octree_ESF(benchmark::State& state) {
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, state.range(0));
	fedes::Octree<double> octree(source.nodes, source.elements, 15, 10);
	thread_pool pool;

	for (auto _ : state) {
		fedes::ParallelElementShapeFunction(octree, source, target, pool, 2000);
		benchmark::DoNotOptimize(target);
		benchmark::ClobberMemory();
	}
}

// BENCHMARK(BM_Octree_ESF)->Arg(1)->Iterations(1000)->Unit(benchmark::kMillisecond);