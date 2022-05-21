# FEDES 

![standard](https://img.shields.io/badge/c%2B%2B-23-blue.svg)
![ci.yml](https://github.com/nightly/fedes/actions/workflows/ci.yml/badge.svg)

## Build instructions

### Requirements
- [CMake](https://cmake.org/) (>=3.22)
- [Git](https://git-scm.com/) (for submodule cloning)
- C++23 compiler

### Cloning with submodules 
```
git clone --recurse-submodules https://github.com/nightly/fedes
```

### CMake build process
```
cmake -S . -B build
cmake --build build
```

To run the provided tests:
```
cd build
ctest --output-on-failure --verbose
```

## Example usage
```cpp
fedes::Model source, target;
thread_pool pool(std::thread::hardware_concurrency());
fedes::AnsysInputReadLis("../../models/Example1-Vane-big/Model1_Input-Ansys.txt", source);
fedes::AnsysOutputRead("../../models/Example1-Vane-big/Model1_output-Ansys.txt", source);
fedes::AbaqusInputRead("../../models/Example1-Vane-big/Model2-Input-Abaqus.inp", target);
target.SetTargetIndexes(source);

fedes::Octree<double> octree(source.nodes, 8, 10);
fedes::ParallelNearestPointMethod(octree, source, target, pool);
```

## Layout 
- `apps`: contains executable applications 
- `benchmarks`: contains indexing and interpolations benchmarks 
- `external`: external libraries & dependencies as Git submodules 
- `models`: stores sample FEA input & output meshes for testing purposes 
- `src/fedes`: contains library code 
- `tests`: contains tests alongside test data
