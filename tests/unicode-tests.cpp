#include <gtest/gtest.h>

#include <ipaddress/ipaddress.hpp>

using namespace ipaddress;

template <typename T, size_t N>
constexpr bool test_string(const T (&str)[N]) noexcept {
    const T* it = str;
    const T* end = it + N;
    while (it != end) {
        auto code = error_code::no_error;
        uint32_t error_symbol = 0;
        char symbol = internal::char_converter<T>::get_char_or_error(it, end, code, error_symbol);
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

TEST(char_converter, Constexpr) {
    constexpr auto result1 = test_string("1234");
#if __cpp_char8_t >= 201811L
    constexpr auto result2 = test_string(u8"1234");
    constexpr auto result3 = test_string(u8"12\ud55c34");
#endif
    constexpr auto result4 = test_string(u"1234");
    constexpr auto result5 = test_string(u"1猫4");
    constexpr auto result6 = test_string(U"1234");
    constexpr auto result7 = test_string(U"1猫4");
    constexpr auto result8 = test_string(L"1234");
    constexpr auto result9 = test_string(L"1猫4");

    ASSERT_TRUE(result1);
#if __cpp_char8_t >= 201811L
    ASSERT_TRUE(result2);
    ASSERT_FALSE(result3);
#endif
    ASSERT_TRUE(result4);
    ASSERT_FALSE(result5);
    ASSERT_TRUE(result6);
    ASSERT_FALSE(result7);
    ASSERT_TRUE(result8);
    ASSERT_FALSE(result9);
}

TEST(char_converter, Char) {
    constexpr char str[] = "abc123";

    const char* it = str;
    const char* end = it + sizeof(str);

    auto code = error_code::no_error;
    char symbol = '\0';
    uint32_t error_symbol = 0;

    symbol = internal::char_converter<char>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, 'a');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = internal::char_converter<char>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, 'b');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = internal::char_converter<char>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, 'c');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = internal::char_converter<char>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '1');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = internal::char_converter<char>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '2');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = internal::char_converter<char>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '3');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);

    ++it;
    symbol = internal::char_converter<char>::get_char_or_error(it, end, code, error_symbol);
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

    symbol = internal::char_converter<char8_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '1');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = internal::char_converter<char8_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '2');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
    
    ++it;
    symbol = internal::char_converter<char8_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0x10348);
    
    ++it;
    symbol = internal::char_converter<char8_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0xd55c);
    
    ++it;
    symbol = internal::char_converter<char8_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::unexpected_symbol);
    EXPECT_EQ(error_symbol, 0x0418);
    
    ++it;
    symbol = internal::char_converter<char8_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '$');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);

    ++it;
    symbol = internal::char_converter<char8_t>::get_char_or_error(it, end, code, error_symbol);
    EXPECT_EQ(symbol, '\0');
    EXPECT_EQ(code, error_code::no_error);
    EXPECT_EQ(error_symbol, 0);
}

#endif
