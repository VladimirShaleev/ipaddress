
#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN 
#  include <Windows.h>
#else
#endif

#include <benchmark/benchmark.h>

#include <ipaddress/ipaddress.hpp>
#include <boost/asio.hpp>

class Ipv4AddressFixture : public benchmark::Fixture {
public:
    void SetUp(const benchmark::State& state) override {
        static const std::vector<std::string> addresses = {
            "127.0.0.1",
            "192.168.0.1",
            "0.0.0.0",
            "100.64.0.0",
            "192.168.1.1",
            "127.239.0.1",
            "127.128.128.255",
            "224.1.1.1"
        };

        const auto index = int(std::log2(state.range(0)));
        str = addresses[index % addresses.size()];
    }

    std::string str;
};

class Ipv6AddressFixture : public benchmark::Fixture {
public:
    void SetUp(const benchmark::State& state) override {
        static const std::vector<std::string> addresses = {
            "2001:db8::1",
            "0001:0002:0003:0004:0005:0006:0007:0008%12345",
            "::ffff:172.32.0.0",
            "2002:ac1d:2d64::1",
            "2001:0000:4136:e378:8000:63bf:3fff:fdd2",
            "2000::4136:e378:8000:63bf:3fff:fdd2",
            "2001:db8:0:0:1:0:0:1",
            "fe80::1ff:fe23:4567:890a%31"
        };

        const auto index = int(std::log2(state.range(0)));
        str = addresses[index % addresses.size()];
    }

    std::string str;
};

static void Arguments(benchmark::internal::Benchmark* b) {
    b->RangeMultiplier(2)->Range(1, 128);
}

// IPv4 address parser tests
// 
BENCHMARK_DEFINE_F(Ipv4AddressFixture, BM_parse_ipaddress)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(ipaddress::ipv4_address::parse(str).to_uint());
    }
}
BENCHMARK_REGISTER_F(Ipv4AddressFixture, BM_parse_ipaddress)->Apply(Arguments);

BENCHMARK_DEFINE_F(Ipv4AddressFixture, BM_parse_inet_pton)(benchmark::State& state) {
    for (auto _ : state) {
        in_addr ip_addr{}; 
        if (inet_pton(AF_INET, str.data(), &ip_addr) != 1) {
            state.SkipWithError("parse ip failed");
        }
        benchmark::DoNotOptimize(uint32_t(ntohl(ip_addr.s_addr)));
    }
}
BENCHMARK_REGISTER_F(Ipv4AddressFixture, BM_parse_inet_pton)->Apply(Arguments);

BENCHMARK_DEFINE_F(Ipv4AddressFixture, BM_parse_boost)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(boost::asio::ip::make_address_v4(str).to_uint());
    }
}
BENCHMARK_REGISTER_F(Ipv4AddressFixture, BM_parse_boost)->Apply(Arguments);

// IPv6 address parser tests
// 
BENCHMARK_DEFINE_F(Ipv6AddressFixture, BM_parse_ipaddress)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(ipaddress::ipv6_address::parse(str));
    }
}
BENCHMARK_REGISTER_F(Ipv6AddressFixture, BM_parse_ipaddress)->Apply(Arguments);

BENCHMARK_DEFINE_F(Ipv6AddressFixture, BM_parse_inet_pton)(benchmark::State& state) {
    for (auto _ : state) {
        in6_addr ip_addr{}; 
        if (inet_pton(AF_INET6, str.data(), &ip_addr) != 1) {
            state.SkipWithError("parse ip failed");
        }
        benchmark::DoNotOptimize(ip_addr.s6_addr);
    }
}
BENCHMARK_REGISTER_F(Ipv6AddressFixture, BM_parse_inet_pton)->Apply(Arguments);

BENCHMARK_DEFINE_F(Ipv6AddressFixture, BM_parse_boost)(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(boost::asio::ip::make_address_v6(str));
    }
}
BENCHMARK_REGISTER_F(Ipv6AddressFixture, BM_parse_boost)->Apply(Arguments);
