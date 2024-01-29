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
    ipv4_network::base_type expected_empty { 0, 0, 0, 0};

    ipv4_network net;
    
    EXPECT_EQ(net.address(), ipv4_address::from_uint32<0>());
    EXPECT_EQ(net.netmask(), ipv4_address::from_uint32<0xFFFFFFFF>());
    EXPECT_EQ(net.hostmask(), ipv4_address::from_uint32<0>());
    EXPECT_EQ(net.prefixlen(), 32);
    EXPECT_EQ(net.version, version::V4);
}
