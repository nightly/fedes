#include <benchmark/benchmark.h>

#include <vector>

#include "fedes/indexing/octree/octree.h"
#include "fedes/model/model.h"
#include "fedes/maths/random.h"
#include "fedes/maths/Vector3.h"

class ConstructLargeFixture : public benchmark::Fixture {
public:
	fedes::Model source;
	std::vector<fedes::Vector3<double>> vec;
	thread_pool pool;

	ConstructLargeFixture() { // 1,000,000 nodes
		Iterations(10);
		UseRealTime();

		vec = fedes::GenerateRandomArray<double>(-1.00, 1.00, 1000000);
		source.nodes = vec;
	}
};

BENCHMARK_F(ConstructLargeFixture, Sequential)(benchmark::State& state) {
	for (auto _ : state) {
		fedes::Octree<double> octree(source.nodes, 15, 8);
		benchmark::DoNotOptimize(octree);
		benchmark::ClobberMemory();
	}
}

BENCHMARK_F(ConstructLargeFixture, Parallel)(benchmark::State& state) {
	for (auto _ : state) {
		fedes::Octree<double> octree(source.nodes, 15, 8, &pool);
		benchmark::DoNotOptimize(octree);
		benchmark::ClobberMemory();
	}
}

