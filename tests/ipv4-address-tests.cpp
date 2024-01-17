#include <gtest/gtest.h>

#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

#if IPADDRESS_CPP_VERSION >= 20
#  define TEST_IPV4_ADDRESS ipv4_address_20
#elif IPADDRESS_CPP_VERSION >= 17
#  define TEST_IPV4_ADDRESS ipv4_address_17
#elif IPADDRESS_CPP_VERSION >= 14
#  define TEST_IPV4_ADDRESS ipv4_address_14
#elif IPADDRESS_CPP_VERSION >= 11
#  define TEST_IPV4_ADDRESS ipv4_address_11
#endif

TEST(TEST_IPV4_ADDRESS, Test)
{
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    auto ipv4 = ipv4_address::parse<"127.0.0.1">();
    auto ipv4_ = ipv4_address::parse<"127.0.0.1">();
#endif

    constexpr auto ipv4_2 = ipv4_address::parse("127.0.0.1");
    auto ipv4_2_ = ipv4_address::parse("127.0.0.1");
    auto b = ipv4_2_;
    //constexpr auto d = ipv4;

    //constexpr auto ipv4 = parse<"127.0.0.1">();

    // ipv4_address::ip_from_string()
}
