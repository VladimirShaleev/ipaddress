cmake_minimum_required(VERSION 3.8.0)

project(ipaddress 
  HOMEPAGE_URL "https://github.com/VladimirShaleev/ipaddress"
  VERSION 1.0.0
  LANGUAGES CXX)

include(CTest)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

include(CMakeDependentOption)
include(CTest)

cmake_dependent_option(IPADDRESS_BUILD_TESTS "Build tests" ON
  "BUILD_TESTING;CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR" OFF)

if(IPADDRESS_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest
      GIT_TAG v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    file(GLOB_RECURSE IPADDRESS_TESTS_SOURCES "tests/*.hpp" "tests/*.cpp")
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${IPADDRESS_TESTS_SOURCES})
    
    add_executable(ipaddress-tests ${IPADDRESS_TESTS_SOURCES})
    target_link_libraries(ipaddress-tests ipaddress GTest::gtest GTest::gtest_main)
    
    include(GoogleTest)
    gtest_discover_tests(ipaddress-tests)
endif()
