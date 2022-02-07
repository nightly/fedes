# FEDES 

![standard](https://img.shields.io/badge/c%2B%2B-23-blue.svg)
![ci.yml](https://github.com/nightly/fedes/actions/workflows/ci.yml/badge.svg)

## Build
```
git clone --recurse-submodules https://github.com/nightly/fedes
```

```
cmake -S . -B build
cmake --build build
```

To run the provided tests:
```
cd build
ctest -j 8 --output-on-failure --verbose
```