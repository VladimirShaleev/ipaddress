include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG 58d77fa8070e8cec2dc1ed015d66b454c8d78850 # release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if(CMAKE_CXX_STANDARD)
  message(STATUS "Select standatd C++${CMAKE_CXX_STANDARD}")
endif()

include(CMakeDependentOption)
cmake_dependent_option(IPADDRESS_TEST_MODULE "Test Cpp module" OFF "IPADDRESS_BUILD_MODULE" OFF)

enable_testing()
add_executable(ipaddress-tests 
  "uint128-tests.cpp"
  "fixed-string-tests.cpp" 
  "fixed-vector-tests.cpp" 
  "ipv4-address-tests.cpp" 
  "ipv6-address-tests.cpp" 
  "ipv4-network-tests.cpp" 
  "ipv6-network-tests.cpp" 
  "ip-address-tests.cpp" 
  "ip-network-tests.cpp")
target_link_libraries(ipaddress-tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock_main)
if(IPADDRESS_TEST_MODULE)
  if(NOT CMAKE_CXX_STANDARD)
    message(FATAL_ERROR
      "CMAKE_CXX_STANDARD must be explicitly set when building tests with C++ modules.\n"
      "For example: -DCMAKE_CXX_STANDARD=20")
  endif()
  target_link_libraries(ipaddress-tests PRIVATE ipaddress-module)
  set_target_properties(ipaddress-tests PROPERTIES CXX_SCAN_FOR_MODULES ON)
  set_target_properties(ipaddress-tests PROPERTIES CXX_EXTENSIONS OFF)
  if(MSVC)
    target_compile_options(ipaddress-tests PRIVATE /bigobj)
  endif()

  # These macros are used in unit tests, so let's add them
  target_compile_definitions(ipaddress-tests PRIVATE IPADDRESS_TEST_MODULE)
  target_compile_definitions(ipaddress-tests PRIVATE IPADDRESS_NONTYPE_TEMPLATE_PARAMETER)
  target_compile_definitions(ipaddress-tests PRIVATE IPADDRESS_CONSTEXPR=constexpr)
  target_compile_definitions(ipaddress-tests PRIVATE IPADDRESS_CONSTEXPR_17=constexpr)
  target_compile_definitions(ipaddress-tests PRIVATE IPADDRESS_CPP_VERSION=${CMAKE_CXX_STANDARD})
else()
  target_sources(ipaddress-tests PRIVATE "unicode-tests.cpp")
  target_link_libraries(ipaddress-tests PRIVATE ipaddress)
endif()

if(IPADDRESS_ENABLE_CLANG_TIDY AND IPADDRESS_CLANG_TIDY)
  set_target_properties(ipaddress-tests PROPERTIES CXX_CLANG_TIDY ${IPADDRESS_CLANG_TIDY})
endif()

include(GoogleTest)
gtest_discover_tests(ipaddress-tests)
