#include <map>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

#if IPADDRESS_CPP_VERSION >= 14

template <size_t N>
static constexpr error_code get_parse_error(const char(&address)[N]) noexcept {
    error_code err = error_code::no_error;
    ipv4_address::parse(address, err);
    return err;
}

TEST(ipv4_address, CompileTime) {
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    constexpr auto ip1 = ipv4_address::parse<"127.0.0.1">();
    ASSERT_EQ(ip1.to_uint(), 0x7F000001);
    
    constexpr auto ip2 = ipv4_address::parse<"127.0.0.1">();
    constexpr auto ip2_uint32 = ip2.to_uint();
    constexpr auto ip2_bytes = ip2.bytes();
    constexpr auto ip2_byte_0 = ip2_bytes[0];
    constexpr auto ip2_byte_1 = ip2_bytes.at(1);
    constexpr auto ip2_byte_2 = *(ip2_bytes.begin() + 2);
    constexpr auto ip2_byte_3 = ip2_bytes.back();
    ASSERT_EQ(ip2_uint32, 0x7F000001);
    ASSERT_EQ(ip2_byte_0, 0x7F);
    ASSERT_EQ(ip2_byte_1, 0x00);
    ASSERT_EQ(ip2_byte_2, 0x00);
    ASSERT_EQ(ip2_byte_3, 0x01);

    constexpr auto ip4 = ipv4_address::from_uint<0x7F000002>();
    constexpr auto ip4_uint32 = ip4.to_uint();
    ASSERT_EQ(ip4_uint32, 0x7F000002);

    constexpr auto b1 = ip2 < ip4;
    constexpr auto b2 = ip2 > ip4;
    constexpr auto b3 = ip2 <= ip4;
    constexpr auto b4 = ip2 >= ip4;
    constexpr auto b5 = ip2 == ip4;
    constexpr auto b6 = ip2 != ip4;
    ASSERT_TRUE(b1);
    ASSERT_FALSE(b2);
    ASSERT_TRUE(b3);
    ASSERT_FALSE(b4);
    ASSERT_FALSE(b5);
    ASSERT_TRUE(b6);

    constexpr auto ip_wchar = ipv4_address::parse<L"127.0.0.1">();
    ASSERT_EQ(ip_wchar.to_uint(), 0x7F000001);

    constexpr auto ip_char16 = ipv4_address::parse<u"127.0.0.1">();
    ASSERT_EQ(ip_char16.to_uint(), 0x7F000001);

    constexpr auto ip_char32 = ipv4_address::parse<U"127.0.0.1">();
    ASSERT_EQ(ip_char32.to_uint(), 0x7F000001);

#if __cpp_char8_t >= 201811L
    constexpr auto ip_char8 = ipv4_address::parse<u8"127.0.0.1">();
    ASSERT_EQ(ip_char8.to_uint(), 0x7F000001);
#endif
#endif
    auto ip3 = ipv4_address::from_uint<0x7F000001>();
    ASSERT_EQ(ip3.to_uint(), 0x7F000001);

    constexpr auto ip5 = ipv4_address::parse("127.0.0.1");
    constexpr auto ip5_uint32 = ip5.to_uint();
    constexpr auto ip5_bytes = ip5.bytes();
    constexpr auto ip5_byte_0 = ip5_bytes[0];
    constexpr auto ip5_byte_1 = ip5_bytes.at(1);
    constexpr auto ip5_byte_2 = *(ip5_bytes.begin() + 2);
    constexpr auto ip5_byte_3 = ip5_bytes.back();
    ASSERT_EQ(ip5_uint32, 0x7F000001);
    ASSERT_EQ(ip5_byte_0, 0x7F);
    ASSERT_EQ(ip5_byte_1, 0x00);
    ASSERT_EQ(ip5_byte_2, 0x00);
    ASSERT_EQ(ip5_byte_3, 0x01);

    constexpr auto err = get_parse_error("127.0.0.256");
    ASSERT_EQ(err, error_code::octet_exceeded_255);

    constexpr auto ip6 = ipv4_address::from_uint(0x7F000002);
    constexpr auto ip6_uint32 = ip6.to_uint();
    ASSERT_EQ(ip6_uint32, 0x7F000002);

    constexpr auto ip7 = ipv4_address::from_bytes({0xC0, 0xA8, 0x00, 0x01});
    constexpr auto ip7_uint32 = ip7.to_uint();
    ASSERT_EQ(ip7_uint32, 0xC0A80001);

    constexpr uint8_t bytes[] = {0xC0, 0xA8, 0x00, 0x01};
    constexpr auto ip8 = ipv4_address::from_bytes(bytes, 3);
    constexpr auto ip8_uint32 = ip8.to_uint();
    ASSERT_EQ(ip8_uint32, 0xC0A80000);

    constexpr auto b7 = ip5 < ip6;
    constexpr auto b8 = ip5 > ip6;
    constexpr auto b9 = ip5 <= ip6;
    constexpr auto b10 = ip5 >= ip6;
    constexpr auto b11 = ip5 == ip6;
    constexpr auto b12 = ip5 != ip6;
    
    ASSERT_TRUE(b7);
    ASSERT_FALSE(b8);
    ASSERT_TRUE(b9);
    ASSERT_FALSE(b10);
    ASSERT_FALSE(b11);
    ASSERT_TRUE(b12);

    constexpr auto ip9 = u"127.0.0.1"_ipv4;
    constexpr auto ip10 = U"127.128.128.255"_ipv4;
    constexpr auto ip9_1 = 0x7F000001_ipv4;
    ASSERT_EQ(ip9, ipv4_address::parse("127.0.0.1"));
    ASSERT_EQ(ip10, ipv4_address::parse("127.128.128.255"));
    ASSERT_EQ(ip9_1, ipv4_address::parse("127.0.0.1"));

    constexpr auto ip11 = ipv4_address::parse("224.1.1.1").is_multicast();
    constexpr auto ip12 = ipv4_address::parse("240.0.0.0").is_multicast();
    ASSERT_TRUE(ip11);
    ASSERT_FALSE(ip12);

    constexpr auto ip13 = ipv4_address::parse("192.168.1.1").is_private();
    constexpr auto ip14 = ipv4_address::parse("192.169.0.0").is_private();
    ASSERT_TRUE(ip13);
    ASSERT_FALSE(ip14);

    constexpr auto ip15 = ipv4_address::parse("192.0.7.1").is_global();
    constexpr auto ip16 = ipv4_address::parse("203.0.113.1").is_global();
    ASSERT_TRUE(ip15);
    ASSERT_FALSE(ip16);

    constexpr auto ip17 = ipv4_address::parse("240.0.0.1").is_reserved();
    constexpr auto ip18 = ipv4_address::parse("239.255.255.255").is_reserved();
    ASSERT_TRUE(ip17);
    ASSERT_FALSE(ip18);

    constexpr auto ip19 = ipv4_address::parse("127.100.200.254").is_loopback();
    constexpr auto ip20 = ipv4_address::parse("128.0.0.0").is_loopback();
    ASSERT_TRUE(ip19);
    ASSERT_FALSE(ip20);

    constexpr auto ip21 = ipv4_address::parse("169.254.100.200").is_link_local();
    constexpr auto ip22 = ipv4_address::parse("169.255.100.200").is_link_local();
    ASSERT_TRUE(ip21);
    ASSERT_FALSE(ip22);

    constexpr auto ip23 = ipv4_address::parse("0.0.0.0").is_unspecified();
    constexpr auto ip24 = ipv4_address::parse("169.255.100.200").is_unspecified();
    ASSERT_TRUE(ip23);
    ASSERT_FALSE(ip24);
    
    constexpr auto ip_wchar_2 = ipv4_address::parse(L"127.0.0.1");
    ASSERT_EQ(ip_wchar_2.to_uint(), 0x7F000001);

    constexpr auto ip_char16_2 = ipv4_address::parse(u"127.0.0.1");
    ASSERT_EQ(ip_char16_2.to_uint(), 0x7F000001);

    constexpr auto ip_char32_2 = ipv4_address::parse(U"127.0.0.1");
    ASSERT_EQ(ip_char32_2.to_uint(), 0x7F000001);

#if __cpp_char8_t >= 201811L
   constexpr auto ip_char8_2 = ipv4_address::parse(u8"127.0.0.1");
   ASSERT_EQ(ip_char8_2.to_uint(), 0x7F000001);
#endif
}

#endif

TEST(ipv4_address, DefaultCtor) {
    ipv4_address::base_type expected_empty { 0, 0, 0, 0};

    ipv4_address ip;
    
    EXPECT_EQ(ip.bytes(), expected_empty);
    EXPECT_EQ(ip.to_uint(), 0);
    EXPECT_EQ(uint32_t(ip), 0);
    EXPECT_EQ(ip.size(), 4);
    EXPECT_EQ(ip.version(), ip_version::V4);
}

TEST(ipv4_address, CopyCtor) {
    ipv4_address::base_type expected_ip { 127, 0, 0, 1};

    auto ip = ipv4_address::parse("127.0.0.1");
    auto ip_copy = ip;

    EXPECT_EQ(ip.bytes(), expected_ip);
    EXPECT_EQ(ip.to_uint(), 0x7F000001);
    EXPECT_EQ(uint32_t(ip), 0x7F000001);

    EXPECT_EQ(ip_copy.bytes(), expected_ip);
    EXPECT_EQ(ip_copy.to_uint(), 0x7F000001);
    EXPECT_EQ(uint32_t(ip_copy), 0x7F000001);
}

TEST(ipv4_address, CopyOperator) {
    ipv4_address::base_type expected_ip { 127, 0, 0, 1};

    auto ip = ipv4_address::parse("127.0.0.1");
    ipv4_address ip_copy;

    EXPECT_EQ(ip_copy.to_uint(), 0);
    ip_copy = ip;

    EXPECT_EQ(ip.bytes(), expected_ip);
    EXPECT_EQ(ip.to_uint(), 0x7F000001);
    EXPECT_EQ(uint32_t(ip), 0x7F000001);

    EXPECT_EQ(ip_copy.bytes(), expected_ip);
    EXPECT_EQ(ip_copy.to_uint(), 0x7F000001);
    EXPECT_EQ(uint32_t(ip_copy), 0x7F000001);
}

using FromUint32Params = TestWithParam<std::tuple<uint32_t, const char*>>;
TEST_P(FromUint32Params, from_uint) {
    ASSERT_EQ(ipv4_address::from_uint(get<0>(GetParam())), ipv4_address::parse(get<1>(GetParam())));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, FromUint32Params,
    testing::Values(
            std::make_tuple(0, "0.0.0.0"),
            std::make_tuple(0xC0A80001, "192.168.0.1")
    ));

using FromBytesIpv4Params = TestWithParam<ipv4_address::base_type>;
TEST_P(FromBytesIpv4Params, from_bytes) {
    ASSERT_EQ(ipv4_address::from_bytes(GetParam()).bytes(), GetParam());
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, FromBytesIpv4Params,
    testing::Values(
        ipv4_address::base_type{0x00, 0x08, 0x00, 0x00},
        ipv4_address::base_type{0xC0, 0xA8, 0x00, 0x01}
    ));

TEST(ipv4_address, parse_utf) {
    auto str1 = L"127.0.0.1";
    auto ip_wchar = ipv4_address::parse(str1);
    ASSERT_EQ(ip_wchar.to_uint(), 0x7F000001);

    auto str2 = u"127.0.0.1";
    auto ip_char16 = ipv4_address::parse(str2);
    ASSERT_EQ(ip_char16.to_uint(), 0x7F000001);

    auto str3 = U"127.0.0.1";
    auto ip_char32 = ipv4_address::parse(str3);
    ASSERT_EQ(ip_char32.to_uint(), 0x7F000001);

#if __cpp_char8_t >= 201811L
    auto str4 = u8"127.0.0.1";
    auto ip_char8 = ipv4_address::parse(str4);
    ASSERT_EQ(ip_char8.to_uint(), 0x7F000001);
#endif
}

using AddressParserIpv4Params = TestWithParam<std::tuple<const char*, uint32_t, std::array<uint8_t, 4>>>;
TEST_P(AddressParserIpv4Params, parse) {
    const auto& excepted_uint32 = get<1>(GetParam());
    const auto& excepted_bytes = get<2>(GetParam());

    auto ip = ipv4_address::parse(get<0>(GetParam()));
    auto actual_uint32 = ip.to_uint();
    const auto& actual_bytes = ip.bytes();

    ASSERT_EQ(actual_uint32, excepted_uint32);
    ASSERT_EQ(actual_bytes[0], excepted_bytes[0]);
    ASSERT_EQ(actual_bytes[1], excepted_bytes[1]);
    ASSERT_EQ(actual_bytes[2], excepted_bytes[2]);
    ASSERT_EQ(actual_bytes[3], excepted_bytes[3]);

    std::string s1;
    ipv4_address addr;
    std::string s2;
    std::istringstream ss(std::string("test: ") + get<0>(GetParam()) + " parser");
    ss >> s1 >> addr >> s2;

    ASSERT_EQ(s1, std::string("test:"));
    ASSERT_EQ(addr.to_uint(), excepted_uint32);
    ASSERT_EQ(s2, std::string("parser"));

    ipv4_address bad_addr;
    std::istringstream ss2("test"); ss2 >> bad_addr;
    ASSERT_TRUE(ss2.fail());
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, AddressParserIpv4Params,
    testing::Values(
        std::make_tuple("0.0.0.0",     0x00000000, std::array<uint8_t, 4>{ 0x00, 0x00, 0x00, 0x00 }),
        std::make_tuple("100.64.0.0",  0x64400000, std::array<uint8_t, 4>{ 0x64, 0x40, 0x00, 0x00 }),
        std::make_tuple("127.0.0.1",   0x7F000001, std::array<uint8_t, 4>{ 0x7F, 0x00, 0x00, 0x01 }),
        std::make_tuple("192.168.1.1", 0xC0A80101, std::array<uint8_t, 4>{ 0xC0, 0xA8, 0x01, 0x01 })
    ));

using InvalidAddressIpv4Params = TestWithParam<std::tuple<const char*, error_code, const char*>>;
TEST_P(InvalidAddressIpv4Params, parse) {
    auto expected_address = get<0>(GetParam());
    auto expected_error_code = get<1>(GetParam());

    error_code err = error_code::no_error;
    ipv4_address::parse(expected_address, err);
    ASSERT_EQ(err, expected_error_code);

#ifdef IPADDRESS_NO_EXCEPTIONS
    ipv4_address::base_type expected_empty { 0, 0, 0, 0};
    auto error_ip = ipv4_address::parse(expected_address);

    EXPECT_EQ(error_ip.bytes(), expected_empty);
    ASSERT_EQ(error_ip.to_uint(), 0);
    EXPECT_EQ(uint32_t(error_ip), 0);
#elif IPADDRESS_CPP_VERSION >= 14
    EXPECT_THAT(
        [address=expected_address]() { ipv4_address::parse(address); },
        ThrowsMessage<parse_error>(StrEq(get<2>(GetParam()))));
    EXPECT_THAT(
        [address=expected_address]() { ipv4_address::parse(address); },
        Throws<parse_error>(Property(&parse_error::code, Eq(expected_error_code))));
#else
    ASSERT_THROW(ipv4_address::parse(expected_address), parse_error);
#endif
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, InvalidAddressIpv4Params,
    testing::Values(
        std::make_tuple("", error_code::empty_address, "address cannot be empty"),

        std::make_tuple("127",             error_code::expected_4_octets, "expected 4 octets in 127"),
        std::make_tuple("127.0",           error_code::expected_4_octets, "expected 4 octets in 127.0"),
        std::make_tuple("127.0.0",         error_code::expected_4_octets, "expected 4 octets in 127.0.0"),
        std::make_tuple("42.42.42.42.42",  error_code::expected_4_octets, "expected 4 octets in 42.42.42.42.42"),
        std::make_tuple("192.168.0.1.com", error_code::expected_4_octets, "expected 4 octets in 192.168.0.1.com"),
        std::make_tuple("42.42.42.42...",  error_code::expected_4_octets, "expected 4 octets in 42.42.42.42..."),

        std::make_tuple("...42.42.42.42", error_code::empty_octet, "empty octet 0 in address ...42.42.42.42"),
        std::make_tuple("42..42.42.42",   error_code::empty_octet, "empty octet 1 in address 42..42.42.42"),
        std::make_tuple("42.42..42.42",   error_code::empty_octet, "empty octet 2 in address 42.42..42.42"),
        std::make_tuple("42.42.42..42",   error_code::empty_octet, "empty octet 3 in address 42.42.42..42"),
        std::make_tuple("42.42..42",      error_code::empty_octet, "empty octet 2 in address 42.42..42"),
        std::make_tuple(".42.42.42.42",   error_code::empty_octet, "empty octet 0 in address .42.42.42.42"),
        std::make_tuple(".",              error_code::empty_octet, "empty octet 0 in address ."),
        std::make_tuple("42..42.42",      error_code::empty_octet, "empty octet 1 in address 42..42.42"),
        std::make_tuple("...",            error_code::empty_octet, "empty octet 0 in address ..."),
        std::make_tuple("127.0.0.",       error_code::empty_octet, "empty octet 3 in address 127.0.0."),

        std::make_tuple("0x0a.0x0a.0x0a.0x0a", error_code::octet_has_invalid_symbol, "in octet 0 of address 0x0a.0x0a.0x0a.0x0a has invalid symbol"),
        std::make_tuple("0xa.0x0a.0x0a.0x0a",  error_code::octet_has_invalid_symbol, "in octet 0 of address 0xa.0x0a.0x0a.0x0a has invalid symbol"),
        std::make_tuple("42.42.42.-0",         error_code::octet_has_invalid_symbol, "in octet 3 of address 42.42.42.-0 has invalid symbol"),
        std::make_tuple("42.42.42.+0",         error_code::octet_has_invalid_symbol, "in octet 3 of address 42.42.42.+0 has invalid symbol"),
        std::make_tuple("42.42.42.-42",        error_code::octet_has_invalid_symbol, "in octet 3 of address 42.42.42.-42 has invalid symbol"),
        std::make_tuple("+1.+2.+3.4",          error_code::octet_has_invalid_symbol, "in octet 0 of address +1.+2.+3.4 has invalid symbol"),
        std::make_tuple("1.2.3.4e0",           error_code::octet_has_invalid_symbol, "in octet 3 of address 1.2.3.4e0 has invalid symbol"),
        std::make_tuple("1.2.3.4::",           error_code::octet_has_invalid_symbol, "in octet 3 of address 1.2.3.4:: has invalid symbol"),
        std::make_tuple("1.a.2.3",             error_code::octet_has_invalid_symbol, "in octet 1 of address 1.a.2.3 has invalid symbol"),
        std::make_tuple("127.0.0.1/24",        error_code::octet_has_invalid_symbol, "in octet 3 of address 127.0.0.1/24 has invalid symbol"),
        
        std::make_tuple("1271.0.0.1",   error_code::octet_more_3_characters, "in octet 0 of address 1271.0.0.1 more 3 characters"),
        std::make_tuple("127.1271.0.1", error_code::octet_more_3_characters, "in octet 1 of address 127.1271.0.1 more 3 characters"),
        std::make_tuple("127.0.1271.1", error_code::octet_more_3_characters, "in octet 2 of address 127.0.1271.1 more 3 characters"),
        std::make_tuple("127.0.0.1271", error_code::octet_more_3_characters, "in octet 3 of address 127.0.0.1271 more 3 characters"),

        std::make_tuple("257.0.0.0",     error_code::octet_exceeded_255, "octet 0 of address 257.0.0.0 exceeded 255"),
        std::make_tuple("127.258.0.1",   error_code::octet_exceeded_255, "octet 1 of address 127.258.0.1 exceeded 255"),
        std::make_tuple("127.0.700.1",   error_code::octet_exceeded_255, "octet 2 of address 127.0.700.1 exceeded 255"),
        std::make_tuple("192.168.0.999", error_code::octet_exceeded_255, "octet 3 of address 192.168.0.999 exceeded 255"),

        std::make_tuple("000.000.000.000", error_code::leading_0_are_not_permitted, "leading zeros are not permitted in octet 0 of address 000.000.000.000"),
        std::make_tuple("192.168.000.001", error_code::leading_0_are_not_permitted, "leading zeros are not permitted in octet 2 of address 192.168.000.001"),
        std::make_tuple("016.016.016.016", error_code::leading_0_are_not_permitted, "leading zeros are not permitted in octet 0 of address 016.016.016.016"),
        std::make_tuple("001.000.008.016", error_code::leading_0_are_not_permitted, "leading zeros are not permitted in octet 0 of address 001.000.008.016"),
        std::make_tuple("01.2.3.40",       error_code::leading_0_are_not_permitted, "leading zeros are not permitted in octet 0 of address 01.2.3.40"),
        std::make_tuple("1.02.3.40",       error_code::leading_0_are_not_permitted, "leading zeros are not permitted in octet 1 of address 1.02.3.40"),
        std::make_tuple("1.2.03.40",       error_code::leading_0_are_not_permitted, "leading zeros are not permitted in octet 2 of address 1.2.03.40"),
        std::make_tuple("1.2.3.040",       error_code::leading_0_are_not_permitted, "leading zeros are not permitted in octet 3 of address 1.2.3.040")
    ));

template <typename T, size_t N1, size_t N2>
static void parse_unexpected_symbol(const T (&expected_address)[N1], const T (&str)[N2]) {
    using tstring = std::basic_string<T, std::char_traits<T>, std::allocator<T>>;
    using tistringstream = std::basic_istringstream<T, std::char_traits<T>, std::allocator<T>>;

    error_code err1 = error_code::no_error;
    error_code err2 = error_code::no_error;

    ipv4_address::parse(expected_address, err1);
    ipv4_address::parse(tstring(expected_address), err2);
    ASSERT_EQ(err1, error_code::unexpected_symbol);
    ASSERT_EQ(err2, error_code::unexpected_symbol);

    tistringstream ss(str);
    ipv4_address ip1, ip2;
    ss >> ip1;
    ASSERT_FALSE(ss.fail());
    ASSERT_EQ(ip1, ipv4_address::parse("127.0.0.1"));
    ss >> ip2;
    ASSERT_TRUE(ss.fail());
    ASSERT_EQ(ip2, ipv4_address::parse("0.0.0.0"));

#ifdef IPADDRESS_NO_EXCEPTIONS
    auto error_ip1 = ipv4_address::parse(expected_address);
    auto error_ip2 = ipv4_address::parse(tstring(expected_address));
    ASSERT_EQ(error_ip1.to_uint(), 0);
    ASSERT_EQ(error_ip2.to_uint(), 0);
#elif IPADDRESS_CPP_VERSION >= 14
    EXPECT_THAT(
        [address=expected_address]() { ipv4_address::parse(address); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+10348} in string 127.{U+10348}.{U+d55c}.1")));
    EXPECT_THAT(
        [address=expected_address]() { ipv4_address::parse(tstring(address)); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+10348} in string 127.{U+10348}.{U+d55c}.1")));
    EXPECT_THAT(
        [address=expected_address]() { ipv4_address::parse(address); },
        Throws<parse_error>(Property(&parse_error::code, Eq(error_code::unexpected_symbol))));
#else // googletest EXPECT_THAT is not supported in cpp less than 14
    ASSERT_THROW(ipv4_address::parse(expected_address), parse_error);
    ASSERT_THROW((ipv4_address::parse(tstring(expected_address))), parse_error);
#endif
}
#define PARSE_UNEXPECTED_SYMBOL(unicode) parse_unexpected_symbol(unicode##"127.\U00010348.\ud55c.1", unicode##"127.0.0.1 127.\U00010348.\ud55c.1")

#if __cpp_char8_t >= 201811L
TEST(ipv4_address, ParseUnexpectedUtf8) {
    PARSE_UNEXPECTED_SYMBOL(u8);
}
#endif

TEST(ipv4_address, ParseUnexpectedUtf16) {
    PARSE_UNEXPECTED_SYMBOL(u);
}

TEST(ipv4_address, ParseUnexpectedUtf32) {
    PARSE_UNEXPECTED_SYMBOL(U);
}

TEST(ipv4_address, ParseUnexpectedWideChar) {
    PARSE_UNEXPECTED_SYMBOL(L);
}

TEST(ipv4_address, Comparison) {
    auto ip1 = ipv4_address::parse("127.239.0.1");
    auto ip2 = ipv4_address::parse("127.240.0.1");
    auto ip3 = ipv4_address::parse("127.240.0.1");
    
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
    
    ASSERT_FALSE(ip2 < ip3);
    ASSERT_TRUE(ip2 <= ip3);
    ASSERT_FALSE(ip2 > ip3);
    ASSERT_TRUE(ip2 >= ip3);
    ASSERT_TRUE(ip2 == ip3);
    ASSERT_FALSE(ip2 != ip3);
    
    ASSERT_FALSE(ip3 < ip2);
    ASSERT_TRUE(ip3 <= ip2);
    ASSERT_FALSE(ip3 > ip2);
    ASSERT_TRUE(ip3 >= ip2);
    ASSERT_TRUE(ip3 == ip2);
    ASSERT_FALSE(ip3 != ip2);
}

using ToStringIpv4Params = TestWithParam<const char*>;
TEST_P(ToStringIpv4Params, to_string) {
    const auto expected = GetParam();

    const auto actual = ipv4_address::parse(expected);

    std::ostringstream ss;
    ss << actual;

    ASSERT_EQ(actual.to_string(), std::string(expected));
    ASSERT_EQ(std::to_string(actual), std::string(expected));
    ASSERT_EQ(ss.str(), std::string(expected));
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, ToStringIpv4Params,
    testing::Values(
        "0.0.0.0",
        "127.0.0.1",
        "255.0.42.42"
    ));

TEST(ipv4_address, Hash) {
    auto ip1 = ipv4_address::parse("127.0.0.1");
    auto ip2 = ipv4_address::parse("127.0.0.2");
    auto ip3 = ipv4_address::parse("127.0.0.3");
    auto hash_functor = std::hash<ipv4_address>{};

    ASSERT_EQ(ip1.hash(), sizeof(size_t) == 8 ? 2753421670903790749ULL : 4009513643U);
    ASSERT_EQ(ip2.hash(), sizeof(size_t) == 8 ? 9476900596348728241ULL : 1628829895U);
    ASSERT_EQ(ip3.hash(), sizeof(size_t) == 8 ? 5977026382871150005ULL : 778481730U);

    ASSERT_EQ(hash_functor(ip1), sizeof(size_t) == 8 ? 2753421670903790749ULL : 4009513643U);
    ASSERT_EQ(hash_functor(ip2), sizeof(size_t) == 8 ? 9476900596348728241ULL : 1628829895U);
    ASSERT_EQ(hash_functor(ip3), sizeof(size_t) == 8 ? 5977026382871150005ULL : 778481730U);
}

TEST(ipv4_address, Containers) {
    auto ip1 = ipv4_address::parse("127.0.0.1");
    auto ip2 = ipv4_address::parse("127.0.0.2");
    auto ip3 = ipv4_address::parse("127.0.0.3");

    std::vector<ipv4_address> vec;
    vec.push_back(ip1);
    vec.push_back(ip2);
    vec.emplace_back(ip3);
    vec.emplace_back(ipv4_address::base_type {127, 0, 0, 4});
    ASSERT_EQ(vec[0], ip1);
    ASSERT_EQ(vec[1], ip2);
    ASSERT_EQ(vec[2], ip3);
    ASSERT_EQ(vec[3], ipv4_address::parse("127.0.0.4"));

    std::map<ipv4_address, int> map;
    map[ip2] = 2;
    map[ip1] = 1;
    map[ip3] = 3;
    auto it = map.begin();
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(it++->first, ip1);
    ASSERT_EQ(it++->first, ip2);
    ASSERT_EQ(it++->first, ip3);
    
    std::unordered_map<ipv4_address, int> unordered_map;
    unordered_map[ip2] = 2;
    unordered_map[ip1] = 1;
    unordered_map[ip3] = 3;
    unordered_map[ip3] = 4;
    ASSERT_EQ(unordered_map.size(), 3);
    ASSERT_EQ(unordered_map[ip1], 1);
    ASSERT_EQ(unordered_map[ip2], 2);
    ASSERT_EQ(unordered_map[ip3], 4);
}

TEST(ipv4_address, Swap) {
    auto ip1 = ipv4_address::parse("127.0.0.1");
    auto ip2 = ipv4_address::parse("127.0.0.2");
    
    std::swap(ip1, ip2);

    ASSERT_EQ(ip1, ipv4_address::parse("127.0.0.2"));
    ASSERT_EQ(ip2, ipv4_address::parse("127.0.0.1"));
}

TEST(ipv4_address, reverse_pointer) {
    auto ip = ipv4_address::parse("127.0.0.1");
    
    const auto actual = ip.reverse_pointer();

    ASSERT_EQ(actual, "1.0.0.127.in-addr.arpa");
}

TEST(ipv4_address, literals) {
    IPADDRESS_CONSTEXPR auto ip1 = "127.0.0.1"_ipv4;
    IPADDRESS_CONSTEXPR auto ip2 = L"127.128.128.255"_ipv4;
    IPADDRESS_CONSTEXPR auto ip3 = 0x7F000001_ipv4;
    
    ASSERT_EQ(ip1, ipv4_address::parse("127.0.0.1"));
    ASSERT_EQ(ip2, ipv4_address::parse("127.128.128.255"));
    ASSERT_EQ(ip3, ipv4_address::parse("127.0.0.1"));
}

using IsMulticastIpv4Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsMulticastIpv4Params, is_multicast) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_address::parse(std::get<0>(GetParam())).is_multicast();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, IsMulticastIpv4Params,
    testing::Values(
        std::make_tuple("224.1.1.1", true),
        std::make_tuple("240.0.0.0", false)
    ));

using IsPrivateIpv4Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsPrivateIpv4Params, is_private) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_address::parse(std::get<0>(GetParam())).is_private();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, IsPrivateIpv4Params,
    testing::Values(
        std::make_tuple("192.168.1.1", true),
        std::make_tuple("192.169.0.0", false),
        std::make_tuple("10.255.255.255", true),
        std::make_tuple("11.0.0.0", false),
        std::make_tuple("172.31.255.255", true),
        std::make_tuple("172.32.0.0", false)
    ));

using IsGlobalIpv4Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsGlobalIpv4Params, is_global) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_address::parse(std::get<0>(GetParam())).is_global();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, IsGlobalIpv4Params,
    testing::Values(
        std::make_tuple("192.0.7.1", true),
        std::make_tuple("203.0.113.1", false)
    ));

using IsReservedIpv4Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsReservedIpv4Params, is_reserved) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_address::parse(std::get<0>(GetParam())).is_reserved();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, IsReservedIpv4Params,
    testing::Values(
        std::make_tuple("240.0.0.1", true),
        std::make_tuple("239.255.255.255", false)
    ));

using IsLoopbackIpv4Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsLoopbackIpv4Params, is_loopback) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_address::parse(std::get<0>(GetParam())).is_loopback();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, IsLoopbackIpv4Params,
    testing::Values(
        std::make_tuple("127.100.200.254", true),
        std::make_tuple("127.42.0.0", true),
        std::make_tuple("128.0.0.0", false)
    ));

using IsLinkLocalIpv4Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsLinkLocalIpv4Params, is_link_local) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_address::parse(std::get<0>(GetParam())).is_link_local();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, IsLinkLocalIpv4Params,
    testing::Values(
        std::make_tuple("169.254.100.200", true),
        std::make_tuple("169.255.100.200", false)
    ));

using IsUnspecifiedIpv4Params = TestWithParam<std::tuple<const char*, bool>>;
TEST_P(IsUnspecifiedIpv4Params, is_unspecified) {
    const auto expected = std::get<1>(GetParam());

    const auto actual = ipv4_address::parse(std::get<0>(GetParam())).is_unspecified();

    ASSERT_EQ(actual, expected);
}
INSTANTIATE_TEST_SUITE_P(
    ipv4_address, IsUnspecifiedIpv4Params,
    testing::Values(
        std::make_tuple("0.0.0.0", true)
    ));
