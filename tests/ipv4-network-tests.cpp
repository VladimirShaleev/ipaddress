#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

template <size_t N1, size_t N2>
static constexpr ipv4_network test_swap(const char(&str1)[N1], const char(&str2)[N2]) {
    auto net1 = ipv4_network::parse(str1);
    auto net2 = ipv4_network::parse(str2);
    net1.swap(net2);
    return net1;
}

template <size_t N>
static constexpr error_code test_error(const char(&str)[N]) noexcept {
    error_code err = error_code::NO_ERROR;
    ipv4_network::parse(str, err);
    return err;
}

TEST(ipv4_network, CompileTime) {
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    auto net1 = ipv4_network::parse<"127.0.0.0/8">();
    ASSERT_EQ(net1.network_address().to_uint(), 0x7F000000);
    ASSERT_EQ(net1.broadcast_address().to_uint(), 0x7FFFFFFF);
    ASSERT_EQ(net1.netmask().to_uint(), 0xFF000000);
    ASSERT_EQ(net1.hostmask().to_uint(), 0x00FFFFFF);
    ASSERT_EQ(net1.prefixlen(), 8);

    constexpr auto net2 = ipv4_network::parse<"127.0.0.1">();
    constexpr auto net2_address = net2.network_address().to_uint();
    constexpr auto net2_broadcast = net2.broadcast_address().to_uint();
    constexpr auto net2_netmask = net2.netmask().to_uint();
    constexpr auto net2_hostmask = net2.hostmask().to_uint();
    constexpr auto net2_prefixlen = net2.prefixlen();
    ASSERT_EQ(net2_address, 0x7F000001);
    ASSERT_EQ(net2_broadcast, 0x7F000001);
    ASSERT_EQ(net2_netmask, 0xFFFFFFFF);
    ASSERT_EQ(net2_hostmask, 0x00000000);
    ASSERT_EQ(net2_prefixlen, 32);
#endif

    constexpr auto net3 = ipv4_network::parse("127.0.0.0/8");
    constexpr auto net3_address = net3.network_address().to_uint();
    constexpr auto net3_broadcast = net3.broadcast_address().to_uint();
    constexpr auto net3_netmask = net3.netmask().to_uint();
    constexpr auto net3_hostmask = net3.hostmask().to_uint();
    constexpr auto net3_prefixlen = net3.prefixlen();
    constexpr auto net3_hash = net3.hash();
    ASSERT_EQ(net3_address, 0x7F000000);
    ASSERT_EQ(net3_broadcast, 0x7FFFFFFF);
    ASSERT_EQ(net3_netmask, 0xFF000000);
    ASSERT_EQ(net3_hostmask, 0x00FFFFFF);
    ASSERT_EQ(net3_prefixlen, 8);
    ASSERT_GT(net3_hash, size_t(0));

    constexpr auto net4 = test_swap("0.0.0.0/8", "127.0.0.0/16");
    constexpr auto net4_address = net4.network_address().to_uint();
    constexpr auto net4_broadcast = net4.broadcast_address().to_uint();
    constexpr auto net4_netmask = net4.netmask().to_uint();
    constexpr auto net4_hostmask = net4.hostmask().to_uint();
    constexpr auto net4_prefixlen = net4.prefixlen();
    constexpr auto net4_hash = net4.hash();
    ASSERT_EQ(net4_address, 0x7F000000);
    ASSERT_EQ(net4_broadcast, 0x7F00FFFF);
    ASSERT_EQ(net4_netmask, 0xFFFF0000);
    ASSERT_EQ(net4_hostmask, 0x0000FFFF);
    ASSERT_EQ(net4_prefixlen, 16);
    ASSERT_GT(net4_hash, size_t(0));

    constexpr auto net5_error = test_error("127.0.0.1/24");
    ASSERT_EQ(net5_error, error_code::HAS_HOST_BITS_SET);
    
    constexpr auto b1 = net3 < net4;
    constexpr auto b2 = net3 > net4;
    constexpr auto b3 = net3 <= net4;
    constexpr auto b4 = net3 >= net4;
    constexpr auto b5 = net3 == net4;
    constexpr auto b6 = net3 != net4;
    ASSERT_TRUE(b1);
    ASSERT_FALSE(b2);
    ASSERT_TRUE(b3);
    ASSERT_FALSE(b4);
    ASSERT_FALSE(b5);
    ASSERT_TRUE(b6);

    constexpr auto net6 = "127.0.0.0/16"_ipv4_net;
    constexpr auto net7 = "127.128.128.255"_ipv4_net;
    ASSERT_EQ(net6, ipv4_network::parse("127.0.0.0/16"));
    ASSERT_EQ(net7, ipv4_network::parse("127.128.128.255/32"));

    constexpr auto net8 = ipv4_network::from_address(ipv4_address::parse("127.0.0.0"), 16);
    constexpr auto net9 = ipv4_network::from_address(ipv4_address::parse("127.0.0.0"), 16, false);
    ASSERT_EQ(net8, ipv4_network::parse("127.0.0.0/16"));
    ASSERT_EQ(net9, ipv4_network::parse("127.0.0.0/16"));
}

TEST(ipv4_network, DefaultCtor) {
    ipv4_network net;
    
    EXPECT_EQ(net.network_address(), ipv4_address::from_uint<0>());
    ASSERT_EQ(net.broadcast_address(), ipv4_address::from_uint<0>());
    EXPECT_EQ(net.netmask(), ipv4_address::from_uint<0xFFFFFFFF>());
    EXPECT_EQ(net.hostmask(), ipv4_address::from_uint<0>());
    EXPECT_EQ(net.prefixlen(), 32);
    EXPECT_EQ(net.version(), ip_version::V4);
}

TEST(ipv4_network, CopyCtor) {
    auto net = ipv4_network::parse("127.0.0.0/8");
    auto net_copy = net;
    
    EXPECT_EQ(net_copy.network_address(), ipv4_address::from_uint<0x7F000000>());
    ASSERT_EQ(net_copy.broadcast_address(), ipv4_address::from_uint<0x7FFFFFFF>());
    EXPECT_EQ(net_copy.netmask(), ipv4_address::from_uint<0xFF000000>());
    EXPECT_EQ(net_copy.hostmask(), ipv4_address::from_uint<0x00FFFFFF>());
    EXPECT_EQ(net_copy.prefixlen(), 8);
}

TEST(ipv4_network, CopyOperator) {
    auto net = ipv4_network::parse("127.0.0.0/8");
    ipv4_network net_copy;
    
    EXPECT_EQ(net_copy.network_address(), ipv4_address::from_uint<0>());
    ASSERT_EQ(net_copy.broadcast_address(), ipv4_address::from_uint<0>());
    EXPECT_EQ(net_copy.netmask(), ipv4_address::from_uint<0xFFFFFFFF>());
    EXPECT_EQ(net_copy.hostmask(), ipv4_address::from_uint<0>());
    EXPECT_EQ(net_copy.prefixlen(), 32);
    net_copy = net;
    
    EXPECT_EQ(net_copy.network_address(), ipv4_address::from_uint<0x7F000000>());
    ASSERT_EQ(net_copy.broadcast_address(), ipv4_address::from_uint<0x7FFFFFFF>());
    EXPECT_EQ(net_copy.netmask(), ipv4_address::from_uint<0xFF000000>());
    EXPECT_EQ(net_copy.hostmask(), ipv4_address::from_uint<0x00FFFFFF>());
    EXPECT_EQ(net_copy.prefixlen(), 8);
}

using NetworkParserIpv4Params = TestWithParam<std::tuple<const char*, const char*, const char*, const char*, const char*, size_t>>;
TEST_P(NetworkParserIpv4Params, parse) {
    auto excepted_address = ipv4_address::parse(get<1>(GetParam()));
    auto excepted_broadcast = ipv4_address::parse(get<2>(GetParam()));
    auto excepted_netmask = ipv4_address::parse(get<3>(GetParam()));
    auto excepted_hostmask = ipv4_address::parse(get<4>(GetParam()));
    auto excepted_prefixlen = get<5>(GetParam());

    auto net = ipv4_network::parse(get<0>(GetParam()));
   
    EXPECT_EQ(net.network_address(), excepted_address);
    ASSERT_EQ(net.broadcast_address(), excepted_broadcast);
    EXPECT_EQ(net.netmask(), excepted_netmask);
    EXPECT_EQ(net.hostmask(), excepted_hostmask);
    EXPECT_EQ(net.prefixlen(), excepted_prefixlen);

    std::string s1;
    ipv4_network net_from_stream;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> net_from_stream >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    EXPECT_EQ(net_from_stream.network_address(), excepted_address);
    ASSERT_EQ(net_from_stream.broadcast_address(), excepted_broadcast);
    EXPECT_EQ(net_from_stream.netmask(), excepted_netmask);
    EXPECT_EQ(net_from_stream.hostmask(), excepted_hostmask);
    EXPECT_EQ(net_from_stream.prefixlen(), excepted_prefixlen);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, NetworkParserIpv4Params,
    testing::Values(
        std::make_tuple("1.2.3.4", "1.2.3.4", "1.2.3.4", "255.255.255.255", "0.0.0.0", 32),
        std::make_tuple("1.2.3.4/32", "1.2.3.4", "1.2.3.4", "255.255.255.255", "0.0.0.0", 32),
        std::make_tuple("1.2.3.4/255.255.255.255", "1.2.3.4", "1.2.3.4", "255.255.255.255", "0.0.0.0", 32),
        std::make_tuple("192.0.2.0/24", "192.0.2.0", "192.0.2.255", "255.255.255.0", "0.0.0.255", 24),
        std::make_tuple("192.0.2.0/255.255.255.0", "192.0.2.0", "192.0.2.255", "255.255.255.0", "0.0.0.255", 24),
        std::make_tuple("192.0.2.0/0.0.0.255", "192.0.2.0", "192.0.2.255", "255.255.255.0", "0.0.0.255", 24),
        std::make_tuple("192.0.2.0/27", "192.0.2.0", "192.0.2.31", "255.255.255.224", "0.0.0.31", 27),
        std::make_tuple("192.0.2.0/255.255.255.224", "192.0.2.0", "192.0.2.31", "255.255.255.224", "0.0.0.31", 27)
    ));

using NetworkParserIpv4NotStrictParams = TestWithParam<std::tuple<const char*, const char*, const char*, const char*, const char*, size_t>>;
TEST_P(NetworkParserIpv4NotStrictParams, parse_not_strict) {
    auto excepted_address = ipv4_address::parse(get<1>(GetParam()));
    auto excepted_broadcast = ipv4_address::parse(get<2>(GetParam()));
    auto excepted_netmask = ipv4_address::parse(get<3>(GetParam()));
    auto excepted_hostmask = ipv4_address::parse(get<4>(GetParam()));
    auto excepted_prefixlen = get<5>(GetParam());

    auto net = ipv4_network::parse(get<0>(GetParam()), false);
   
    EXPECT_EQ(net.network_address(), excepted_address);
    ASSERT_EQ(net.broadcast_address(), excepted_broadcast);
    EXPECT_EQ(net.netmask(), excepted_netmask);
    EXPECT_EQ(net.hostmask(), excepted_hostmask);
    EXPECT_EQ(net.prefixlen(), excepted_prefixlen);

    std::string s1;
    ipv4_network net_from_stream;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> not_strict >> net_from_stream >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    EXPECT_EQ(net_from_stream.network_address(), excepted_address);
    ASSERT_EQ(net_from_stream.broadcast_address(), excepted_broadcast);
    EXPECT_EQ(net_from_stream.netmask(), excepted_netmask);
    EXPECT_EQ(net_from_stream.hostmask(), excepted_hostmask);
    EXPECT_EQ(net_from_stream.prefixlen(), excepted_prefixlen);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, NetworkParserIpv4NotStrictParams,
    testing::Values(
        std::make_tuple("1.2.3.4/24", "1.2.3.0", "1.2.3.255", "255.255.255.0", "0.0.0.255", 24),
        std::make_tuple("192.0.2.0/255.255.0.0", "192.0.0.0", "192.0.255.255", "255.255.0.0", "0.0.255.255", 16)
    ));

using NetworkFromAddressIpv4Params = TestWithParam<std::tuple<const char*, const char*, size_t, bool>>;
TEST_P(NetworkFromAddressIpv4Params, from_address) {
    auto address = ipv4_address::parse(get<1>(GetParam()));
    auto prefixlen = get<2>(GetParam());
    auto strict = get<3>(GetParam());

    auto expected = ipv4_network::parse(get<0>(GetParam()), strict);

    error_code err = error_code::NO_ERROR;
    auto actual = ipv4_network::from_address(address, err, prefixlen, strict);
    ASSERT_EQ(actual, expected);
    ASSERT_EQ(actual.network_address(), expected.network_address());
    ASSERT_EQ(actual.broadcast_address(), expected.broadcast_address());
    ASSERT_EQ(actual.netmask(), expected.netmask());
    ASSERT_EQ(actual.hostmask(), expected.hostmask());
    ASSERT_EQ(actual.prefixlen(), expected.prefixlen());

    auto actual2 = ipv4_network::from_address(address, prefixlen, strict);
    ASSERT_EQ(actual2, expected);
    ASSERT_EQ(actual2.network_address(), expected.network_address());
    ASSERT_EQ(actual2.broadcast_address(), expected.broadcast_address());
    ASSERT_EQ(actual2.netmask(), expected.netmask());
    ASSERT_EQ(actual2.hostmask(), expected.hostmask());
    ASSERT_EQ(actual2.prefixlen(), expected.prefixlen());
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, NetworkFromAddressIpv4Params,
    testing::Values(
        std::make_tuple("1.2.3.4/24", "1.2.3.4", 24, false),
        std::make_tuple("192.0.2.0/255.255.255.0", "192.0.2.0", 24, true)
    ));

TEST(ipv4_network, from_address_error) {
    error_code err = error_code::NO_ERROR;
    auto actual = ipv4_network::from_address(ipv4_address::parse("127.0.0.1"), err, 24);
    ASSERT_EQ(err, error_code::HAS_HOST_BITS_SET);
    ASSERT_EQ(actual.network_address(), ipv4_address::parse("0.0.0.0"));
    ASSERT_EQ(actual.broadcast_address(), ipv4_address::parse("0.0.0.0"));
    ASSERT_EQ(actual.netmask(), ipv4_address::parse("255.255.255.255"));
    ASSERT_EQ(actual.hostmask(), ipv4_address::parse("0.0.0.0"));
    ASSERT_EQ(actual.prefixlen(), 32);
    
#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_network = ipv4_network::from_address(ipv4_address::parse("127.0.0.1"), 24);
    ASSERT_EQ(error_network.address(), ipv4_address::parse("0.0.0.0"));
    ASSERT_EQ(error_network.netmask(), ipv4_address::parse("255.255.255.255"));
    ASSERT_EQ(error_network.hostmask(), ipv4_address::parse("0.0.0.0"));
    ASSERT_EQ(error_network.prefixlen(), 32);
#else
    EXPECT_THAT(
        []() { ipv4_network::from_address(ipv4_address::parse("127.0.0.1"), 24); },
        ThrowsMessage<parse_error>(StrEq("has host bits set in address 127.0.0.1")));
#endif
}

using InvalidNetworkIpv4Params = TestWithParam<std::tuple<const char*, error_code, const char*>>;
TEST_P(InvalidNetworkIpv4Params, parse) {
    auto expected_network = get<0>(GetParam());
    auto expected_error_code = get<1>(GetParam());

    error_code err;
    ipv4_network::parse(expected_network, err);
    ASSERT_EQ(err, expected_error_code);

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_network = ipv4_network::parse(expected_network);

    EXPECT_EQ(error_network.address().to_uint(), 0x00000000);
    ASSERT_EQ(error_network.netmask().to_uint(), 0xFFFFFFFF);
    ASSERT_EQ(error_network.hostmask().to_uint(), 0x00000000);
    ASSERT_EQ(error_network.prefixlen(), 32);
#else
    EXPECT_THAT(
        [network=expected_network]() { ipv4_network::parse(network); },
        ThrowsMessage<parse_error>(StrEq(get<2>(GetParam()))));
    EXPECT_THAT(
        [network=expected_network]() { ipv4_network::parse(network); },
        Throws<parse_error>(Property(&parse_error::code, Eq(expected_error_code))));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, InvalidNetworkIpv4Params,
    testing::Values(
        std::make_tuple("1.2.3.4/", error_code::EMPTY_NETMASK, "empty mask in address 1.2.3.4/"),
        std::make_tuple("1.2.3.4/33", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/33"),
        std::make_tuple("1.2.3.4/255.0.0.256", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/255.0.0.256"),
        std::make_tuple("1.2.3.4/-1", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/-1"),
        std::make_tuple("1.2.3.4/+1", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/+1"),
        std::make_tuple("1.2.3.4/ 1 ", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/ 1 "),
        std::make_tuple("1.2.3.4/0x1", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/0x1"),
        std::make_tuple("1.2.3.4/254.254.255.256", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/254.254.255.256"),
        std::make_tuple("1.2.3.4/1.a.2.3", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.2.3.4/1.a.2.3"),
        std::make_tuple("1.1.1.1/254.xyz.2.3", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.1.1.1/254.xyz.2.3"),
        std::make_tuple("1.1.1.1/pudding", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.1.1.1/pudding"),
        std::make_tuple("1.1.1.1/::", error_code::INVALID_NETMASK, "is not a valid netmask in address 1.1.1.1/::"),

        std::make_tuple("1.2.3.4/255.255.255.127", error_code::NETMASK_PATTERN_MIXES_ZEROES_AND_ONES, "netmask pattern mixes zeroes & ones in address 1.2.3.4/255.255.255.127"),
        std::make_tuple("1.2.3.4/255.255.255.251", error_code::NETMASK_PATTERN_MIXES_ZEROES_AND_ONES, "netmask pattern mixes zeroes & ones in address 1.2.3.4/255.255.255.251"),
        std::make_tuple("1.1.1.1/240.255.0.0", error_code::NETMASK_PATTERN_MIXES_ZEROES_AND_ONES, "netmask pattern mixes zeroes & ones in address 1.1.1.1/240.255.0.0"),
        std::make_tuple("1.1.1.1/255.254.128.0", error_code::NETMASK_PATTERN_MIXES_ZEROES_AND_ONES, "netmask pattern mixes zeroes & ones in address 1.1.1.1/255.254.128.0"),
        std::make_tuple("1.1.1.1/0.1.127.255", error_code::NETMASK_PATTERN_MIXES_ZEROES_AND_ONES, "netmask pattern mixes zeroes & ones in address 1.1.1.1/0.1.127.255"),

        std::make_tuple("1.2.3.4/24", error_code::HAS_HOST_BITS_SET, "has host bits set in address 1.2.3.4/24"),
        std::make_tuple("1.2.3.4//", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 1.2.3.4//"),
        std::make_tuple("1.2.3.4//32", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 1.2.3.4//32"),
        std::make_tuple("1.2.3.4/32/24", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 1.2.3.4/32/24"),

        std::make_tuple("/", error_code::EMPTY_NETMASK, "empty mask in address /"),
        std::make_tuple("/8", error_code::EMPTY_ADDRESS, "address cannot be empty"),
        std::make_tuple("bogus", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address bogus has invalid symbol"),
        std::make_tuple("example.com", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address example.com has invalid symbol"),
        std::make_tuple("10/8", error_code::EXPECTED_4_OCTETS, "expected 4 octets in 10/8"),
        std::make_tuple("::1.2.3.4", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address ::1.2.3.4 has invalid symbol"),
        std::make_tuple("1.2.3.256", error_code::OCTET_EXCEEDED_255, "octet 0 of address 1.2.3.256 exceeded 255")
    ));

TEST(ipv4_network, Comparison) {
    auto net1 = ipv4_network::parse("127.240.1.0/24");
    auto net2 = ipv4_network::parse("127.240.1.0");
    auto net3 = ipv4_network::parse("127.240.1.0/32");
    
    ASSERT_TRUE(net1 < net2);
    ASSERT_TRUE(net1 <= net2);
    ASSERT_FALSE(net1 > net2);
    ASSERT_FALSE(net1 >= net2);
    ASSERT_FALSE(net1 == net2);
    ASSERT_TRUE(net1 != net2);
    
    ASSERT_FALSE(net2 < net1);
    ASSERT_FALSE(net2 <= net1);
    ASSERT_TRUE(net2 > net1);
    ASSERT_TRUE(net2 >= net1);
    ASSERT_FALSE(net2 == net1);
    ASSERT_TRUE(net2 != net1);
    
    ASSERT_FALSE(net2 < net3);
    ASSERT_TRUE(net2 <= net3);
    ASSERT_FALSE(net2 > net3);
    ASSERT_TRUE(net2 >= net3);
    ASSERT_TRUE(net2 == net3);
    ASSERT_FALSE(net2 != net3);
    
    ASSERT_FALSE(net3 < net2);
    ASSERT_TRUE(net3 <= net2);
    ASSERT_FALSE(net3 > net2);
    ASSERT_TRUE(net3 >= net2);
    ASSERT_TRUE(net3 == net2);
    ASSERT_FALSE(net3 != net2);
}

using ToStringNetworkIpv4Params = TestWithParam<std::tuple<const char*, const char*>>;
TEST_P(ToStringNetworkIpv4Params, to_string) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_network::parse(std::get<0>(GetParam()));

    std::ostringstream ss;
    ss << actual;

    ASSERT_EQ(actual.to_string(), std::string(expected));
    ASSERT_EQ(std::string(actual), std::string(expected));
    ASSERT_EQ(std::to_string(actual), std::string(expected));
    ASSERT_EQ(ss.str(), std::string(expected));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_network, ToStringNetworkIpv4Params,
    testing::Values(
        std::make_tuple("1.2.3.4", "1.2.3.4/32"),
        std::make_tuple("1.2.3.4/32", "1.2.3.4/32"),
        std::make_tuple("1.2.3.4/255.255.255.255", "1.2.3.4/32")
    ));

TEST(ipv4_network, Hash) {
    auto net1 = ipv4_network::parse("127.0.0.1");
    auto net2 = ipv4_network::parse("127.0.0.1/32");
    auto net3 = ipv4_network::parse("127.0.0.0/24");
    auto net4 = ipv4_network::parse("127.0.0.0/16");
    auto hash_functor = std::hash<ipv4_network>{};

    ASSERT_EQ(net1.hash(), sizeof(size_t) == 8 ? 605929542755200525ULL : 2256296057U);
    ASSERT_EQ(net2.hash(), sizeof(size_t) == 8 ? 605929542755200525ULL : 2256296057U);
    ASSERT_EQ(net3.hash(), sizeof(size_t) == 8 ? 7623195473821216247ULL : 546525844U);
    ASSERT_EQ(net4.hash(), sizeof(size_t) == 8 ? 5503161050249481481ULL : 2858631644U);

    ASSERT_EQ(hash_functor(net1), sizeof(size_t) == 8 ? 605929542755200525ULL : 2256296057U);
    ASSERT_EQ(hash_functor(net2), sizeof(size_t) == 8 ? 605929542755200525ULL : 2256296057U);
    ASSERT_EQ(hash_functor(net3), sizeof(size_t) == 8 ? 7623195473821216247ULL : 546525844U);
    ASSERT_EQ(hash_functor(net4), sizeof(size_t) == 8 ? 5503161050249481481ULL : 2858631644U);
}

TEST(ipv4_network, Containers) {
    auto net1 = ipv4_network::parse("127.0.0.0");
    auto net2 = ipv4_network::parse("127.0.0.0/24");
    auto net3 = ipv4_network::parse("127.0.0.0/16");

    std::vector<ipv4_network> vec;
    vec.push_back(net1);
    vec.push_back(net2);
    vec.emplace_back(net3);
    ASSERT_EQ(vec[0], net1);
    ASSERT_EQ(vec[1], net2);
    ASSERT_EQ(vec[2], net3);

    std::map<ipv4_network, int> map;
    map[net2] = 2;
    map[net1] = 1;
    map[net3] = 3;
    auto it = map.begin();
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(it++->first, net3);
    ASSERT_EQ(it++->first, net2);
    ASSERT_EQ(it++->first, net1);
    
    std::unordered_map<ipv4_network, int> unordered_map;
    unordered_map[net2] = 2;
    unordered_map[net1] = 1;
    unordered_map[net3] = 3;
    unordered_map[net3] = 4;
    ASSERT_EQ(unordered_map.size(), 3);
    ASSERT_EQ(unordered_map[net1], 1);
    ASSERT_EQ(unordered_map[net2], 2);
    ASSERT_EQ(unordered_map[net3], 4);
}

TEST(ipv4_network, Swap) {
    auto net1 = ipv4_network::parse("127.0.0.1");
    auto net2 = ipv4_network::parse("127.0.0.0/24");
    
    std::swap(net1, net2);

    ASSERT_EQ(net1, ipv4_network::parse("127.0.0.0/255.255.255.0"));
    ASSERT_EQ(net2, ipv4_network::parse("127.0.0.1"));
}

TEST(ipv4_network, literals) {
    auto net1 = "127.0.0.0/16"_ipv4_net;
    auto net2 = "127.128.128.255"_ipv4_net;
    
    ASSERT_EQ(net1, ipv4_network::parse("127.0.0.0/16"));
    ASSERT_EQ(net2, ipv4_network::parse("127.128.128.255/32"));
}
