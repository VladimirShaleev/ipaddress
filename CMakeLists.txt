cmake_minimum_required(VERSION 3.11)

option(IPADDRESS_NO_EXCEPTIONS "Disable handling cpp exception for" OFF)
option(IPADDRESS_NO_IPV6_SCOPE "Disable scope id for IPv6 addresses" OFF)
option(IPADDRESS_NO_OVERLOAD_STD "Do not overload std functions such as to_string, hash etc" OFF)
set(IPADDRESS_IPV6_SCOPE_MAX_LENGTH "16" CACHE STRING "Maximum scope-id length for IPv6 addresses")

project(ipaddress 
  HOMEPAGE_URL "https://github.com/VladimirShaleev/ipaddress"
  DESCRIPTION "A library for working and manipulating IPv4/IPv6 addresses and networks"
  VERSION 1.2.0
  LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(GNUInstallDirs)
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
if(IPADDRESS_NO_IPV6_SCOPE)
  target_compile_definitions(${PROJECT_NAME} INTERFACE IPADDRESS_NO_IPV6_SCOPE)
  target_compile_definitions(${PROJECT_NAME} INTERFACE IPADDRESS_IPV6_SCOPE_MAX_LENGTH=0)
else()
  target_compile_definitions(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:IPADDRESS_IPV6_SCOPE_MAX_LENGTH=${IPADDRESS_IPV6_SCOPE_MAX_LENGTH}>)
endif()
target_compile_definitions(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:IPADDRESS_ENDIAN=${IPADDRESS_BIG_ENDIAN}>)
if(MSVC)
  target_sources(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.natvis>)
endif()

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/package/ipaddress-config.cmake.in" 
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(${PROJECT_NAME}-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/package/ipaddress.pc.in" 
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc" @ONLY)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets)
install(EXPORT ${PROJECT_NAME}-targets 
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}"
  NAMESPACE ${PROJECT_NAME}::)
install(
  FILES
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.natvis"
  DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME})
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/pkgconfig")
install(DIRECTORY include/ DESTINATION include)

cmake_dependent_option(IPADDRESS_ENABLE_CLANG_TIDY "Enable clang-tidy checks" OFF
  "CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR" OFF)
cmake_dependent_option(IPADDRESS_BUILD_TESTS "Build tests" ON
  "BUILD_TESTING;CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR" OFF)
cmake_dependent_option(IPADDRESS_BUILD_BENCHMARK "Build benchmark" OFF
  "CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR" OFF)
cmake_dependent_option(IPADDRESS_BUILD_DOC "Build doc" ON
  "CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR" OFF)
cmake_dependent_option(IPADDRESS_BUILD_PACKAGES "Build packages" ON
  "CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR" OFF)
cmake_dependent_option(IPADDRESS_BUILD_MODULE "Build Cpp module" OFF
  "CMAKE_VERSION VERSION_GREATER_EQUAL 3.26.0;cxx_std_20 IN_LIST CMAKE_CXX_COMPILE_FEATURES;CMAKE_GENERATOR MATCHES Visual OR CMAKE_GENERATOR MATCHES Ninja" OFF)

if(IPADDRESS_ENABLE_CLANG_TIDY)
  find_program(IPADDRESS_CLANG_TIDY NAMES clang-tidy-16 clang-tidy)
  if(IPADDRESS_CLANG_TIDY)
    execute_process(COMMAND "${IPADDRESS_CLANG_TIDY}" --version
      OUTPUT_VARIABLE IPADDRESS_CLANG_TIDY_VERSION
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "clang-tidy ${IPADDRESS_CLANG_TIDY_VERSION}")
    string(REGEX MATCH ".*LLVM version ([0-9]+)\\.([0-9]+)\\.([0-9]+)" _ ${IPADDRESS_CLANG_TIDY_VERSION})
    set(IPADDRESS_CLANG_TIDY_VERSION_MAJOR ${CMAKE_MATCH_1})
    if(IPADDRESS_CLANG_TIDY_VERSION_MAJOR LESS 14)
      message(WARNING "Supported by clang-tidy version 14 or higher")
      unset(IPADDRESS_CLANG_TIDY CACHE)
      unset(IPADDRESS_ENABLE_CLANG_TIDY CACHE)
    endif()
  else()
    message(WARNING "Could not find clang-tidy program")
    unset(IPADDRESS_ENABLE_CLANG_TIDY CACHE)
  endif()
endif()

if(IPADDRESS_BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(IPADDRESS_BUILD_BENCHMARK)
  add_subdirectory(benchmark)
endif()

if(IPADDRESS_BUILD_DOC)
  add_subdirectory(doc)
endif()

if(IPADDRESS_BUILD_PACKAGES)
  add_subdirectory(package)
endif()

if(IPADDRESS_BUILD_MODULE)
  add_subdirectory(src)
endif()
