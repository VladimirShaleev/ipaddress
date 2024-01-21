cmake_minimum_required(VERSION 3.8.0)

option(IPADDRESS_NO_EXCEPTIONS "Disable handling cpp exception for" OFF)
option(IPADDRESS_NO_OVERLOAD_STD "Do not overload std functions such as to_string, hash etc" OFF)

project(ipaddress 
  HOMEPAGE_URL "https://github.com/VladimirShaleev/ipaddress"
  VERSION 1.0.0
  LANGUAGES CXX)

include(CMakeDependentOption)
include(TestBigEndian)
include(CTest)

test_big_endian(IPADDRESS_BIG_ENDIAN)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

if(IPADDRESS_NO_EXCEPTIONS)
  target_compile_definitions(${PROJECT_NAME} INTERFACE IPADDRESS_NO_EXCEPTIONS)
endif()
if(IPADDRESS_NO_OVERLOAD_STD)
  target_compile_definitions(${PROJECT_NAME} INTERFACE IPADDRESS_NO_OVERLOAD_STD)
endif()
target_compile_definitions(${PROJECT_NAME} INTERFACE IPADDRESS_ENDIAN=${IPADDRESS_BIG_ENDIAN})

cmake_dependent_option(IPADDRESS_BUILD_TESTS "Build tests" ON
  "BUILD_TESTING;CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR" OFF)

if(IPADDRESS_BUILD_TESTS)
  add_subdirectory(tests)
endif()
