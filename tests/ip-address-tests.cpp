#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

TEST(ip_address, DefaultCtor) {
    IPADDRESS_CONSTEXPR ip_address ip;

    IPADDRESS_CONSTEXPR auto actual = ip.v4();

    EXPECT_EQ(ip.version(), ip_version::V4);
    EXPECT_EQ(ip.size(), 4);
    EXPECT_TRUE(ip.is_v4());
    EXPECT_FALSE(ip.is_v6());
    EXPECT_TRUE(actual.has_value());

    IPADDRESS_CONSTEXPR auto value = actual.value();
    EXPECT_EQ(value.to_uint(), 0);
}

TEST(ip_address, CopyCtor) {
    IPADDRESS_CONSTEXPR auto ip = ip_address::parse("127.0.0.1");
    IPADDRESS_CONSTEXPR auto ip_copy = ip;

    IPADDRESS_CONSTEXPR auto actual_ip = ip.v4();
    IPADDRESS_CONSTEXPR auto actual_ip_copy = ip_copy.v4();

    EXPECT_EQ(ip.version(), ip_version::V4);
    EXPECT_EQ(ip.size(), 4);
    EXPECT_TRUE(ip.is_v4());
    EXPECT_FALSE(ip.is_v6());
    EXPECT_TRUE(actual_ip.has_value());

    EXPECT_EQ(ip_copy.version(), ip_version::V4);
    EXPECT_EQ(ip_copy.size(), 4);
    EXPECT_TRUE(ip_copy.is_v4());
    EXPECT_FALSE(ip_copy.is_v6());
    EXPECT_TRUE(actual_ip_copy.has_value());

    IPADDRESS_CONSTEXPR auto value_ip = actual_ip.value();
    IPADDRESS_CONSTEXPR auto value_ip_copy = actual_ip_copy.value();
    EXPECT_EQ(value_ip.to_uint(), 0x7F000001);
    EXPECT_EQ(value_ip_copy.to_uint(), 0x7F000001);
}

TEST(ip_address, CopyOperator) {
    IPADDRESS_CONSTEXPR auto ip = 
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    ip_address::parse<"::ffff:0:0">();
#else
    ip_address::parse("::ffff:0:0");
#endif
    ip_address ip_copy = ip_address::parse("127.0.0.1");

    auto actual_ip4_copy = ip_copy.v4();
    auto actual_ip6_copy = ip_copy.v6();
    const auto value_ip4_copy = actual_ip4_copy.value();
    EXPECT_EQ(ip_copy.version(), ip_version::V4);
    EXPECT_EQ(ip_copy.size(), 4);
    EXPECT_TRUE(ip_copy.is_v4());
    EXPECT_FALSE(ip_copy.is_v6());
    EXPECT_TRUE(actual_ip4_copy.has_value());
    EXPECT_FALSE(actual_ip6_copy.has_value());
    EXPECT_EQ(value_ip4_copy.to_uint(), 0x7F000001);
    
    ip_copy = ip;
    actual_ip4_copy = ip_copy.v4();
    actual_ip6_copy = ip_copy.v6();
    const auto value_ip6_copy = actual_ip6_copy.value();
    EXPECT_EQ(ip_copy.version(), ip_version::V6);
    EXPECT_EQ(ip_copy.size(), 16);
    EXPECT_FALSE(ip_copy.is_v4());
    EXPECT_TRUE(ip_copy.is_v6());
    EXPECT_FALSE(actual_ip4_copy.has_value());
    EXPECT_TRUE(actual_ip6_copy.has_value());
    EXPECT_EQ(value_ip6_copy.to_uint(), uint128_t::from_string("281470681743360").value());
}

TEST(ip_address, Ctors) {
    IPADDRESS_CONSTEXPR auto ipv4 = ipv4_address::parse("192.168.0.1");
    IPADDRESS_CONSTEXPR auto ipv6 = ipv6_address::parse("2002:ac1d:2d64::1");
    constexpr ipv4_address::base_type ipv4_bytes {0xC0, 0xA8, 0x00, 0x01};
    constexpr ipv6_address::base_type ipv6_bytes { 
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 255, 255,
        0, 0, 0, 0
    };

    IPADDRESS_CONSTEXPR ip_address actual1(ipv4);
    IPADDRESS_CONSTEXPR ip_address actual2(ipv6);
    IPADDRESS_CONSTEXPR ip_address actual3(ipv4_bytes);
    IPADDRESS_CONSTEXPR ip_address actual4(ipv6_bytes);
    
    auto actual1_ip = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_ip.has_value());
    EXPECT_EQ(actual1_ip.value().to_uint(), 0xC0A80001);

    auto actual2_ip = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_ip.has_value());
    EXPECT_EQ(actual2_ip.value().to_uint(), uint128_t::from_string("42549171344950636613079587071710986241").value());
    
    auto actual3_ip = actual3.v4();
    EXPECT_EQ(actual3.version(), ip_version::V4);
    EXPECT_EQ(actual3.size(), 4);
    EXPECT_TRUE(actual3.is_v4());
    EXPECT_FALSE(actual3.is_v6());
    EXPECT_TRUE(actual3_ip.has_value());
    EXPECT_EQ(actual3_ip.value().to_uint(), 0xC0A80001);

    auto actual4_ip = actual4.v6();
    EXPECT_EQ(actual4.version(), ip_version::V6);
    EXPECT_EQ(actual4.size(), 16);
    EXPECT_FALSE(actual4.is_v4());
    EXPECT_TRUE(actual4.is_v6());
    EXPECT_TRUE(actual4_ip.has_value());
    EXPECT_EQ(actual4_ip.value().to_uint(), 281470681743360ULL);
}

TEST(ip_address, from_bytes) {
    constexpr ipv4_address::base_type ipv4_bytes {0xC0, 0xA8, 0x00, 0x01};
    constexpr ipv6_address::base_type ipv6_bytes { 
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 255, 255,
        0, 0, 0, 0
    };

    IPADDRESS_CONSTEXPR auto actual1 = ip_address::from_bytes(ipv4_bytes);
    IPADDRESS_CONSTEXPR auto actual2 = ip_address::from_bytes(ipv6_bytes);
    IPADDRESS_CONSTEXPR auto actual3 = ip_address::from_bytes(ipv4_bytes.data(), 3, ip_version::V4);
    IPADDRESS_CONSTEXPR auto actual4 = ip_address::from_bytes(ipv6_bytes.data(), 11, ip_version::V6);
    
    auto actual1_ip = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_ip.has_value());
    EXPECT_EQ(actual1_ip.value().to_uint(), 0xC0A80001);

    auto actual2_ip = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_ip.has_value());
    EXPECT_EQ(actual2_ip.value().to_uint(), 281470681743360ULL);
    
    auto actual3_ip = actual3.v4();
    EXPECT_EQ(actual3.version(), ip_version::V4);
    EXPECT_EQ(actual3.size(), 4);
    EXPECT_TRUE(actual3.is_v4());
    EXPECT_FALSE(actual3.is_v6());
    EXPECT_TRUE(actual3_ip.has_value());
    EXPECT_EQ(actual3_ip.value().to_uint(), 0xC0A80000);

    auto actual4_ip = actual4.v6();
    EXPECT_EQ(actual4.version(), ip_version::V6);
    EXPECT_EQ(actual4.size(), 16);
    EXPECT_FALSE(actual4.is_v4());
    EXPECT_TRUE(actual4.is_v6());
    EXPECT_TRUE(actual4_ip.has_value());
    EXPECT_EQ(actual4_ip.value().to_uint(), 280375465082880ULL);
}

TEST(ip_address, from_uint) {
    constexpr ipv4_address::uint_type ip1 = 0xC0A80001;
    IPADDRESS_CONSTEXPR ipv6_address::uint_type ip2 = 281470681743360ULL;

    IPADDRESS_CONSTEXPR auto actual1 = ip_address::from_uint(ip1);
    IPADDRESS_CONSTEXPR auto actual2 = ip_address::from_uint(ip2);
    
    auto actual1_ip = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_ip.has_value());
    EXPECT_EQ(actual1_ip.value().to_uint(), 0xC0A80001);

    auto actual2_ip = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_ip.has_value());
    EXPECT_EQ(actual2_ip.value().to_uint(), 281470681743360ULL);
}

TEST(ip_address, to_uint) {
    IPADDRESS_CONSTEXPR auto expected1 = 0xC0A80001;
    IPADDRESS_CONSTEXPR auto expected2 = uint128_t(2306595200482672640, 1);
    IPADDRESS_CONSTEXPR auto expected3 = 1;

    IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("192.168.0.1");
    IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("2002:ac1d:2d64::1");

    IPADDRESS_CONSTEXPR auto actual1 = ip1.to_uint128();
    IPADDRESS_CONSTEXPR auto actual2 = (uint128_t) ip1;
    IPADDRESS_CONSTEXPR auto actual3 = (uint32_t) ip1;
    IPADDRESS_CONSTEXPR auto actual4 = ip2.to_uint128();
    IPADDRESS_CONSTEXPR auto actual5 = (uint128_t) ip2;
    IPADDRESS_CONSTEXPR auto actual6 = (uint32_t) ip2;

    EXPECT_EQ(actual1, expected1);
    EXPECT_EQ(actual2, expected1);
    EXPECT_EQ(actual3, expected1);
    EXPECT_EQ(actual4, expected2);
    EXPECT_EQ(actual5, expected2);
    EXPECT_EQ(actual6, expected3);
}

TEST(ip_address, data) {
    byte_array<4> expected1 = { 192, 168, 0, 1 };
    byte_array<16> expected2 = { 
        0x20, 0x02, 0xac, 0x1d,
        0x2d, 0x64, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0x01 
    };

    static IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("192.168.0.1");
    static IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("2002:ac1d:2d64::1");

    IPADDRESS_CONSTEXPR auto data1 = ip1.data();
    IPADDRESS_CONSTEXPR auto data2 = ip2.data();

    byte_array<4> actual1 = { data1[0], data1[1], data1[2], data1[3] };
    byte_array<16> actual2 = { 
        data2[0], data2[1], data2[2], data2[3],
        data2[4], data2[5], data2[6], data2[7],
        data2[8], data2[9], data2[10], data2[11],
        data2[12], data2[13], data2[14], data2[15] 
    };

    EXPECT_EQ(actual1, expected1);
    EXPECT_EQ(actual2, expected2);
}

TEST(ip_address, parse_utf) {
    constexpr ipv6_address::base_type ip_bytes { 0x20, 0x01, 0x0D, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

    auto str1 = L"127.0.0.1";
    auto str2 = L"2001:db8::1";
    auto str3 = u"127.0.0.1";
    auto str4 = u"2001:db8::1";
    auto str5 = U"127.0.0.1";
    auto str6 = U"2001:db8::1";

    auto ip_wchar_1 = ip_address::parse(str1);
    auto ip_wchar_2 = ip_address::parse(str2);
    auto ip_char16_3 = ip_address::parse(str3);
    auto ip_char16_4 = ip_address::parse(str4);
    auto ip_char32_5 = ip_address::parse(str5);
    auto ip_char32_6 = ip_address::parse(str6);
    ASSERT_EQ(ip_wchar_1.v4().value().to_uint(), 0x7F000001);
    ASSERT_EQ(ip_wchar_2.v6().value().bytes(), ip_bytes);
    ASSERT_EQ(ip_char16_3.v4().value().to_uint(), 0x7F000001);
    ASSERT_EQ(ip_char16_4.v6().value().bytes(), ip_bytes);
    ASSERT_EQ(ip_char32_5.v4().value().to_uint(), 0x7F000001);
    ASSERT_EQ(ip_char32_6.v6().value().bytes(), ip_bytes);

#if __cpp_char8_t >= 201811L
    auto str7 = u8"127.0.0.1";
    auto str8 = u8"2001:db8::1";
    auto ip_char7_8 = ip_address::parse(str7);
    auto ip_char8_8 = ip_address::parse(str8);
    ASSERT_EQ(ip_char7_8.v4().value().to_uint(), 0x7F000001);
    ASSERT_EQ(ip_char8_8.v6().value().bytes(), ip_bytes);
#endif
}

TEST(ip_address, parse) {
    auto str1 = "100.64.0.0";
    auto str2 = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";

    IPADDRESS_CONSTEXPR auto actual1 = ip_address::parse("192.168.1.1");
    IPADDRESS_CONSTEXPR auto actual2 = ip_address::parse("fe80::1ff:fe23:4567:890a");
    auto actual3 = ip_address::parse(str1);
    auto actual4 = ip_address::parse(str2);
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    constexpr auto actual5 = ip_address::parse<"127.0.0.1">();
    constexpr auto actual6 = ip_address::parse<"fe80::1ff:fe23:4567:890a%eth2">();
#endif
    
    ip_address actual7;
    ip_address actual8;
    std::string s1;
    std::string s2;
    std::istringstream ss1("test: 255.0.42.42 parser");
    std::istringstream ss2("test: 2001:db8:0:0:1:0:0:1 parser");
    ss1 >> s1 >> actual7 >> s2;
    ss2 >> s1 >> actual8 >> s2;

    IPADDRESS_CONSTEXPR auto actual1_ip = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_ip.has_value());
    EXPECT_EQ(actual1_ip.value().to_uint(), 0xC0A80101);

    IPADDRESS_CONSTEXPR auto actual2_ip = actual2.v6();
    EXPECT_EQ(actual2.version(), ip_version::V6);
    EXPECT_EQ(actual2.size(), 16);
    EXPECT_FALSE(actual2.is_v4());
    EXPECT_TRUE(actual2.is_v6());
    EXPECT_TRUE(actual2_ip.has_value());
    EXPECT_EQ(actual2_ip.value().to_uint(), uint128_t::from_string("338288524927261089654163009981888563466").value());
    
    auto actual3_ip = actual3.v4();
    EXPECT_EQ(actual3.version(), ip_version::V4);
    EXPECT_EQ(actual3.size(), 4);
    EXPECT_TRUE(actual3.is_v4());
    EXPECT_FALSE(actual3.is_v6());
    EXPECT_TRUE(actual3_ip.has_value());
    EXPECT_EQ(actual3_ip.value().to_uint(), 0x64400000);
    
    auto actual4_ip = actual4.v6();
    EXPECT_EQ(actual4.version(), ip_version::V6);
    EXPECT_EQ(actual4.size(), 16);
    EXPECT_FALSE(actual4.is_v4());
    EXPECT_TRUE(actual4.is_v6());
    EXPECT_TRUE(actual4_ip.has_value());
    EXPECT_EQ(actual4_ip.value().to_uint(), uint128_t::from_string("42540766452641154071740215577757643572").value());
    
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    constexpr auto actual5_ip = actual5.v4();
    EXPECT_EQ(actual5.version(), ip_version::V4);
    EXPECT_EQ(actual5.size(), 4);
    EXPECT_TRUE(actual5.is_v4());
    EXPECT_FALSE(actual5.is_v6());
    EXPECT_TRUE(actual5_ip.has_value());
    EXPECT_EQ(actual5_ip.value().to_uint(), 0x7F000001);
    
    constexpr auto actual6_ip = actual6.v6();
    EXPECT_EQ(actual6.version(), ip_version::V6);
    EXPECT_EQ(actual6.size(), 16);
    EXPECT_FALSE(actual6.is_v4());
    EXPECT_TRUE(actual6.is_v6());
    EXPECT_TRUE(actual6_ip.has_value());
    EXPECT_EQ(actual6_ip.value().to_uint(), uint128_t::from_string("338288524927261089654163009981888563466").value());
#endif

    auto actual7_ip = actual7.v4();
    EXPECT_EQ(actual7.version(), ip_version::V4);
    EXPECT_EQ(actual7.size(), 4);
    EXPECT_TRUE(actual7.is_v4());
    EXPECT_FALSE(actual7.is_v6());
    EXPECT_TRUE(actual7_ip.has_value());
    EXPECT_EQ(actual7_ip.value().to_uint(), 0xFF002A2A);
    
    auto actual8_ip = actual8.v6();
    EXPECT_EQ(actual8.version(), ip_version::V6);
    EXPECT_EQ(actual8.size(), 16);
    EXPECT_FALSE(actual8.is_v4());
    EXPECT_TRUE(actual8.is_v6());
    EXPECT_TRUE(actual8_ip.has_value());
    EXPECT_EQ(actual8_ip.value().to_uint(), uint128_t::from_string("42540766411282592856904266426630537217").value());
}

using InvalidAddressParams = TestWithParam<std::tuple<const char*, error_code, const char*>>;
TEST_P(InvalidAddressParams, parse) {
    auto expected_address = get<0>(GetParam());
    auto expected_error_code = get<1>(GetParam());

    error_code err = error_code::no_error;
    ip_address::parse(expected_address, err);
    ASSERT_EQ(err, expected_error_code);

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_ip = ip_address::parse(expected_address);

    EXPECT_EQ(error_ip.to_uint128(), 0);
#elif IPADDRESS_CPP_VERSION >= 14
    EXPECT_THAT(
        [address=expected_address]() { ip_address::parse(address); },
        ThrowsMessage<parse_error>(StrEq(get<2>(GetParam()))));
    EXPECT_THAT(
        [address=expected_address]() { ip_address::parse(address); },
        Throws<parse_error>(Property(&parse_error::code, Eq(expected_error_code))));
#else
    ASSERT_THROW(ip_address::parse(expected_address), parse_error);
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ip_address, InvalidAddressParams,
    testing::Values(
        std::make_tuple("", error_code::empty_address, "address cannot be empty"),
        std::make_tuple("127", error_code::least_3_parts, "least 3 parts in address 127"),
        std::make_tuple("127.0.0.", error_code::empty_octet, "empty octet 0 in address 127.0.0."),
        std::make_tuple("127.0.0.1/24", error_code::octet_has_invalid_symbol, "in octet 0 of address 127.0.0.1/24 has invalid symbol"),
        std::make_tuple("127.0.0.1271", error_code::octet_more_3_characters, "in octet 0 of address 127.0.0.1271 more 3 characters"),
        std::make_tuple("192.168.0.999", error_code::octet_exceeded_255, "octet 0 of address 192.168.0.999 exceeded 255"),
        std::make_tuple("1.2.3.040", error_code::leading_0_are_not_permitted, "leading zeros are not permitted in octet 0 of address 1.2.3.040"),
        std::make_tuple("FEDC:9878%scope", error_code::least_3_parts, "least 3 parts in address FEDC:9878%scope"),
        std::make_tuple("10:9:8:7:6:5:4:3:42.42.42.42%scope", error_code::most_8_colons_permitted, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:42.42.42.42%scope"),
        std::make_tuple("2001:db8:::1%scope", error_code::most_one_double_colon_permitted, "at most one '::' permitted in address 2001:db8:::1%scope"),
        std::make_tuple("123456789abcfe::", error_code::part_is_more_4_chars, "in part 0 of address 123456789abcfe:: more 4 characters"),
        std::make_tuple("::1/24", error_code::part_has_invalid_symbol, "in part 3 of address ::1/24 has invalid symbols"),
        std::make_tuple("2001:db8:::1%scope", error_code::most_one_double_colon_permitted, "at most one '::' permitted in address 2001:db8:::1%scope"),
        std::make_tuple(":6:5:4:3:2:1::%scope", error_code::leading_colon_only_permitted_as_part_of_double_colon, "at leading ':' only permitted as part of '::' in address :6:5:4:3:2:1::%scope"),
        std::make_tuple("8:7:6:5:4:3:2:1::%scope", error_code::trailing_colon_only_permitted_as_part_of_double_colon, "at trailing ':' only permitted as part of '::' in address 8:7:6:5:4:3:2:1::%scope"),
        std::make_tuple("1:2:3:4:5::6:7:8%scope", error_code::expected_at_most_7_other_parts_with_double_colon, "expected at most 7 other parts with '::' in address 1:2:3:4:5::6:7:8%scope"),
        std::make_tuple("7:6:5:4:3:2:1%scope", error_code::exactly_8_parts_expected_without_double_colon, "exactly 8 parts expected without '::' in address 7:6:5:4:3:2:1%scope"),
        std::make_tuple("::%123456789abcdefgh", error_code::scope_id_is_too_long, "scope id is too long in address ::%123456789abcdefgh"),
        std::make_tuple("::1%scope_id/24", error_code::invalid_scope_id, "invalid scope id in address ::1%scope_id/24") 
    ));

template <typename T, size_t N1, size_t N2, size_t N3>
static void parse_unexpected_symbol(const T (&expected_address)[N1], const T (&expected_scope)[N2], const T (&str)[N3]) { // NOLINT(readability-function-cognitive-complexity)
    using tstring = std::basic_string<T, std::char_traits<T>, std::allocator<T>>;
    using tistringstream = std::basic_istringstream<T, std::char_traits<T>, std::allocator<T>>;

    auto ip = ip_address::parse("2001:db8::1");
    error_code err1 = error_code::no_error;
    error_code err2 = error_code::no_error;
    error_code err3 = error_code::no_error;
    error_code err4 = error_code::no_error;

    ip_address::parse(expected_address, err1);
    ip_address::parse(tstring(expected_address), err2);
    ip.set_scope_id(expected_scope, err3);
    ASSERT_FALSE(ip.get_scope_id());
    ip.set_scope_id(tstring(expected_scope), err4);
    ASSERT_FALSE(ip.get_scope_id());
    ASSERT_EQ(err1, error_code::unexpected_symbol);
    ASSERT_EQ(err2, error_code::unexpected_symbol);
    ASSERT_EQ(err3, error_code::unexpected_symbol);
    ASSERT_EQ(err4, error_code::unexpected_symbol);

    tistringstream ss(str);
    ipv6_address ip1, ip2, ip3;
    ss >> ip1;
    ASSERT_FALSE(ss.fail());
    ASSERT_EQ(ip1, ipv6_address::parse("2001:db8::1%123"));
    ss >> ip2;
    ASSERT_TRUE(ss.fail());
    ASSERT_EQ(ip2, ipv6_address::parse("::"));
    ss.clear();
    ss >> ip3;
    ASSERT_TRUE(ss.fail());
    ASSERT_EQ(ip3, ipv6_address::parse("::"));

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_ip1 = ip_address::parse(expected_address);
    auto error_ip2 = ip_address::parse(tstring(expected_address));
    ASSERT_EQ(error_ip1.to_uint128(), 0);
    ASSERT_EQ(error_ip2.to_uint128(), 0);
    ip.set_scope_id(expected_scope);
    ASSERT_FALSE(ip.get_scope_id());
    ip.set_scope_id(tstring(expected_scope));
    ASSERT_FALSE(ip.get_scope_id());
#elif IPADDRESS_CPP_VERSION >= 14
    EXPECT_THAT(
        [address=expected_address]() { ip_address::parse(address); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+10348} in string 200{U+10348}:d{U+d55c}8::1")));
    EXPECT_THAT(
        [address=expected_address]() { ip_address::parse(tstring(address)); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+10348} in string 200{U+10348}:d{U+d55c}8::1")));
    EXPECT_THAT(
        [address=expected_address]() { ip_address::parse(address); },
        Throws<parse_error>(Property(&parse_error::code, Eq(error_code::unexpected_symbol))));
    EXPECT_THAT(
        [address=expected_address]() { ip_address::parse(tstring(address)); },
        Throws<parse_error>(Property(&parse_error::code, Eq(error_code::unexpected_symbol))));
    EXPECT_THAT(
        [=]() { ip_address(ip).set_scope_id(expected_scope); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+d55c} in string 12{U+d55c}3")));
    EXPECT_THAT(
        [=]() { ip_address(ip).set_scope_id(tstring(expected_scope)); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+d55c} in string 12{U+d55c}3")));
    EXPECT_THAT(
        [=]() { ip_address(ip).set_scope_id(expected_scope); },
        Throws<parse_error>(Property(&parse_error::code, Eq(error_code::unexpected_symbol))));
    EXPECT_THAT(
        [=]() { ip_address(ip).set_scope_id(tstring(expected_scope)); },
        Throws<parse_error>(Property(&parse_error::code, Eq(error_code::unexpected_symbol))));
#else // googletest EXPECT_THAT is not supported in cpp less than 14
    ASSERT_THROW(ip_address::parse(expected_address), parse_error);
    ASSERT_THROW((ip_address::parse(tstring(expected_address))), parse_error);
    ASSERT_THROW(ip.set_scope_id(expected_scope), parse_error);
    ASSERT_THROW((ip.set_scope_id(tstring(expected_scope))), parse_error);
#endif
}
#define PARSE_UNEXPECTED_SYMBOL(unicode) parse_unexpected_symbol(unicode##"200\U00010348:d\ud55c8::1", unicode##"12\ud55c3", unicode##"2001:db8::1%123 200\U00010348:d\ud55c8::1 2001:db8::1%12\ud55c3")

#if __cpp_char8_t >= 201811L
TEST(ip_address, ParseUnexpectedUtf8) {
    PARSE_UNEXPECTED_SYMBOL(u8);
}
#endif

TEST(ip_address, ParseUnexpectedUtf16) {
    PARSE_UNEXPECTED_SYMBOL(u);
}

TEST(ip_address, ParseUnexpectedUtf32) {
    PARSE_UNEXPECTED_SYMBOL(U);
}

TEST(ip_address, ParseUnexpectedWideChar) {
    PARSE_UNEXPECTED_SYMBOL(L);
}

TEST(ip_address, Comparison) {
    IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("127.240.0.1");
    IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("2001:db8::1");
    IPADDRESS_CONSTEXPR auto ip3 = ip_address::parse("2001:db8::2");

    ASSERT_TRUE(ip1 < ip2);
    ASSERT_TRUE(ip1 <= ip2);
    ASSERT_FALSE(ip1 > ip2);
    ASSERT_FALSE(ip1 >= ip2);
    ASSERT_FALSE(ip1 == ip2);
    ASSERT_TRUE(ip1 != ip2);
    
    ASSERT_FALSE(ip2 < ip1);
    ASSERT_FALSE(ip2 <= ip1);
    ASSERT_TRUE(ip2 > ip1);
    ASSERT_TRUE(ip2 >= ip1);
    ASSERT_FALSE(ip2 == ip1);
    ASSERT_TRUE(ip2 != ip1);

    ASSERT_FALSE(ip3 < ip2);
    ASSERT_FALSE(ip3 <= ip2);
    ASSERT_TRUE(ip3 > ip2);
    ASSERT_TRUE(ip3 >= ip2);
    ASSERT_FALSE(ip3 == ip2);
    ASSERT_TRUE(ip3 != ip2);
}

TEST(ip_address, to_string) {
    IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("127.240.0.1");
    IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("fe80::1ff:fe23:4567:890a%eth2");

    const std::string expected_address = "127.240.0.1";
    const char* expected_full_2 = "fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2";
    const char* expected_compact_2 = "fe80:0:0:0:1ff:fe23:4567:890a%eth2";
    const char* expected_compressed_2 = "fe80::1ff:fe23:4567:890a%eth2";
    const char* expected_compressed_upper_2 = "FE80::1FF:FE23:4567:890A%eth2";

    std::ostringstream ss_full; ss_full << full << ip1;
    std::ostringstream ss_default; ss_default << ip1;
    std::ostringstream ss_compact; ss_compact << compact << ip1;
    std::ostringstream ss_compressed; ss_compressed << compressed << ip1;
    std::ostringstream ss_compressed_upper; ss_compressed_upper << std::uppercase << compressed << ip1;

    std::ostringstream ss_full_2; ss_full_2 << full << ip2;
    std::ostringstream ss_default_2; ss_default_2 << ip2;
    std::ostringstream ss_compact_2; ss_compact_2 << compact << ip2;
    std::ostringstream ss_compressed_2; ss_compressed_2 << compressed << ip2;
    std::ostringstream ss_compressed_upper_2; ss_compressed_upper_2 << std::uppercase << compressed << ip2;
    
    ASSERT_EQ(ip1.to_string(format::full), expected_address);
    ASSERT_EQ(ip1.to_string(format::compact), expected_address);
    ASSERT_EQ(ip1.to_string(format::compressed),expected_address);
    ASSERT_EQ(ip1.to_string(), expected_address);
    ASSERT_EQ((std::string) ip1, expected_address);
    ASSERT_EQ(std::to_string(ip1), expected_address);
    ASSERT_EQ(ss_full.str(),expected_address);
    ASSERT_EQ(ss_default.str(), expected_address);
    ASSERT_EQ(ss_compact.str(), expected_address);
    ASSERT_EQ(ss_compressed.str(), expected_address);
    ASSERT_EQ(ss_compressed_upper.str(), expected_address);

    ASSERT_EQ(ip2.to_string(format::full), expected_full_2);
    ASSERT_EQ(ip2.to_string(format::compact), expected_compact_2);
    ASSERT_EQ(ip2.to_string(format::compressed), expected_compressed_2);
    ASSERT_EQ(ip2.to_string(), expected_compressed_2);
    ASSERT_EQ((std::string) ip2, expected_compressed_2);
    ASSERT_EQ(std::to_string(ip2), expected_compressed_2);
    ASSERT_EQ(ss_full_2.str(), expected_full_2);
    ASSERT_EQ(ss_default_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compact_2.str(), expected_compact_2);
    ASSERT_EQ(ss_compressed_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compressed_upper_2.str(), expected_compressed_upper_2);
}

TEST(ip_address, to_wstring) {
    IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("127.240.0.1");
    IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("fe80::1ff:fe23:4567:890a%eth2");

    const std::wstring expected_address = L"127.240.0.1";
    const wchar_t* expected_full_2 = L"fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2";
    const wchar_t* expected_compact_2 = L"fe80:0:0:0:1ff:fe23:4567:890a%eth2";
    const wchar_t* expected_compressed_2 = L"fe80::1ff:fe23:4567:890a%eth2";
    const wchar_t* expected_compressed_upper_2 = L"FE80::1FF:FE23:4567:890A%eth2";

    std::wostringstream ss_full; ss_full << full << ip1;
    std::wostringstream ss_default; ss_default << ip1;
    std::wostringstream ss_compact; ss_compact << compact << ip1;
    std::wostringstream ss_compressed; ss_compressed << compressed << ip1;
    std::wostringstream ss_compressed_upper; ss_compressed_upper << std::uppercase << compressed << ip1;

    std::wostringstream ss_full_2; ss_full_2 << full << ip2;
    std::wostringstream ss_default_2; ss_default_2 << ip2;
    std::wostringstream ss_compact_2; ss_compact_2 << compact << ip2;
    std::wostringstream ss_compressed_2; ss_compressed_2 << compressed << ip2;
    std::wostringstream ss_compressed_upper_2; ss_compressed_upper_2 << std::uppercase << compressed << ip2;
    
    ASSERT_EQ(ip1.to_wstring(format::full), expected_address);
    ASSERT_EQ(ip1.to_wstring(format::compact), expected_address);
    ASSERT_EQ(ip1.to_wstring(format::compressed),expected_address);
    ASSERT_EQ(ip1.to_wstring(), expected_address);
    ASSERT_EQ((std::wstring) ip1, expected_address);
    ASSERT_EQ(std::to_wstring(ip1), expected_address);
    ASSERT_EQ(ss_full.str(),expected_address);
    ASSERT_EQ(ss_default.str(), expected_address);
    ASSERT_EQ(ss_compact.str(), expected_address);
    ASSERT_EQ(ss_compressed.str(), expected_address);
    ASSERT_EQ(ss_compressed_upper.str(), expected_address);

    ASSERT_EQ(ip2.to_wstring(format::full), expected_full_2);
    ASSERT_EQ(ip2.to_wstring(format::compact), expected_compact_2);
    ASSERT_EQ(ip2.to_wstring(format::compressed), expected_compressed_2);
    ASSERT_EQ(ip2.to_wstring(), expected_compressed_2);
    ASSERT_EQ((std::wstring) ip2, expected_compressed_2);
    ASSERT_EQ(std::to_wstring(ip2), expected_compressed_2);
    ASSERT_EQ(ss_full_2.str(), expected_full_2);
    ASSERT_EQ(ss_default_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compact_2.str(), expected_compact_2);
    ASSERT_EQ(ss_compressed_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compressed_upper_2.str(), expected_compressed_upper_2);
}

TEST(ip_address, to_u16string) {
    using u16ostringstream = std::basic_ostringstream<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>>;

    IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("127.240.0.1");
    IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("fe80::1ff:fe23:4567:890a%eth2");

    const std::u16string expected_address = u"127.240.0.1";
    const char16_t* expected_full_2 = u"fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2";
    const char16_t* expected_compact_2 = u"fe80:0:0:0:1ff:fe23:4567:890a%eth2";
    const char16_t* expected_compressed_2 = u"fe80::1ff:fe23:4567:890a%eth2";
    const char16_t* expected_compressed_upper_2 = u"FE80::1FF:FE23:4567:890A%eth2";

    u16ostringstream ss_full; ss_full << full << ip1;
    u16ostringstream ss_default; ss_default << ip1;
    u16ostringstream ss_compact; ss_compact << compact << ip1;
    u16ostringstream ss_compressed; ss_compressed << compressed << ip1;
    u16ostringstream ss_compressed_upper; ss_compressed_upper << std::uppercase << compressed << ip1;

    u16ostringstream ss_full_2; ss_full_2 << full << ip2;
    u16ostringstream ss_default_2; ss_default_2 << ip2;
    u16ostringstream ss_compact_2; ss_compact_2 << compact << ip2;
    u16ostringstream ss_compressed_2; ss_compressed_2 << compressed << ip2;
    u16ostringstream ss_compressed_upper_2; ss_compressed_upper_2 << std::uppercase << compressed << ip2;
    
    ASSERT_EQ(ip1.to_u16string(format::full), expected_address);
    ASSERT_EQ(ip1.to_u16string(format::compact), expected_address);
    ASSERT_EQ(ip1.to_u16string(format::compressed),expected_address);
    ASSERT_EQ(ip1.to_u16string(), expected_address);
    ASSERT_EQ((std::u16string) ip1, expected_address);
    ASSERT_EQ(ss_full.str(),expected_address);
    ASSERT_EQ(ss_default.str(), expected_address);
    ASSERT_EQ(ss_compact.str(), expected_address);
    ASSERT_EQ(ss_compressed.str(), expected_address);
    ASSERT_EQ(ss_compressed_upper.str(), expected_address);

    ASSERT_EQ(ip2.to_u16string(format::full), expected_full_2);
    ASSERT_EQ(ip2.to_u16string(format::compact), expected_compact_2);
    ASSERT_EQ(ip2.to_u16string(format::compressed), expected_compressed_2);
    ASSERT_EQ(ip2.to_u16string(), expected_compressed_2);
    ASSERT_EQ((std::u16string) ip2, expected_compressed_2);
    ASSERT_EQ(ss_full_2.str(), expected_full_2);
    ASSERT_EQ(ss_default_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compact_2.str(), expected_compact_2);
    ASSERT_EQ(ss_compressed_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compressed_upper_2.str(), expected_compressed_upper_2);
}

TEST(ip_address, to_u32string) {
    using u32ostringstream = std::basic_ostringstream<char32_t, std::char_traits<char32_t>, std::allocator<char32_t>>;

    IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("127.240.0.1");
    IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("fe80::1ff:fe23:4567:890a%eth2");

    const std::u32string expected_address = U"127.240.0.1";
    const char32_t* expected_full_2 = U"fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2";
    const char32_t* expected_compact_2 = U"fe80:0:0:0:1ff:fe23:4567:890a%eth2";
    const char32_t* expected_compressed_2 = U"fe80::1ff:fe23:4567:890a%eth2";
    const char32_t* expected_compressed_upper_2 = U"FE80::1FF:FE23:4567:890A%eth2";

    u32ostringstream ss_full; ss_full << full << ip1;
    u32ostringstream ss_default; ss_default << ip1;
    u32ostringstream ss_compact; ss_compact << compact << ip1;
    u32ostringstream ss_compressed; ss_compressed << compressed << ip1;
    u32ostringstream ss_compressed_upper; ss_compressed_upper << std::uppercase << compressed << ip1;

    u32ostringstream ss_full_2; ss_full_2 << full << ip2;
    u32ostringstream ss_default_2; ss_default_2 << ip2;
    u32ostringstream ss_compact_2; ss_compact_2 << compact << ip2;
    u32ostringstream ss_compressed_2; ss_compressed_2 << compressed << ip2;
    u32ostringstream ss_compressed_upper_2; ss_compressed_upper_2 << std::uppercase << compressed << ip2;
    
    ASSERT_EQ(ip1.to_u32string(format::full), expected_address);
    ASSERT_EQ(ip1.to_u32string(format::compact), expected_address);
    ASSERT_EQ(ip1.to_u32string(format::compressed),expected_address);
    ASSERT_EQ(ip1.to_u32string(), expected_address);
    ASSERT_EQ((std::u32string) ip1, expected_address);
    ASSERT_EQ(ss_full.str(),expected_address);
    ASSERT_EQ(ss_default.str(), expected_address);
    ASSERT_EQ(ss_compact.str(), expected_address);
    ASSERT_EQ(ss_compressed.str(), expected_address);
    ASSERT_EQ(ss_compressed_upper.str(), expected_address);

    ASSERT_EQ(ip2.to_u32string(format::full), expected_full_2);
    ASSERT_EQ(ip2.to_u32string(format::compact), expected_compact_2);
    ASSERT_EQ(ip2.to_u32string(format::compressed), expected_compressed_2);
    ASSERT_EQ(ip2.to_u32string(), expected_compressed_2);
    ASSERT_EQ((std::u32string) ip2, expected_compressed_2);
    ASSERT_EQ(ss_full_2.str(), expected_full_2);
    ASSERT_EQ(ss_default_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compact_2.str(), expected_compact_2);
    ASSERT_EQ(ss_compressed_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compressed_upper_2.str(), expected_compressed_upper_2);
}

#if __cpp_char8_t >= 201811L
TEST(ip_address, to_u8string) {
    using u8ostringstream = std::basic_ostringstream<char8_t, std::char_traits<char8_t>, std::allocator<char8_t>>;

    IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("127.240.0.1");
    IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("fe80::1ff:fe23:4567:890a%eth2");

    const std::u8string expected_address = u8"127.240.0.1";
    const char8_t* expected_full_2 = u8"fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2";
    const char8_t* expected_compact_2 = u8"fe80:0:0:0:1ff:fe23:4567:890a%eth2";
    const char8_t* expected_compressed_2 = u8"fe80::1ff:fe23:4567:890a%eth2";
    const char8_t* expected_compressed_upper_2 = u8"FE80::1FF:FE23:4567:890A%eth2";

    u8ostringstream ss_full; ss_full << full << ip1;
    u8ostringstream ss_default; ss_default << ip1;
    u8ostringstream ss_compact; ss_compact << compact << ip1;
    u8ostringstream ss_compressed; ss_compressed << compressed << ip1;
    u8ostringstream ss_compressed_upper; ss_compressed_upper << std::uppercase << compressed << ip1;

    u8ostringstream ss_full_2; ss_full_2 << full << ip2;
    u8ostringstream ss_default_2; ss_default_2 << ip2;
    u8ostringstream ss_compact_2; ss_compact_2 << compact << ip2;
    u8ostringstream ss_compressed_2; ss_compressed_2 << compressed << ip2;
    u8ostringstream ss_compressed_upper_2; ss_compressed_upper_2 << std::uppercase << compressed << ip2;
    
    ASSERT_EQ(ip1.to_u8string(format::full), expected_address);
    ASSERT_EQ(ip1.to_u8string(format::compact), expected_address);
    ASSERT_EQ(ip1.to_u8string(format::compressed),expected_address);
    ASSERT_EQ(ip1.to_u8string(), expected_address);
    ASSERT_EQ((std::u8string) ip1, expected_address);
    ASSERT_EQ(ss_full.str(),expected_address);
    ASSERT_EQ(ss_default.str(), expected_address);
    ASSERT_EQ(ss_compact.str(), expected_address);
    ASSERT_EQ(ss_compressed.str(), expected_address);
    ASSERT_EQ(ss_compressed_upper.str(), expected_address);

    ASSERT_EQ(ip2.to_u8string(format::full), expected_full_2);
    ASSERT_EQ(ip2.to_u8string(format::compact), expected_compact_2);
    ASSERT_EQ(ip2.to_u8string(format::compressed), expected_compressed_2);
    ASSERT_EQ(ip2.to_u8string(), expected_compressed_2);
    ASSERT_EQ((std::u8string) ip2, expected_compressed_2);
    ASSERT_EQ(ss_full_2.str(), expected_full_2);
    ASSERT_EQ(ss_default_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compact_2.str(), expected_compact_2);
    ASSERT_EQ(ss_compressed_2.str(), expected_compressed_2);
    ASSERT_EQ(ss_compressed_upper_2.str(), expected_compressed_upper_2);
}
#endif

TEST(ip_address, Hash) {
    constexpr auto hash_functor = std::hash<ip_address>{};

    IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("127.0.0.1");
    IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("2001:db8::1");

    IPADDRESS_CONSTEXPR auto hash1 = ip1.hash();
    IPADDRESS_CONSTEXPR auto hash2 = ip2.hash();
    IPADDRESS_CONSTEXPR auto hash3 = hash_functor(ip1);
    IPADDRESS_CONSTEXPR auto hash4 = hash_functor(ip2);

    ASSERT_EQ(hash1, sizeof(size_t) == 8 ? 2753421670903790749ULL : 4009513643U);
    ASSERT_EQ(hash2, sizeof(size_t) == 8 ? 6222153021643990098ULL : 1539407693U);
    ASSERT_EQ(hash3, sizeof(size_t) == 8 ? 2753421670903790749ULL : 4009513643U);
    ASSERT_EQ(hash4, sizeof(size_t) == 8 ? 6222153021643990098ULL : 1539407693U);
}

TEST(ip_address, Containers) {
    IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("127.0.0.1");
    IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("2001:db8::1");
    IPADDRESS_CONSTEXPR auto ip3 = ip_address::parse("2001:db8::2");

    std::vector<ip_address> vec;
    vec.push_back(ip1);
    vec.push_back(ip2);
    vec.emplace_back(ip3);
    ASSERT_EQ(vec[0], ip1);
    ASSERT_EQ(vec[1], ip2);
    ASSERT_EQ(vec[2], ip3);

    std::map<ip_address, int> map;
    map[ip2] = 2;
    map[ip1] = 1;
    map[ip3] = 3;
    auto it = map.begin();
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(it++->first, ip1);
    ASSERT_EQ(it++->first, ip2);
    ASSERT_EQ(it++->first, ip3);
    
    auto ip3_with_scope = ip_address::parse("2001:db8::2%scope");
    std::unordered_map<ip_address, int> unordered_map;
    unordered_map[ip2] = 2;
    unordered_map[ip1] = 1;
    unordered_map[ip3] = 3;
    unordered_map[ip3] = 4;
    unordered_map[ip3_with_scope] = 0;
    ASSERT_EQ(unordered_map.size(), 4);
    ASSERT_EQ(unordered_map[ip1], 1);
    ASSERT_EQ(unordered_map[ip2], 2);
    ASSERT_EQ(unordered_map[ip3], 4);
    ASSERT_EQ(unordered_map[ip3_with_scope], 0);
}

TEST(ip_address, Swap) {
    auto ip1 = ip_address::parse("127.0.0.1");
    auto ip2 = ip_address::parse("2001:db8::2%test");
    
    std::swap(ip1, ip2);

    ASSERT_EQ(ip1, ip_address::parse("2001:db8::2%test"));
    ASSERT_EQ(ip2, ip_address::parse("127.0.0.1"));
}

TEST(ip_address, reverse_pointer) {
    IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("127.0.0.1");
    IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("2001:db8::1");
    
    const auto actual1 = ip1.reverse_pointer();
    const auto actual2 = ip2.reverse_pointer();

    ASSERT_EQ(actual1, "1.0.0.127.in-addr.arpa");
    ASSERT_EQ(actual2, "1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.8.b.d.0.1.0.0.2.ip6.arpa");
}

TEST(ip_address, ipv4_mapped) {
    IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("127.0.0.1");
    IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("::ffff:192.168.1.1%test");
    IPADDRESS_CONSTEXPR auto ip3 = ip_address::parse("::c0a8:101");

    IPADDRESS_CONSTEXPR auto actual1 = ip1.ipv4_mapped();
    IPADDRESS_CONSTEXPR auto actual2 = ip2.ipv4_mapped();
    IPADDRESS_CONSTEXPR auto actual3 = ip3.ipv4_mapped();

    IPADDRESS_CONSTEXPR auto actual_has_value1 = actual1.has_value();
    IPADDRESS_CONSTEXPR auto actual_has_value2 = actual2.has_value();
    IPADDRESS_CONSTEXPR auto actual_has_value3 = actual3.has_value();
    ASSERT_FALSE(actual_has_value1);
    ASSERT_TRUE(actual_has_value2);
    ASSERT_FALSE(actual_has_value3);

    IPADDRESS_CONSTEXPR ip_address ipv4 = actual2.value();
    ASSERT_EQ(ipv4, ip_address::parse("192.168.1.1"));
}

TEST(ip_address, sixtofour) {
    IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("127.0.0.1");
    IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("2002:ac1d:2d64::1");
    IPADDRESS_CONSTEXPR auto ip3 = ip_address::parse("2000:ac1d:2d64::1");

    IPADDRESS_CONSTEXPR auto actual1 = ip1.sixtofour();
    IPADDRESS_CONSTEXPR auto actual2 = ip2.sixtofour();
    IPADDRESS_CONSTEXPR auto actual3 = ip3.sixtofour();

    IPADDRESS_CONSTEXPR auto actual_has_value1 = actual1.has_value();
    IPADDRESS_CONSTEXPR auto actual_has_value2 = actual2.has_value();
    IPADDRESS_CONSTEXPR auto actual_has_value3 = actual3.has_value();
    ASSERT_FALSE(actual_has_value1);
    ASSERT_TRUE(actual_has_value2);
    ASSERT_FALSE(actual_has_value3);

    IPADDRESS_CONSTEXPR ip_address ipv4 = actual2.value();
    ASSERT_EQ(ipv4, ip_address::parse("172.29.45.100"));
}

TEST(ip_address, teredo) {
    IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("127.0.0.1");
    IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("2001:0000:4136:e378:8000:63bf:3fff:fdd2");
    IPADDRESS_CONSTEXPR auto ip3 = ip_address::parse("2000::4136:e378:8000:63bf:3fff:fdd2");

    IPADDRESS_CONSTEXPR auto actual1 = ip1.teredo();
    IPADDRESS_CONSTEXPR auto actual2 = ip2.teredo();
    IPADDRESS_CONSTEXPR auto actual3 = ip3.teredo();

    IPADDRESS_CONSTEXPR auto actual_has_value1 = actual1.has_value();
    IPADDRESS_CONSTEXPR auto actual_has_value2 = actual2.has_value();
    IPADDRESS_CONSTEXPR auto actual_has_value3 = actual3.has_value();
    ASSERT_FALSE(actual_has_value1);
    ASSERT_TRUE(actual_has_value2);
    ASSERT_FALSE(actual_has_value3);

    IPADDRESS_CONSTEXPR ip_address server = actual2.value().first;
    IPADDRESS_CONSTEXPR ip_address client = actual2.value().second;
    ASSERT_EQ(server, ip_address::parse("65.54.227.120"));
    ASSERT_EQ(client, ip_address::parse("192.0.2.45"));
}

TEST(ip_address, is_prop) {
    IPADDRESS_CONSTEXPR auto ip1 = ip_address::parse("224.1.1.1").is_multicast();
    IPADDRESS_CONSTEXPR auto ip2 = ip_address::parse("240.0.0.0").is_multicast();
    IPADDRESS_CONSTEXPR auto ip3 = ip_address::parse("ffff::").is_multicast();
    IPADDRESS_CONSTEXPR auto ip4 = ip_address::parse("fdff::").is_multicast();
    IPADDRESS_CONSTEXPR auto ip5 = ip_address::parse("192.168.1.1").is_private();
    IPADDRESS_CONSTEXPR auto ip6 = ip_address::parse("192.169.0.0").is_private();
    IPADDRESS_CONSTEXPR auto ip7 = ip_address::parse("fc00::").is_private();
    IPADDRESS_CONSTEXPR auto ip8 = ip_address::parse("fbff:ffff::").is_private();
    IPADDRESS_CONSTEXPR auto ip9 = ip_address::parse("192.0.7.1").is_global();
    IPADDRESS_CONSTEXPR auto ip10 = ip_address::parse("203.0.113.1").is_global();
    IPADDRESS_CONSTEXPR auto ip11 = ip_address::parse("200::1").is_global();
    IPADDRESS_CONSTEXPR auto ip12 = ip_address::parse("fc00::").is_global();
    IPADDRESS_CONSTEXPR auto ip13 = ip_address::parse("240.0.0.1").is_reserved();
    IPADDRESS_CONSTEXPR auto ip14 = ip_address::parse("239.255.255.255").is_reserved();
    IPADDRESS_CONSTEXPR auto ip15 = ip_address::parse("100::").is_reserved();
    IPADDRESS_CONSTEXPR auto ip16 = ip_address::parse("ffff::").is_reserved();
    IPADDRESS_CONSTEXPR auto ip17 = ip_address::parse("127.42.0.0").is_loopback();
    IPADDRESS_CONSTEXPR auto ip18 = ip_address::parse("128.0.0.0").is_loopback();
    IPADDRESS_CONSTEXPR auto ip19 = ip_address::parse("::1").is_loopback();
    IPADDRESS_CONSTEXPR auto ip20 = ip_address::parse("::2").is_loopback();
    IPADDRESS_CONSTEXPR auto ip21 = ip_address::parse("169.254.100.200").is_link_local();
    IPADDRESS_CONSTEXPR auto ip22 = ip_address::parse("169.255.100.200").is_link_local();
    IPADDRESS_CONSTEXPR auto ip23 = ip_address::parse("fea0::").is_link_local();
    IPADDRESS_CONSTEXPR auto ip24 = ip_address::parse("fe7f:ffff::").is_link_local();
    IPADDRESS_CONSTEXPR auto ip25 = ip_address::parse("0.0.0.0").is_unspecified();
    IPADDRESS_CONSTEXPR auto ip26 = ip_address::parse("127.0.0.1").is_unspecified();
    IPADDRESS_CONSTEXPR auto ip27 = ip_address::parse("::").is_unspecified();
    IPADDRESS_CONSTEXPR auto ip28 = ip_address::parse("::1").is_unspecified();
    IPADDRESS_CONSTEXPR auto ip29 = ip_address::parse("127.0.0.1").is_site_local();
    IPADDRESS_CONSTEXPR auto ip30 = ip_address::parse("0.0.0.0").is_site_local();
    IPADDRESS_CONSTEXPR auto ip31 = ip_address::parse("fecf::").is_site_local();
    IPADDRESS_CONSTEXPR auto ip32 = ip_address::parse("fbf:ffff::").is_site_local();
    
    ASSERT_TRUE(ip1);
    ASSERT_FALSE(ip2);
    ASSERT_TRUE(ip3);
    ASSERT_FALSE(ip4);
    ASSERT_TRUE(ip5);
    ASSERT_FALSE(ip6);
    ASSERT_TRUE(ip7);
    ASSERT_FALSE(ip8);
    ASSERT_TRUE(ip9);
    ASSERT_FALSE(ip10);
    ASSERT_TRUE(ip11);
    ASSERT_FALSE(ip12);
    ASSERT_TRUE(ip13);
    ASSERT_FALSE(ip14);
    ASSERT_TRUE(ip15);
    ASSERT_FALSE(ip16);
    ASSERT_TRUE(ip17);
    ASSERT_FALSE(ip18);
    ASSERT_TRUE(ip19);
    ASSERT_FALSE(ip20);
    ASSERT_TRUE(ip21);
    ASSERT_FALSE(ip22);
    ASSERT_TRUE(ip23);
    ASSERT_FALSE(ip24);
    ASSERT_TRUE(ip25);
    ASSERT_FALSE(ip26);
    ASSERT_TRUE(ip27);
    ASSERT_FALSE(ip28);
    ASSERT_FALSE(ip29);
    ASSERT_FALSE(ip30);
    ASSERT_TRUE(ip31);
    ASSERT_FALSE(ip32);
}

TEST(ip_address, literals) {
    IPADDRESS_CONSTEXPR auto ip1 = "127.128.128.255"_ip;
    IPADDRESS_CONSTEXPR auto ip2 = "2001:db8::1"_ip;
    
    ASSERT_TRUE(ip1.is_v4());
    ASSERT_TRUE(ip2.is_v6());
    ASSERT_EQ(ip1, ip_address::parse("127.128.128.255"));
    ASSERT_EQ(ip2, ip_address::parse("2001:db8::1"));
}

TEST(ip_address, ScopeId) {
    auto ip1 = ip_address::parse("127.128.128.255");
    auto ip2 = ip_address::parse("2001:db8::1");
    
    ASSERT_FALSE(bool(ip1.get_scope_id()));
    ASSERT_FALSE(bool(ip2.get_scope_id()));

    ip1.set_scope_id("123");
    ip2.set_scope_id("123");
    ASSERT_FALSE(bool(ip1.get_scope_id()));
    ASSERT_TRUE(bool(ip2.get_scope_id()));
    ASSERT_FALSE(ip1.get_scope_id().has_string());
    ASSERT_TRUE(ip2.get_scope_id().has_string());
    ASSERT_FALSE(ip1.get_scope_id().has_uint32());
    ASSERT_TRUE(ip2.get_scope_id().has_uint32());
    ASSERT_EQ(ip1.get_scope_id().get_uint32(), 0);
    ASSERT_EQ(ip2.get_scope_id().get_uint32(), 123);

    ip1.set_scope_id("eth1");
    ip2.set_scope_id("eth1");
    ASSERT_FALSE(bool(ip1.get_scope_id()));
    ASSERT_TRUE(bool(ip2.get_scope_id()));
    ASSERT_FALSE(ip1.get_scope_id().has_string());
    ASSERT_TRUE(ip2.get_scope_id().has_string());
    ASSERT_FALSE(ip1.get_scope_id().has_uint32());
    ASSERT_FALSE(ip2.get_scope_id().has_uint32());
    ASSERT_EQ(ip1.get_scope_id().get_string(), "");
    ASSERT_EQ(ip2.get_scope_id().get_string(), "eth1");

    ASSERT_EQ(ip1.to_string(), "127.128.128.255");
    ASSERT_EQ(ip2.to_string(), "2001:db8::1%eth1");
}
