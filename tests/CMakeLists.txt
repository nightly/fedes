cmake_minimum_required (VERSION 3.8)

add_subdirectory("${PROJECT_SOURCE_DIR}/external/googletest" "external/googletest")
 
macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})

    target_compile_features(${TESTNAME}  PRIVATE cxx_std_20)
    if(MSVC)
        target_compile_options(${TESTNAME} PUBLIC "/Zc:__cplusplus") 
    endif()

    target_link_libraries(${TESTNAME} 
    PRIVATE 
        fedes
        gtest 
        gtest_main)

    gtest_discover_tests(${TESTNAME}
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

package_add_test(dynamic_array interop/dynamic_array.cpp)

package_add_test(octant indexing/octree/octant.cpp)
package_add_test(octant indexing/octree/octree.cpp)
