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
    constexpr ip_address ip;

    constexpr auto actual = ip.v4();

    EXPECT_EQ(ip.version(), ip_version::V4);
    EXPECT_EQ(ip.size(), 4);
    EXPECT_TRUE(ip.is_v4());
    EXPECT_FALSE(ip.is_v6());
    EXPECT_TRUE(actual.has_value());

    constexpr auto value = actual.value();
    EXPECT_EQ(value.to_uint(), 0);
}

TEST(ip_address, CopyCtor) {
    constexpr auto ip = ip_address::parse("127.0.0.1");
    constexpr auto ip_copy = ip;

    constexpr auto actual_ip = ip.v4();
    constexpr auto actual_ip_copy = ip_copy.v4();

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

    constexpr auto value_ip = actual_ip.value();
    constexpr auto value_ip_copy = actual_ip_copy.value();
    EXPECT_EQ(value_ip.to_uint(), 0x7F000001);
    EXPECT_EQ(value_ip_copy.to_uint(), 0x7F000001);
}

TEST(ip_address, CopyOperator) {
    constexpr auto ip = 
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
    constexpr auto ipv4 = ipv4_address::parse("192.168.0.1");
    constexpr auto ipv6 = ipv6_address::parse("2002:ac1d:2d64::1");
    constexpr ipv4_address::base_type ipv4_bytes {0xC0, 0xA8, 0x00, 0x01};
    constexpr ipv6_address::base_type ipv6_bytes { 
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 255, 255,
        0, 0, 0, 0
    };

    constexpr ip_address actual1(ipv4);
    constexpr ip_address actual2(ipv6);
    constexpr ip_address actual3(ipv4_bytes);
    constexpr ip_address actual4(ipv6_bytes);
    
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

    constexpr auto actual1 = ip_address::from_bytes(ipv4_bytes);
    constexpr auto actual2 = ip_address::from_bytes(ipv6_bytes);
    constexpr auto actual3 = ip_address::from_bytes(ipv4_bytes.data(), 3, ip_version::V4);
    constexpr auto actual4 = ip_address::from_bytes(ipv6_bytes.data(), 11, ip_version::V6);
    
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
    constexpr ipv6_address::uint_type ip2 = 281470681743360ULL;

    constexpr auto actual1 = ip_address::from_uint(ip1);
    constexpr auto actual2 = ip_address::from_uint(ip2);
    
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
    constexpr auto expected1 = 0xC0A80001;
    constexpr auto expected2 = uint128_t(2306595200482672640, 1);
    constexpr auto expected3 = 1;

    constexpr auto ip1 = ip_address::parse("192.168.0.1");
    constexpr auto ip2 = ip_address::parse("2002:ac1d:2d64::1");

    constexpr auto actual1 = ip1.to_uint();
    constexpr auto actual2 = (uint128_t) ip1;
    constexpr auto actual3 = (uint32_t) ip1;
    constexpr auto actual4 = ip2.to_uint();
    constexpr auto actual5 = (uint128_t) ip2;
    constexpr auto actual6 = (uint32_t) ip2;

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

    static constexpr auto ip1 = ip_address::parse("192.168.0.1");
    static constexpr auto ip2 = ip_address::parse("2002:ac1d:2d64::1");

    constexpr auto data1 = ip1.data();
    constexpr auto data2 = ip2.data();

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

    constexpr auto actual1 = ip_address::parse("192.168.1.1");
    constexpr auto actual2 = ip_address::parse("fe80::1ff:fe23:4567:890a");
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

    constexpr auto actual1_ip = actual1.v4();
    EXPECT_EQ(actual1.version(), ip_version::V4);
    EXPECT_EQ(actual1.size(), 4);
    EXPECT_TRUE(actual1.is_v4());
    EXPECT_FALSE(actual1.is_v6());
    EXPECT_TRUE(actual1_ip.has_value());
    EXPECT_EQ(actual1_ip.value().to_uint(), 0xC0A80101);

    constexpr auto actual2_ip = actual2.v6();
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

    error_code err;
    ip_address::parse(expected_address, err);
    ASSERT_EQ(err, expected_error_code);

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_ip = ip_address::parse(expected_address);

    EXPECT_EQ(error_ip.v4().value().to_uint(), 0);
#else
    EXPECT_THAT(
        [address=expected_address]() { ip_address::parse(address); },
        ThrowsMessage<parse_error>(StrEq(get<2>(GetParam()))));
    EXPECT_THAT(
        [address=expected_address]() { ip_address::parse(address); },
        Throws<parse_error>(Property(&parse_error::code, Eq(expected_error_code))));
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ip_address, InvalidAddressParams,
    testing::Values(
        std::make_tuple("", error_code::EMPTY_ADDRESS, "address cannot be empty"),
        std::make_tuple("127", error_code::LEAST_3_PARTS, "least 3 parts in address 127"),
        std::make_tuple("127.0.0.", error_code::EMPTY_OCTET, "empty octet 0 in address 127.0.0."),
        std::make_tuple("127.0.0.1/24", error_code::OCTET_HAS_INVALID_SYMBOL, "in octet 0 of address 127.0.0.1/24 has invalid symbol"),
        std::make_tuple("127.0.0.1271", error_code::OCTET_MORE_3_CHARACTERS, "in octet 0 of address 127.0.0.1271 more 3 characters"),
        std::make_tuple("192.168.0.999", error_code::OCTET_EXCEEDED_255, "octet 0 of address 192.168.0.999 exceeded 255"),
        std::make_tuple("1.2.3.040", error_code::LEADING_0_ARE_NOT_PERMITTED, "leading zeros are not permitted in octet 0 of address 1.2.3.040"),
        std::make_tuple("FEDC:9878%scope", error_code::LEAST_3_PARTS, "least 3 parts in address FEDC:9878%scope"),
        std::make_tuple("10:9:8:7:6:5:4:3:42.42.42.42%scope", error_code::MOST_8_COLONS_PERMITTED, "most 8 colons permitted in address 10:9:8:7:6:5:4:3:42.42.42.42%scope"),
        std::make_tuple("2001:db8:::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001:db8:::1%scope"),
        std::make_tuple("123456789abcfe::", error_code::PART_IS_MORE_4_CHARS, "in part 0 of address 123456789abcfe:: more 4 characters"),
        std::make_tuple("::1/24", error_code::PART_HAS_INVALID_SYMBOL, "in part 3 of address ::1/24 has invalid symbols"),
        std::make_tuple("2001:db8:::1%scope", error_code::MOST_ONE_DOUBLE_COLON_PERMITTED, "at most one '::' permitted in address 2001:db8:::1%scope"),
        std::make_tuple(":6:5:4:3:2:1::%scope", error_code::LEADING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at leading ':' only permitted as part of '::' in address :6:5:4:3:2:1::%scope"),
        std::make_tuple("8:7:6:5:4:3:2:1::%scope", error_code::TRAILING_COLON_ONLY_PERMITTED_AS_PART_OF_DOUBLE_COLON, "at trailing ':' only permitted as part of '::' in address 8:7:6:5:4:3:2:1::%scope"),
        std::make_tuple("1:2:3:4:5::6:7:8%scope", error_code::EXPECTED_AT_MOST_7_OTHER_PARTS_WITH_DOUBLE_COLON, "expected at most 7 other parts with '::' in address 1:2:3:4:5::6:7:8%scope"),
        std::make_tuple("7:6:5:4:3:2:1%scope", error_code::EXACTLY_8_PARTS_EXPECTED_WITHOUT_DOUBLE_COLON, "exactly 8 parts expected without '::' in address 7:6:5:4:3:2:1%scope"),
        std::make_tuple("::%123456789abcdefgh", error_code::SCOPE_ID_IS_TOO_LONG, "scope id is too long in address ::%123456789abcdefgh"),
        std::make_tuple("::1%scope_id/24", error_code::INVALID_SCOPE_ID, "invalid scope id in address ::1%scope_id/24") 
    ));

TEST(ip_address, Comparison) {
    constexpr auto ip1 = ip_address::parse("127.240.0.1");
    constexpr auto ip2 = ip_address::parse("2001:db8::1");
    constexpr auto ip3 = ip_address::parse("2001:db8::2");

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
    constexpr auto ip1 = ip_address::parse("127.240.0.1");
    constexpr auto ip2 = ip_address::parse("fe80::1ff:fe23:4567:890a%eth2");

    const std::string expected_address = "127.240.0.1";
    const auto expected_full_2 = "fe80:0000:0000:0000:01ff:fe23:4567:890a%eth2";
    const auto expected_compact_2 = "fe80:0:0:0:1ff:fe23:4567:890a%eth2";
    const auto expected_compressed_2 = "fe80::1ff:fe23:4567:890a%eth2";
    const auto expected_compressed_upper_2 = "FE80::1FF:FE23:4567:890A%eth2";

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

TEST(ip_address, Hash) {
    constexpr auto hash_functor = std::hash<ip_address>{};

    constexpr auto ip1 = ip_address::parse("127.0.0.1");
    constexpr auto ip2 = ip_address::parse("2001:db8::1");

    constexpr auto hash1 = ip1.hash();
    constexpr auto hash2 = ip2.hash();
    constexpr auto hash3 = hash_functor(ip1);
    constexpr auto hash4 = hash_functor(ip2);

    ASSERT_EQ(hash1, sizeof(size_t) == 8 ? 2753421670903790749ULL : 4009513643U);
    ASSERT_EQ(hash2, sizeof(size_t) == 8 ? 6222153021643990098ULL : 1539407693U);
    ASSERT_EQ(hash3, sizeof(size_t) == 8 ? 2753421670903790749ULL : 4009513643U);
    ASSERT_EQ(hash4, sizeof(size_t) == 8 ? 6222153021643990098ULL : 1539407693U);
}

TEST(ip_address, Containers) {
    constexpr auto ip1 = ip_address::parse("127.0.0.1");
    constexpr auto ip2 = ip_address::parse("2001:db8::1");
    constexpr auto ip3 = ip_address::parse("2001:db8::2");

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
    constexpr auto ip1 = ip_address::parse("127.0.0.1");
    constexpr auto ip2 = ip_address::parse("2001:db8::1");
    
    const auto actual1 = ip1.reverse_pointer();
    const auto actual2 = ip2.reverse_pointer();

    ASSERT_EQ(actual1, "1.0.0.127.in-addr.arpa");
    ASSERT_EQ(actual2, "1.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.8.b.d.0.1.0.0.2.ip6.arpa");
}

TEST(ip_address, ipv4_mapped) {
    constexpr auto ip1 = ip_address::parse("127.0.0.1");
    constexpr auto ip2 = ip_address::parse("::ffff:192.168.1.1%test");
    constexpr auto ip3 = ip_address::parse("::c0a8:101");

    constexpr auto actual1 = ip1.ipv4_mapped();
    constexpr auto actual2 = ip2.ipv4_mapped();
    constexpr auto actual3 = ip3.ipv4_mapped();

    constexpr auto actual_has_value1 = actual1.has_value();
    constexpr auto actual_has_value2 = actual2.has_value();
    constexpr auto actual_has_value3 = actual3.has_value();
    ASSERT_FALSE(actual_has_value1);
    ASSERT_TRUE(actual_has_value2);
    ASSERT_FALSE(actual_has_value3);

    constexpr ip_address ipv4 = actual2.value();
    ASSERT_EQ(ipv4, ip_address::parse("192.168.1.1"));
}

TEST(ip_address, sixtofour) {
    constexpr auto ip1 = ip_address::parse("127.0.0.1");
    constexpr auto ip2 = ip_address::parse("2002:ac1d:2d64::1");
    constexpr auto ip3 = ip_address::parse("2000:ac1d:2d64::1");

    constexpr auto actual1 = ip1.sixtofour();
    constexpr auto actual2 = ip2.sixtofour();
    constexpr auto actual3 = ip3.sixtofour();

    constexpr auto actual_has_value1 = actual1.has_value();
    constexpr auto actual_has_value2 = actual2.has_value();
    constexpr auto actual_has_value3 = actual3.has_value();
    ASSERT_FALSE(actual_has_value1);
    ASSERT_TRUE(actual_has_value2);
    ASSERT_FALSE(actual_has_value3);

    constexpr ip_address ipv4 = actual2.value();
    ASSERT_EQ(ipv4, ip_address::parse("172.29.45.100"));
}

TEST(ip_address, teredo) {
    constexpr auto ip1 = ip_address::parse("127.0.0.1");
    constexpr auto ip2 = ip_address::parse("2001:0000:4136:e378:8000:63bf:3fff:fdd2");
    constexpr auto ip3 = ip_address::parse("2000::4136:e378:8000:63bf:3fff:fdd2");

    constexpr auto actual1 = ip1.teredo();
    constexpr auto actual2 = ip2.teredo();
    constexpr auto actual3 = ip3.teredo();

    constexpr auto actual_has_value1 = actual1.has_value();
    constexpr auto actual_has_value2 = actual2.has_value();
    constexpr auto actual_has_value3 = actual3.has_value();
    ASSERT_FALSE(actual_has_value1);
    ASSERT_TRUE(actual_has_value2);
    ASSERT_FALSE(actual_has_value3);

    constexpr ip_address server = actual2.value().first;
    constexpr ip_address client = actual2.value().second;
    ASSERT_EQ(server, ip_address::parse("65.54.227.120"));
    ASSERT_EQ(client, ip_address::parse("192.0.2.45"));
}

TEST(ip_address, is_prop) {
    constexpr auto ip1 = ip_address::parse("224.1.1.1").is_multicast();
    constexpr auto ip2 = ip_address::parse("240.0.0.0").is_multicast();
    constexpr auto ip3 = ip_address::parse("ffff::").is_multicast();
    constexpr auto ip4 = ip_address::parse("fdff::").is_multicast();
    constexpr auto ip5 = ip_address::parse("192.168.1.1").is_private();
    constexpr auto ip6 = ip_address::parse("192.169.0.0").is_private();
    constexpr auto ip7 = ip_address::parse("fc00::").is_private();
    constexpr auto ip8 = ip_address::parse("fbff:ffff::").is_private();
    constexpr auto ip9 = ip_address::parse("192.0.7.1").is_global();
    constexpr auto ip10 = ip_address::parse("203.0.113.1").is_global();
    constexpr auto ip11 = ip_address::parse("200::1").is_global();
    constexpr auto ip12 = ip_address::parse("fc00::").is_global();
    constexpr auto ip13 = ip_address::parse("240.0.0.1").is_reserved();
    constexpr auto ip14 = ip_address::parse("239.255.255.255").is_reserved();
    constexpr auto ip15 = ip_address::parse("100::").is_reserved();
    constexpr auto ip16 = ip_address::parse("ffff::").is_reserved();
    constexpr auto ip17 = ip_address::parse("127.42.0.0").is_loopback();
    constexpr auto ip18 = ip_address::parse("128.0.0.0").is_loopback();
    constexpr auto ip19 = ip_address::parse("::1").is_loopback();
    constexpr auto ip20 = ip_address::parse("::2").is_loopback();
    constexpr auto ip21 = ip_address::parse("169.254.100.200").is_link_local();
    constexpr auto ip22 = ip_address::parse("169.255.100.200").is_link_local();
    constexpr auto ip23 = ip_address::parse("fea0::").is_link_local();
    constexpr auto ip24 = ip_address::parse("fe7f:ffff::").is_link_local();
    constexpr auto ip25 = ip_address::parse("0.0.0.0").is_unspecified();
    constexpr auto ip26 = ip_address::parse("127.0.0.1").is_unspecified();
    constexpr auto ip27 = ip_address::parse("::").is_unspecified();
    constexpr auto ip28 = ip_address::parse("::1").is_unspecified();
    constexpr auto ip29 = ip_address::parse("127.0.0.1").is_site_local();
    constexpr auto ip30 = ip_address::parse("0.0.0.0").is_site_local();
    constexpr auto ip31 = ip_address::parse("fecf::").is_site_local();
    constexpr auto ip32 = ip_address::parse("fbf:ffff::").is_site_local();
    
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
    constexpr auto ip1 = "127.128.128.255"_ip;
    constexpr auto ip2 = "2001:db8::1"_ip;
    
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
