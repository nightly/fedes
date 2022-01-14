cmake_minimum_required (VERSION 3.8)

# FEDES Library
set(FEDES_SOURCES "maths/vector3.cpp" "maths/distance.cpp"  "model/model.cpp" "model/parsers.cpp" "model/writers.cpp"
"indexing/octree/octree.cpp" "indexing/octree/octant.cpp" "indexing/octree/traversals.cpp" "indexing/octree/interpolations.cpp"
"common/files.cpp" "common/strings.cpp" "instrumentation/timer.cpp")

add_library(fedes STATIC ${FEDES_SOURCES})

target_compile_features(fedes PRIVATE cxx_std_20)
if(MSVC)
    target_compile_options(fedes PUBLIC "/Zc:__cplusplus") # Reports __cplusplus version correctly for external libraries checking it
endif()

target_include_directories(fedes
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

# Dependencies 
add_subdirectory("${PROJECT_SOURCE_DIR}/external/matchit" "external/matchit")
target_link_libraries(fedes PRIVATE matchit)

# Preprocessor definitions
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND FEDES_RUN_TESTS)
    target_compile_definitions(fedes PRIVATE FEDES_BENCHMARKING=1)
endif()