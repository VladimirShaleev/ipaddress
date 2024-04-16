#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;
using ipaddress::internal::char_converter;

template <typename T, size_t N>
constexpr bool constexpr_test_error(const T (&str)[N]) noexcept {
    const T* it = str;
    const T* end = it + N;
    while (it != end) {
        auto code = error_code::no_error;
        uint32_t error_symbol = 0;
        char symbol = char_converter<T>::get_char_or_error(it, end, code, error_symbol);
        if (code != error_code::no_error) {
            return false;
        }
        if (symbol == '\0') {
            break;
        }
        ++it;
    }
    return true;
}

template <typename T, size_t N>
constexpr bool constexpr_test(const T (&str)[N]) {
    const T* it = str;
    const T* begin = str;
    const T* end = it + N;
    while (it != end) {
        char symbol = char_converter<T>::get_char(it, begin, end);
        if (symbol == '\0') {
            break;
        }
        ++it;
    }
    return true;
}

template <typename T, size_t N>
void test(const T (&str)[N]) {
    const T* it = str;
    const T* end = it + N;

    auto code = error_code::no_error;
    char symbol = '\0';
    uint32_t error_symbol = 0;

    symbol = char_converter<T>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '1');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = char_converter<T>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '2');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = char_converter<T>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0x10348);
    
    ++it;
    symbol = char_converter<T>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0xd55c);
    
    ++it;
    symbol = char_converter<T>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0x0418);
    
    ++it;
    symbol = char_converter<T>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '$');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);

    ++it;
    symbol = char_converter<T>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
}

template <typename T, size_t N>
void test_exception(const T (&str)[N]) {
    const T* it = str;
    const T* begin = it;
    const T* end = it + sizeof(str);

    char symbol = '\0';

    symbol = char_converter<T>::get_char(it, begin, end);
    EXPECT_EQ(symbol, '1');
    
    ++it;
    symbol = char_converter<T>::get_char(it, begin, end);
    EXPECT_EQ(symbol, '2');
    
    ++it;

#ifdef IPADDRESS_NO_EXCEPTIONS
    const auto actual = char_converter<T>::get_char(it, begin, end);
    EXPECT_EQ(actual, '\0');
#elif IPADDRESS_CPP_VERSION >= 14
    EXPECT_THAT(
        [&]() { char_converter<T>::get_char(it, begin, end); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+10348} in address 12{U+10348}{U+d55c}{U+0418}$")));
#else
    ASSERT_THROW((char_converter<T>::get_char(it, begin, end)), parse_error);
#endif
}

#if IPADDRESS_CPP_VERSION >= 14

TEST(char_converter, CompileTime) {
    constexpr auto result1 = constexpr_test_error("1234");
    constexpr auto result2 = constexpr_test_error(u"1234");
    constexpr auto result3 = constexpr_test_error(u"1猫4");
    constexpr auto result4 = constexpr_test_error(U"1234");
    constexpr auto result5 = constexpr_test_error(U"1猫4");
    constexpr auto result6 = constexpr_test_error(L"1234");
    constexpr auto result7 = constexpr_test_error(L"1猫4");
    ASSERT_TRUE(result1);
    ASSERT_TRUE(result2);
    ASSERT_FALSE(result3);
    ASSERT_TRUE(result4);
    ASSERT_FALSE(result5);
    ASSERT_TRUE(result6);
    ASSERT_FALSE(result7);

    constexpr auto result8 = constexpr_test("1234");
    constexpr auto result9 = constexpr_test(u"1234");
    constexpr auto result10 = constexpr_test(U"1234");
    constexpr auto result11 = constexpr_test(L"1234");
    ASSERT_TRUE(result8);
    ASSERT_TRUE(result9);
    ASSERT_TRUE(result10);
    ASSERT_TRUE(result11);

#if __cpp_char8_t >= 201811L
    constexpr auto result12 = constexpr_test_error(u8"1234");
    constexpr auto result13 = constexpr_test_error(u8"12\ud55c34");
    constexpr auto result14 = constexpr_test(u8"1234");
    ASSERT_TRUE(result12);
    ASSERT_TRUE(result13);
    ASSERT_FALSE(result9);
#endif // __cpp_char8_t
}

#endif

#if __cpp_char8_t >= 201811L
TEST(char_converter, Utf8) {
    test(u8"12\U00010348\ud55c\u0418$");
}
#endif // __cpp_char8_t

TEST(char_converter, Utf16) {
    test(u"12\U00010348\ud55c\u0418$");
}

TEST(char_converter, Utf32) {
    test(U"12\U00010348\ud55c\u0418$");
}

TEST(char_converter, WideChar) {
    test(L"12\U00010348\ud55c\u0418$");
}

#if __cpp_char8_t >= 201811L
TEST(char_converter, Utf8Exception) {
    test_exception(u8"12\U00010348\ud55c\u0418$");
}
#endif // __cpp_char8_t

TEST(char_converter, Utf16Exception) {
    test_exception(u"12\U00010348\ud55c\u0418$");
}

TEST(char_converter, Utf32Exception) {
    test_exception(U"12\U00010348\ud55c\u0418$");
}

TEST(char_converter, WideCharException) {
    test_exception(L"12\U00010348\ud55c\u0418$");
}

TEST(char_converter, Char) {
    constexpr char str[] = "abc123";

    const char* it = str;
    const char* end = it + sizeof(str);

    auto code = error_code::no_error;
    char symbol = '\0';
    uint32_t error_symbol = 0;

    symbol = char_converter<char>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, 'a');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = char_converter<char>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, 'b');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = char_converter<char>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, 'c');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = char_converter<char>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '1');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = char_converter<char>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '2');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = char_converter<char>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '3');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);

    ++it;
    symbol = char_converter<char>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
}
