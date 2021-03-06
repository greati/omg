cmake_minimum_required(VERSION 3.13)

project(omg VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

set(YAML_CPP_BUILD_TESTS CACHE BOOL OFF)

# deps
#-----------------------------------------#
FetchContent_Declare(yamlcpp
    GIT_REPOSITORY  https://github.com/jbeder/yaml-cpp
    GIT_TAG         562aefc114938e388457e6a531ed7b54d9dc1b62 # 0.6.2
)
FetchContent_GetProperties(yamlcpp)
if (NOT yamlcpp_POPULATED)
    FetchContent_Populate(yamlcpp)
    add_subdirectory(${yamlcpp_SOURCE_DIR} ${yamlcpp_BINARY_DIR})
endif()

# define build types and configurations
# --------------------------------------- #
if (CMAKE_CONFIGURATION_TYPES)
    if (NOT "Test" IN_LIST CMAKE_CONFIGURATION_TYPES)
        list(APPEND CMAKE_CONFIGURATION_TYPES Test)
    endif()
else()
    set(allowableBuildTypes Debug Test Release)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${allowableBuildTypes}")
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE IN_LIST allowableBuildTypes)
        message(FATAL_ERROR "Invalid build type ${CMAKE_BUILD_TYPE}") 
    endif()
endif()

set(CMAKE_TEST_POSTFIX _tests)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# use c++17
# --------------------------------------- #
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# libraries
# --------------------------------------- #
add_library(lodepng src/external/lodepng.cpp)
target_include_directories(lodepng PUBLIC include/external)

# executables
# --------------------------------------- #
add_executable(raytracer 
    src/raytracer.cpp 
    src/printer/NetpbmPrinter.cpp 
    src/buffer/Buffer.cpp 
    src/parsing/yaml/YAMLParser.cpp 
    src/raytracer/RaytracerVisitor.cpp
    src/raytracer/SurfaceInteraction.cpp
    src/scene/Scene.cpp
    src/lights/VisibilityTester.cpp
    src/parsing/obj/CyObjParser.cpp
    src/accelerators/BVHAccel.cpp
    src/globals.cpp
    src/objects/Bounds3.cpp
    )
target_link_libraries(raytracer PRIVATE lodepng PUBLIC yaml-cpp PUBLIC tao)
target_include_directories(raytracer PUBLIC include PUBLIC extern/tao/sources/include PUBLIC ${yamlcpp_SOURCE_DIR}/include)

# submodules
# ----------------------------------------#
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/tao/sources/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

add_subdirectory(extern/tao/sources)

# test definitions
# use googletest framework
# --------------------------------------- #
FetchContent_Declare(googletest
    GIT_REPOSITORY  https://github.com/google/googletest.git
    GIT_TAG         2fe3bd994b3189899d93f1d5a881e725e046fdc2 # 1.8.1
)
FetchContent_GetProperties(googletest)
if (NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

include(GoogleTest)

set(test_sources ${test_sources}
    tests/main.cpp
    tests/ray_tests.cpp
    tests/camera_tests.cpp
    tests/transform_tests.cpp
)

add_executable(maintest ${test_sources})
target_link_libraries(maintest PRIVATE gtest PRIVATE tao)
target_include_directories(maintest PRIVATE include)

gtest_discover_tests(maintest)

enable_testing()
