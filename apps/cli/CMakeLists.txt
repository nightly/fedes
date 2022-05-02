cmake_minimum_required (VERSION 3.8)

# FEDES Application/Executable
add_executable(app_fedes_cli "fedes.cpp"  "octree.cpp" )

target_compile_features(app_fedes_cli PRIVATE cxx_std_23)
if(MSVC)
    target_compile_options(app_fedes_cli PUBLIC "/Zc:__cplusplus") 
endif()

# Dependencies
target_link_libraries(app_fedes_cli 
    PRIVATE 
        fedes
        spdlog::spdlog
        thread_pool
)

# Directories
set_target_properties(app_fedes_cli
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
)