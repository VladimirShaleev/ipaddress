#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <ipaddress/ipaddress.hpp>

using namespace testing;
using namespace ipaddress;
using ipaddress::internal::char_converter;

template <typename T, size_t N>
constexpr bool test_string_or_error(const T (&str)[N]) noexcept {
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
constexpr bool test_string(const T (&str)[N]) {
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
char test_exception(const T (&str)[N]) {
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
    return char_converter<T>::get_char(it, begin, end);
}

#if IPADDRESS_CPP_VERSION >= 14

TEST(char_converter, CompileTime) {
    constexpr auto result1 = test_string_or_error("1234");
    constexpr auto result2 = test_string_or_error(u"1234");
    constexpr auto result3 = test_string_or_error(u"1猫4");
    constexpr auto result4 = test_string_or_error(U"1234");
    constexpr auto result5 = test_string_or_error(U"1猫4");
    constexpr auto result6 = test_string_or_error(L"1234");
    constexpr auto result7 = test_string_or_error(L"1猫4");
    ASSERT_TRUE(result1);
    ASSERT_TRUE(result2);
    ASSERT_FALSE(result3);
    ASSERT_TRUE(result4);
    ASSERT_FALSE(result5);
    ASSERT_TRUE(result6);
    ASSERT_FALSE(result7);

#if __cpp_char8_t >= 201811L
    constexpr auto result8 = test_string_or_error(u8"1234");
    constexpr auto result9 = test_string_or_error(u8"12\ud55c34");
    ASSERT_TRUE(result8);
    ASSERT_FALSE(result9);
#endif

    constexpr auto result10 = test_string("1234");
    constexpr auto result11 = test_string(u"1234");
    constexpr auto result12 = test_string(U"1234");
    constexpr auto result13 = test_string_or_error(L"1234");
    ASSERT_TRUE(result10);
    ASSERT_TRUE(result11);
    ASSERT_TRUE(result12);
    ASSERT_TRUE(result13);

#if __cpp_char8_t >= 201811L
    constexpr auto result14 = test_string(u8"1234");
    ASSERT_TRUE(result14);
#endif
}

#endif

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

#if __cpp_char8_t >= 201811L

TEST(char_converter, Utf8) {
    constexpr char8_t str[] = u8"12\U00010348\ud55c\u0418$";

    const char8_t* it = str;
    const char8_t* end = it + sizeof(str);

    auto code = error_code::no_error;
    char symbol = '\0';
    uint32_t error_symbol = 0;

    symbol = char_converter<char8_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '1');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = char_converter<char8_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '2');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = char_converter<char8_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0x10348);
    
    ++it;
    symbol = char_converter<char8_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0xd55c);
    
    ++it;
    symbol = char_converter<char8_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0x0418);
    
    ++it;
    symbol = char_converter<char8_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '$');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);

    ++it;
    symbol = char_converter<char8_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
}

TEST(char_converter, Utf8Exception) {
    constexpr char8_t str[] = u8"12\U00010348\ud55c\u0418$";

#ifdef IPADDRESS_NO_EXCEPTIONS
    const auto actual = test_exception(str);
    EXPECT_EQ(actual, '\0');
#elif IPADDRESS_CPP_VERSION >= 14
    EXPECT_THAT(
        [&]() { test_exception(str); },
        ThrowsMessage<parse_error>(StrEq("unexpected next unicode symbol {U+10348} in address 12")));
#else
    ASSERT_THROW(test_exception(str), parse_error);
#endif
}

#endif

TEST(char_converter, Utf16) {
    constexpr char16_t str[] = u"12\U00010348\ud55c\u0418$";

    const char16_t* it = str;
    const char16_t* end = it + sizeof(str);

    auto code = error_code::no_error;
    char symbol = '\0';
    uint32_t error_symbol = 0;

    symbol = char_converter<char16_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '1');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = char_converter<char16_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '2');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = char_converter<char16_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0x10348);
    
    ++it;
    symbol = char_converter<char16_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0xd55c);
    
    ++it;
    symbol = char_converter<char16_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0x0418);
    
    ++it;
    symbol = char_converter<char16_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '$');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);

    ++it;
    symbol = char_converter<char16_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
}

TEST(char_converter, Utf32) {
    constexpr char32_t str[] = U"12\U00010348\ud55c\u0418$";

    const char32_t* it = str;
    const char32_t* end = it + sizeof(str);

    auto code = error_code::no_error;
    char symbol = '\0';
    uint32_t error_symbol = 0;

    symbol = char_converter<char32_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '1');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = char_converter<char32_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '2');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = char_converter<char32_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0x10348);
    
    ++it;
    symbol = char_converter<char32_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0xd55c);
    
    ++it;
    symbol = char_converter<char32_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0x0418);
    
    ++it;
    symbol = char_converter<char32_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '$');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);

    ++it;
    symbol = char_converter<char32_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
}
