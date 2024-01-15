#include <gtest/gtest.h>

#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

TEST(ipv4_address, Test)
{
    constexpr auto ipv4 = ipv4_address::parse<"127.0.0.1">();
    constexpr auto d = ipv4;

    // constexpr auto ipv4 = parse<"127.0.0.1">();

    // ipv4_address::ip_from_string()
}
