cmake_minimum_required (VERSION 3.8)

add_subdirectory("${PROJECT_SOURCE_DIR}/external/googletest" "external/googletest")

macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})

    target_compile_features(${TESTNAME}  PRIVATE cxx_std_23)
    if(MSVC)
        target_compile_options(${TESTNAME} PUBLIC "/Zc:__cplusplus") 
    endif()

    target_link_libraries(${TESTNAME} 
    PRIVATE 
        fedes
        gtest 
        gmock
        gtest_main)

    gtest_discover_tests(${TESTNAME}
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES 
    FOLDER tests
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/tests")
endmacro()

# Test Suite
package_add_test("vector3" "maths/vector3.cpp")
package_add_test("distance" "maths/distance.cpp")
package_add_test("z_ordering" "maths/z_ordering.cpp")
package_add_test("geometry" "maths/geometry.cpp")

package_add_test("parsers" "model/parsers.cpp")
package_add_test("writers" "model/writers.cpp")

package_add_test("strings" "common/strings.cpp")

package_add_test("octant" "indexing/octree/octant.cpp")
package_add_test("octree_construct" "indexing/octree/construct.cpp")
package_add_test("octree_construct_element" "indexing/octree/construct_element.cpp")
package_add_test("octree_construct_parallel" "indexing/octree/construct_parallel.cpp")
package_add_test("octree_traversals" "indexing/octree/traversals.cpp")
package_add_test("octree_post_order" "indexing/octree/post_order.cpp")
package_add_test("octree_find" "indexing/octree/find.cpp")
package_add_test("octree_nearest" "indexing/octree/nearest.cpp")
package_add_test("octree_field" "indexing/octree/field.cpp")
package_add_test("octree_radius" "indexing/octree/radius.cpp")
package_add_test("octree_comparator" "indexing/octree/octant_comparator.cpp")

package_add_test("octree_interpolations_npm" "interpolations/octree/npm.cpp")
package_add_test("octree_interpolations_fop" "interpolations/octree/fop.cpp")
package_add_test("octree_interpolations_dmue" "interpolations/octree/dmue.cpp")
package_add_test("octree_interpolations_esf" "interpolations/octree/esf.cpp")


