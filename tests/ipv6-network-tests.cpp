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
static constexpr ipv6_network test_swap(const char(&str1)[N1], const char(&str2)[N2]) {
    auto net1 = ipv6_network::parse(str1);
    auto net2 = ipv6_network::parse(str2);
    net1.swap(net2);
    return net1;
}

template <size_t N>
static constexpr error_code test_error(const char(&str)[N]) noexcept {
    error_code err = error_code::NO_ERROR;
    ipv6_network::parse(str, err);
    return err;
}

TEST(ipv6_network, CompileTime) {
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    auto net1 = ipv6_network::parse<"2001:db8::/96">();
    ASSERT_EQ(net1.address(), ipv6_address::parse("2001:db8::"));
    ASSERT_EQ(net1.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff::"));
    ASSERT_EQ(net1.hostmask(), ipv6_address::parse("::ffff:ffff"));
    ASSERT_EQ(net1.prefixlen(), 96);

    constexpr auto net2 = ipv6_network::parse<"2001:db8::1/128">();
    constexpr auto net2_address = net2.address();
    constexpr auto net2_netmask = net2.netmask();
    constexpr auto net2_hostmask = net2.hostmask();
    constexpr auto net2_prefixlen = net2.prefixlen();
    ASSERT_EQ(net2_address, ipv6_address::parse("2001:db8::1"));
    ASSERT_EQ(net2_netmask, ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    ASSERT_EQ(net2_hostmask, ipv6_address::parse("::"));
    ASSERT_EQ(net2_prefixlen, 128);
#endif

    constexpr auto net3 = ipv6_network::parse("2001:db8::/32");
    constexpr auto net3_address = net3.address();
    constexpr auto net3_netmask = net3.netmask();
    constexpr auto net3_hostmask = net3.hostmask();
    constexpr auto net3_prefixlen = net3.prefixlen();
    constexpr auto net3_hash = net3.hash();
    ASSERT_EQ(net3_address, ipv6_address::parse("2001:db8::"));
    ASSERT_EQ(net3_netmask, ipv6_address::parse("ffff:ffff::"));
    ASSERT_EQ(net3_hostmask, ipv6_address::parse("::ffff:ffff:ffff:ffff:ffff:ffff"));
    ASSERT_EQ(net3_prefixlen, 32);
    ASSERT_GT(net3_hash, size_t(0));

    constexpr auto net4 = test_swap("::/128", "2001:db8::%test/64");
    constexpr auto net4_address = net4.address();
    constexpr auto net4_netmask = net4.netmask();
    constexpr auto net4_hostmask = net4.hostmask();
    constexpr auto net4_prefixlen = net4.prefixlen();
    constexpr auto net4_hash = net4.hash();
    ASSERT_EQ(net4_address, ipv6_address::parse("2001:db8::%test"));
    ASSERT_EQ(net4_netmask, ipv6_address::parse("ffff:ffff:ffff:ffff::"));
    ASSERT_EQ(net4_hostmask, ipv6_address::parse("::ffff:ffff:ffff:ffff"));
    ASSERT_EQ(net4_prefixlen, 64);
    ASSERT_GT(net4_hash, size_t(0));

    constexpr auto net5_error = test_error("2001:db8::/8");
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

    constexpr auto net6 = "2001:db8::/32"_ipv6_net;
    constexpr auto net7 = "2001:db8::"_ipv6_net;
    ASSERT_EQ(net6, ipv6_network::parse("2001:db8::/32"));
    ASSERT_EQ(net7, ipv6_network::parse("2001:db8::/128"));

    constexpr auto net8 = ipv6_network::from_address(ipv6_address::parse("2001:db8::"), 32);
    constexpr auto net9 = ipv6_network::from_address(ipv6_address::parse("2001:db8::"), 32, false);
    ASSERT_EQ(net8, ipv6_network::parse("2001:db8::/32"));
    ASSERT_EQ(net9, ipv6_network::parse("2001:db8::/32"));
}

TEST(ipv6_network, DefaultCtor) {
    ipv6_network net;
    
    EXPECT_EQ(net.address(), ipv6_address::parse("::"));
    EXPECT_EQ(net.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    EXPECT_EQ(net.hostmask(), ipv6_address::parse("::"));
    EXPECT_EQ(net.prefixlen(), 128);
    EXPECT_EQ(net.version(), ip_version::V6);
}

TEST(ipv6_network, CopyCtor) {
    auto net = ipv6_network::parse("2001:db8::/96");
    auto net_copy = net;
    
    EXPECT_EQ(net_copy.address(), ipv6_address::parse("2001:db8::"));
    EXPECT_EQ(net_copy.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff::"));
    EXPECT_EQ(net_copy.hostmask(), ipv6_address::parse("::ffff:ffff"));
    EXPECT_EQ(net_copy.prefixlen(), 96);
}

TEST(ipv6_network, CopyOperator) {
    auto net = ipv6_network::parse("2001:db8::/96");
    ipv6_network net_copy;
    
    EXPECT_EQ(net_copy.address(), ipv6_address::parse("::"));
    EXPECT_EQ(net_copy.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    EXPECT_EQ(net_copy.hostmask(), ipv6_address::parse("::"));
    EXPECT_EQ(net_copy.prefixlen(), 128);
    EXPECT_EQ(net_copy.version(), ip_version::V6);
    net_copy = net;

    EXPECT_EQ(net_copy.address(), ipv6_address::parse("2001:db8::"));
    EXPECT_EQ(net_copy.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff::"));
    EXPECT_EQ(net_copy.hostmask(), ipv6_address::parse("::ffff:ffff"));
    EXPECT_EQ(net_copy.prefixlen(), 96);
}

using NetworkParserIpv6Params = TestWithParam<std::tuple<const char*, const char*, const char*, const char*, size_t>>;
TEST_P(NetworkParserIpv6Params, parse) {
    auto excepted_address = ipv6_address::parse(get<1>(GetParam()));
    auto excepted_netmask = ipv6_address::parse(get<2>(GetParam()));
    auto excepted_hostmask = ipv6_address::parse(get<3>(GetParam()));
    auto excepted_prefixlen = get<4>(GetParam());

    auto net = ipv6_network::parse(get<0>(GetParam()));
    
    EXPECT_EQ(net.address(), excepted_address);
    EXPECT_EQ(net.netmask(), excepted_netmask);
    EXPECT_EQ(net.hostmask(), excepted_hostmask);
    EXPECT_EQ(net.prefixlen(), excepted_prefixlen);

    std::string s1;
    ipv6_network net_from_stream;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> net_from_stream >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    EXPECT_EQ(net_from_stream.address(), excepted_address);
    EXPECT_EQ(net_from_stream.netmask(), excepted_netmask);
    EXPECT_EQ(net_from_stream.hostmask(), excepted_hostmask);
    EXPECT_EQ(net_from_stream.prefixlen(), excepted_prefixlen);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, NetworkParserIpv6Params,
    testing::Values(
        std::make_tuple("::1", "::1", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "::", 128),
        std::make_tuple("::1%scope", "::1%scope", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "::", 128),
        std::make_tuple("2001:db8::/32", "2001:db8::", "ffff:ffff::", "::ffff:ffff:ffff:ffff:ffff:ffff", 32),
        std::make_tuple("2001:db8::%scope/32", "2001:db8::%scope", "ffff:ffff::", "::ffff:ffff:ffff:ffff:ffff:ffff", 32),
        std::make_tuple("::/0", "::", "::", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", 0),
        std::make_tuple("::/128", "::", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "::", 128)
    ));

using NetworkParserIpv6NotStrictParams = TestWithParam<std::tuple<const char*, const char*, const char*, const char*, size_t>>;
TEST_P(NetworkParserIpv6NotStrictParams, parse_not_strict) {
    auto excepted_address = ipv6_address::parse(get<1>(GetParam()));
    auto excepted_netmask = ipv6_address::parse(get<2>(GetParam()));
    auto excepted_hostmask = ipv6_address::parse(get<3>(GetParam()));
    auto excepted_prefixlen = get<4>(GetParam());

    auto net = ipv6_network::parse(get<0>(GetParam()), false);
   
    EXPECT_EQ(net.address(), excepted_address);
    EXPECT_EQ(net.netmask(), excepted_netmask);
    EXPECT_EQ(net.hostmask(), excepted_hostmask);
    EXPECT_EQ(net.prefixlen(), excepted_prefixlen);

    std::string s1;
    ipv6_network net_from_stream;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> not_strict >> net_from_stream >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    EXPECT_EQ(net_from_stream.address(), excepted_address);
    EXPECT_EQ(net_from_stream.netmask(), excepted_netmask);
    EXPECT_EQ(net_from_stream.hostmask(), excepted_hostmask);
    EXPECT_EQ(net_from_stream.prefixlen(), excepted_prefixlen);
    ASSERT_EQ(s2, std::string("parser"));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, NetworkParserIpv6NotStrictParams,
    testing::Values(
        std::make_tuple("2001:db8::/16", "2001::", "ffff::", "::ffff:ffff:ffff:ffff:ffff:ffff:ffff", 16),
        std::make_tuple("2001:db8::/24", "2001:d00::", "ffff:ff00::", "0:ff:ffff:ffff:ffff:ffff:ffff:ffff", 24)
    ));

using NetworkFromAddressIpv6Params = TestWithParam<std::tuple<const char*, const char*, size_t, bool>>;
TEST_P(NetworkFromAddressIpv6Params, from_address) {
    auto address = ipv6_address::parse(get<1>(GetParam()));
    auto prefixlen = get<2>(GetParam());
    auto strict = get<3>(GetParam());

    auto expected = ipv6_network::parse(get<0>(GetParam()), strict);

    error_code err = error_code::NO_ERROR;
    auto actual = ipv6_network::from_address(address, err, prefixlen, strict);
    ASSERT_EQ(actual, expected);
    ASSERT_EQ(actual.address(), expected.address());
    ASSERT_EQ(actual.netmask(), expected.netmask());
    ASSERT_EQ(actual.hostmask(), expected.hostmask());
    ASSERT_EQ(actual.prefixlen(), expected.prefixlen());

    auto actual2 = ipv6_network::from_address(address, prefixlen, strict);
    ASSERT_EQ(actual2, expected);
    ASSERT_EQ(actual2.address(), expected.address());
    ASSERT_EQ(actual2.netmask(), expected.netmask());
    ASSERT_EQ(actual2.hostmask(), expected.hostmask());
    ASSERT_EQ(actual2.prefixlen(), expected.prefixlen());
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, NetworkFromAddressIpv6Params,
    testing::Values(
        std::make_tuple("2001:db8::/16", "2001:db8::", 16, false),
        std::make_tuple("2001:db8::/32", "2001:db8::", 32, true)
    ));

TEST(ipv6_network, from_address_error) {
    error_code err = error_code::NO_ERROR;
    auto actual = ipv6_network::from_address(ipv6_address::parse("2001:db8::"), err, 16);
    ASSERT_EQ(err, error_code::HAS_HOST_BITS_SET);
    ASSERT_EQ(actual.address(), ipv6_address::parse("::"));
    ASSERT_EQ(actual.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    ASSERT_EQ(actual.hostmask(), ipv6_address::parse("::"));
    ASSERT_EQ(actual.prefixlen(), 128);
    
#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_network = ipv6_network::from_address(ipv6_address::parse("2001:db8::"), 16);
    ASSERT_EQ(error_network.address(), ipv6_address::parse("::"));
    ASSERT_EQ(error_network.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    ASSERT_EQ(error_network.hostmask(), ipv6_address::parse("::"));
    ASSERT_EQ(error_network.prefixlen(), 128);
#else
    EXPECT_THAT(
        []() { ipv6_network::from_address(ipv6_address::parse("2001:db8::"), 16); },
        ThrowsMessage<parse_error>(StrEq("has host bits set in address 2001:db8::")));
#endif
}

using InvalidNetworkIpv6Params = TestWithParam<std::tuple<const char*, error_code, const char*>>;
TEST_P(InvalidNetworkIpv6Params, parse) {
    auto expected_network = get<0>(GetParam());
    auto expected_error_code = get<1>(GetParam());

    error_code err;
    ipv6_network::parse(expected_network, err);
    ASSERT_EQ(err, expected_error_code);

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_network = ipv6_network::parse(expected_network);

    EXPECT_EQ(error_network.address(), ipv6_address::parse("::"));
    ASSERT_EQ(error_network.netmask(), ipv6_address::parse("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"));
    ASSERT_EQ(error_network.hostmask(), ipv6_address::parse("::"));
    ASSERT_EQ(error_network.prefixlen(), 128);
#else
    EXPECT_THAT(
        [network=expected_network]() { ipv6_network::parse(network); },
        ThrowsMessage<parse_error>(StrEq(get<2>(GetParam()))));
    EXPECT_THAT(
        [network=expected_network]() { ipv6_network::parse(network); },
        Throws<parse_error>(Property(&parse_error::code, Eq(expected_error_code))));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, InvalidNetworkIpv6Params,
    testing::Values(
        std::make_tuple("/", error_code::EMPTY_NETMASK, "empty mask in address /"),
        std::make_tuple("2001:db8::/", error_code::EMPTY_NETMASK, "empty mask in address 2001:db8::/"),
        std::make_tuple("2001:db8::/129", error_code::INVALID_NETMASK, "is not a valid netmask in address 2001:db8::/129"),
        std::make_tuple("2001:db8::/255.255.255.255", error_code::INVALID_NETMASK, "is not a valid netmask in address 2001:db8::/255.255.255.255"),
        std::make_tuple("/%scope", error_code::INVALID_NETMASK, "is not a valid netmask in address /%scope"),
        std::make_tuple("/%scope8", error_code::INVALID_NETMASK, "is not a valid netmask in address /%scope8"),
        std::make_tuple("::1/::1", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/::1"),
        std::make_tuple("::1/1::", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/1::"),
        std::make_tuple("::1/-1", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/-1"),
        std::make_tuple("::1/+1", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/+1"),
        std::make_tuple("::1/ 1 ", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/ 1 "),
        std::make_tuple("::1/word", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/word"),
        std::make_tuple("::1/::", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1/::"),
        std::make_tuple("::1%scope/word", error_code::INVALID_NETMASK, "is not a valid netmask in address ::1%scope/word"),
        
        std::make_tuple("2001:db8::/24", error_code::HAS_HOST_BITS_SET, "has host bits set in address 2001:db8::/24"),
        std::make_tuple("2001:db8:://", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 2001:db8:://"),
        std::make_tuple("2001:db8:://128", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 2001:db8:://128"),
        std::make_tuple("2001:db8::/128/128", error_code::ONLY_ONE_SLASH_PERMITTED, "only one '/' permitted in address 2001:db8::/128/128"),

        std::make_tuple("/8", error_code::EMPTY_ADDRESS, "address cannot be empty"),
        std::make_tuple("google.com", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address google.com has invalid symbol"),
        std::make_tuple("1.2.3.4", error_code::LEAST_3_PARTS, "least 3 parts in address 1.2.3.4"),
        std::make_tuple("10/8", error_code::LEAST_3_PARTS, "least 3 parts in address 10/8"),
        std::make_tuple("1234:axy::b", error_code::PART_HAS_INVALID_SYMBOL, "in part 0 of address 1234:axy::b has invalid symbols"),
        std::make_tuple("google.com%scope", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address google.com%scope has invalid symbol"),
        std::make_tuple("1.2.3.4%scope", error_code::LEAST_3_PARTS, "least 3 parts in address 1.2.3.4%scope"),
        std::make_tuple("10%scope/8", error_code::LEAST_3_PARTS, "least 3 parts in address 10%scope/8"),
        std::make_tuple("1234:axy::b%scope", error_code::PART_HAS_INVALID_SYMBOL, "in part 0 of address 1234:axy::b%scope has invalid symbols")
    ));

TEST(ipv6_network, Comparison) {
    auto net1 = ipv6_network::parse("2001:db8::/96");
    auto net2 = ipv6_network::parse("2001:dc8::");
    auto net3 = ipv6_network::parse("2001:dc8::/128");
    
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

using ToStringNetworkIpv6Params = TestWithParam<std::tuple<const char*, const char*, const char*, const char*>>;
TEST_P(ToStringNetworkIpv6Params, to_string) {
    const auto expected_full = std::get<1>(GetParam());
    const auto expected_compact = std::get<2>(GetParam());
    const auto expected_compressed = std::get<3>(GetParam());

    const auto actual = ipv6_network::parse(std::get<0>(GetParam()));

    std::ostringstream ss_full; ss_full << full << actual;
    std::ostringstream ss_default; ss_default << actual;
    std::ostringstream ss_compact; ss_compact << compact << actual;
    std::ostringstream ss_compressed; ss_compressed << compressed << actual;
    
    ASSERT_EQ(actual.to_string(format::full), std::string(expected_full));
    ASSERT_EQ(actual.to_string(format::compact), std::string(expected_compact));
    ASSERT_EQ(actual.to_string(format::compressed), std::string(expected_compressed));
    ASSERT_EQ(actual.to_string(), std::string(expected_compressed));
    ASSERT_EQ((std::string) actual, std::string(expected_compressed));
    ASSERT_EQ(std::to_string(actual), std::string(expected_compressed));
    ASSERT_EQ(ss_full.str(), std::string(expected_full));
    ASSERT_EQ(ss_default.str(), std::string(expected_compressed));
    ASSERT_EQ(ss_compact.str(), std::string(expected_compact));
    ASSERT_EQ(ss_compressed.str(), std::string(expected_compressed));
}
INSTANTIATE_TEST_SUITE_P(
    ipv6_network, ToStringNetworkIpv6Params,
    testing::Values(
        std::make_tuple("2001:db8::", "2001:0db8:0000:0000:0000:0000:0000:0000/128", "2001:db8:0:0:0:0:0:0/128", "2001:db8::/128"),
        std::make_tuple("2001:db8::/32", "2001:0db8:0000:0000:0000:0000:0000:0000/32", "2001:db8:0:0:0:0:0:0/32", "2001:db8::/32"),
        std::make_tuple("2001:db8::%scope/32", "2001:0db8:0000:0000:0000:0000:0000:0000%scope/32", "2001:db8:0:0:0:0:0:0%scope/32", "2001:db8::%scope/32")
    ));

TEST(ipv6_network, Hash) {
    auto net1 = ipv6_network::parse("2001:db8::");
    auto net2 = ipv6_network::parse("2001:db8::/128");
    auto net3 = ipv6_network::parse("2001:db8::/32");
    auto net4 = ipv6_network::parse("2001:db8::%scope/32");
    auto hash_functor = std::hash<ipv6_network>{};

    ASSERT_EQ(net1.hash(), sizeof(size_t) == 8 ? 15427398023827107304ULL : 3105424018U);
    ASSERT_EQ(net2.hash(), sizeof(size_t) == 8 ? 15427398023827107304ULL : 3105424018U);
    ASSERT_EQ(net3.hash(), sizeof(size_t) == 8 ? 15947883579910446492ULL : 3052474356U);
    ASSERT_EQ(net4.hash(), sizeof(size_t) == 8 ? 1368690913789066344ULL : 589600209U);

    ASSERT_EQ(hash_functor(net1), sizeof(size_t) == 8 ? 15427398023827107304ULL : 3105424018U);
    ASSERT_EQ(hash_functor(net2), sizeof(size_t) == 8 ? 15427398023827107304ULL : 3105424018U);
    ASSERT_EQ(hash_functor(net3), sizeof(size_t) == 8 ? 15947883579910446492ULL : 3052474356U);
    ASSERT_EQ(hash_functor(net4), sizeof(size_t) == 8 ? 1368690913789066344ULL : 589600209U);
}

TEST(ipv6_network, Containers) {
    auto net1 = ipv6_network::parse("2001:db8::");
    auto net2 = ipv6_network::parse("2001:db8::/64");
    auto net3 = ipv6_network::parse("2001:db8::/32");

    std::vector<ipv6_network> vec;
    vec.push_back(net1);
    vec.push_back(net2);
    vec.emplace_back(net3);
    ASSERT_EQ(vec[0], net1);
    ASSERT_EQ(vec[1], net2);
    ASSERT_EQ(vec[2], net3);

    std::map<ipv6_network, int> map;
    map[net2] = 2;
    map[net1] = 1;
    map[net3] = 3;
    auto it = map.begin();
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(it++->first, net3);
    ASSERT_EQ(it++->first, net2);
    ASSERT_EQ(it++->first, net1);
    
    auto net3_with_scope = ipv6_network::parse("2001:db8::%scope/32");
    std::unordered_map<ipv6_network, int> unordered_map;
    unordered_map[net2] = 2;
    unordered_map[net1] = 1;
    unordered_map[net3] = 3;
    unordered_map[net3] = 4;
    unordered_map[net3_with_scope] = 0;
    ASSERT_EQ(unordered_map.size(), 4);
    ASSERT_EQ(unordered_map[net1], 1);
    ASSERT_EQ(unordered_map[net2], 2);
    ASSERT_EQ(unordered_map[net3], 4);
    ASSERT_EQ(unordered_map[net3_with_scope], 0);
}

TEST(ipv6_network, Swap) {
    auto net1 = ipv6_network::parse("2001:db8::");
    auto net2 = ipv6_network::parse("2001:db8::%scope/32");
    
    std::swap(net1, net2);

    ASSERT_EQ(net1, ipv6_network::parse("2001:db8::%scope/32"));
    ASSERT_EQ(net2, ipv6_network::parse("2001:db8::"));
}

TEST(ipv6_network, literals) {
    auto net1 = "2001:db8::/32"_ipv6_net;
    auto net2 = "0001:0002:0003:0004:0005:0006:0007:0008%123456789abcdefg/128"_ipv6_net;
    
    ASSERT_EQ(net1, ipv6_network::parse("2001:db8::/32"));
    ASSERT_EQ(net2, ipv6_network::parse("1:2:3:4:5:6:7:8%123456789abcdefg"));
}
