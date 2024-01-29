#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

TEST(ipv4_network, DefaultCtor) {
    ipv4_network net;
    
    EXPECT_EQ(net.address(), ipv4_address::from_uint32<0>());
    EXPECT_EQ(net.netmask(), ipv4_address::from_uint32<0xFFFFFFFF>());
    EXPECT_EQ(net.hostmask(), ipv4_address::from_uint32<0>());
    EXPECT_EQ(net.prefixlen(), 32);
    EXPECT_EQ(net.version, version::V4);
}

TEST(ipv4_network, CopyCtor) {
    auto net = ipv4_network::parse("127.0.0.0/8");
    auto net_copy = net;
    
    EXPECT_EQ(net_copy.address(), ipv4_address::from_uint32<0x7F000000>());
    EXPECT_EQ(net_copy.netmask(), ipv4_address::from_uint32<0xFF000000>());
    EXPECT_EQ(net_copy.hostmask(), ipv4_address::from_uint32<0x00FFFFFF>());
    EXPECT_EQ(net_copy.prefixlen(), 8);
}

TEST(ipv4_network, CopyOperator) {
    auto net = ipv4_network::parse("127.0.0.0/8");
    ipv4_network net_copy;
    
    EXPECT_EQ(net_copy.address(), ipv4_address::from_uint32<0>());
    EXPECT_EQ(net_copy.netmask(), ipv4_address::from_uint32<0xFFFFFFFF>());
    EXPECT_EQ(net_copy.hostmask(), ipv4_address::from_uint32<0>());
    EXPECT_EQ(net_copy.prefixlen(), 32);
    net_copy = net;
    
    EXPECT_EQ(net_copy.address(), ipv4_address::from_uint32<0x7F000000>());
    EXPECT_EQ(net_copy.netmask(), ipv4_address::from_uint32<0xFF000000>());
    EXPECT_EQ(net_copy.hostmask(), ipv4_address::from_uint32<0x00FFFFFF>());
    EXPECT_EQ(net_copy.prefixlen(), 8);
}
