if(POLICY CMP0155)
  cmake_policy(SET CMP0155 NEW)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 16)
    message(STATUS "The C++20 module requires Clang 16 or newer")
    return()
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14)
    message(STATUS "The C++20 module requires GCC 14 or newer")
    return()
  endif ()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 17.4)
    message(STATUS "The C++20 module requires MSVC 17.4 or newer")
    return()
  endif ()
else()
  message(STATUS "The C++20 module is not supported by the compiler: ${CMAKE_CXX_COMPILER_ID}")
  return()
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
  message(VERBOSE "CMake supports the C++20 module")
elseif(CMAKE_VERSION VERSION_GREATER_EQUAL 3.27)
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
elseif(CMAKE_VERSION VERSION_GREATER_EQUAL 3.26)
  set(CMAKE_EXPERIMENTAL_CXX_SCANDEP_SOURCE "")
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
endif()

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-Winclude-angled-in-module-purview" IPADDRESS_MODULE_PURVIEW_FOUND)

add_library(${PROJECT_NAME}-module)
add_library(${PROJECT_NAME}::${PROJECT_NAME}-module ALIAS ${PROJECT_NAME}-module)
target_link_libraries(${PROJECT_NAME}-module PRIVATE ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}-module PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(${PROJECT_NAME}-module PROPERTIES CXX_EXTENSIONS OFF)
target_sources(${PROJECT_NAME}-module
  PUBLIC
    FILE_SET CXX_MODULES
    TYPE CXX_MODULES
    FILES ipaddress.ixx)

if(IPADDRESS_MODULE_PURVIEW_FOUND)
  target_compile_options(${PROJECT_NAME}-module PRIVATE "-Wno-include-angled-in-module-purview")
endif()

if(CMAKE_GENERATOR MATCHES Ninja)
  install(TARGETS ${PROJECT_NAME}-module
    EXPORT ${PROJECT_NAME}-targets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    FILE_SET CXX_MODULES DESTINATION "${CMAKE_INSTALL_LIBDIR}/cxx/miu"
    CXX_MODULES_BMI DESTINATION "${CMAKE_INSTALL_LIBDIR}/cxx/bmi"
    )
  install(EXPORT ${PROJECT_NAME}-targets 
    DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}"
    NAMESPACE ${PROJECT_NAME}::
    CXX_MODULES_DIRECTORY "cxx-modules")
endif()
