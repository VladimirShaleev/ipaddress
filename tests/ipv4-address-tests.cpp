#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;

TEST(ipv4_address, Test)
{
#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    auto ipv4 = ipv4_address::parse<"127.0.0.255">();
    constexpr auto ipv4_ = ipv4_address::parse<"127.0.0.1">();
    constexpr auto u = ipv4_.to_uint32();

    auto ipv6 = ipv6_address::parse<"::ffff:0:0">();
    auto ipv6_2 = ipv6_address::parse("::ffff:0:0:127.0.0.255");
#endif
    try {
        error_code err;
        std::string addr = "127.0.0.255";
        const char* addr2 = "127.0.0.255";
        //constexpr std::string_view addr3 = "127.0.0.255";
        auto ss = ipv4_address::parse(addr, err);
        auto ss2 = ipv4_address::parse(addr2, err);
        //constexpr auto ss3 = ipv4_address::parse(addr3);

        auto ipv4_2 = ipv4_address::parse("127.0.0.255");
    
        auto ipv4_2_ = ipv4_address::parse("127.0.0.256", err);
        auto b = ipv4_2_;
    }
    catch (const std::exception& exc) {
        auto b = exc;
    }
    //constexpr auto d = ipv4;

    //constexpr auto ipv4 = parse<"127.0.0.1">();

    // ipv4_address::ip_from_string()
}

TEST(ipv4_address, EmptyAddress) {
    ipv4_address::base_type expected_empty { 0, 0, 0, 0};

    error_code err;
    auto ip = ipv4_address::parse("", err);

    EXPECT_EQ(err, error_code::EMPTY_ADDRESS);
    EXPECT_EQ(ip.bytes(), expected_empty);
    EXPECT_EQ(ip.to_uint32(), 0);
    EXPECT_EQ(uint32_t(ip), 0);

    EXPECT_THAT(
        []() { ipv4_address::parse(""); },
        ThrowsMessage<std::runtime_error>(StrEq("address cannot be empty")));
}
