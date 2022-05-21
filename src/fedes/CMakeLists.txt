cmake_minimum_required (VERSION 3.22)

# FEDES Library
set(FEDES_SOURCES 

"maths/vector3.cpp"  "maths/z_ordering.cpp" "maths/distance.h" "maths/random.h"
"maths/element_type.cpp" "maths/geometry.h"

"model/model.cpp" "model/parsers.cpp" "model/writers.cpp" "model/export.cpp" "model/examples.cpp"

"indexing/octree/octree.h" "indexing/octree/octant.h" "indexing/octree/traversals.h" 
"indexing/octree/octant_comparator.h"

"interpolations/octree/octree.h" "interpolations/octree/npm.h" "interpolations/octree/fop.h"
"interpolations/octree/dmue.h" "interpolations/octree/esf.h"

"instrumentation/timer.cpp" 

"common/files.cpp" "common/strings.cpp" "common/log.h"
)

add_library(fedes STATIC ${FEDES_SOURCES})

target_compile_features(fedes PRIVATE cxx_std_23)
if(MSVC)
    target_compile_options(fedes PUBLIC "/Zc:__cplusplus") # Reports __cplusplus version correctly for external libraries checking it
endif()

target_include_directories(fedes
    PUBLIC 
        $<INSTALL_INTERFACE:src>    
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

# Dependencies 
add_subdirectory("${PROJECT_SOURCE_DIR}/external/matchit" "external/matchit")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/spdlog" "external/spdlog")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/thread-pool" "external/thread-pool")

target_link_libraries(fedes
    PRIVATE 
        matchit
    PUBLIC
        thread_pool
        spdlog::spdlog
)

# Directories
set_target_properties(fedes
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
)