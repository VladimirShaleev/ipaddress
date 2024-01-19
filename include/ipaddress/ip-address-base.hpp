#ifndef IPADDRESS_IP_ADDRESS_BASE_HPP
#define IPADDRESS_IP_ADDRESS_BASE_HPP

#include "config.hpp"
#include "errors.hpp"
#include "endian.hpp"
#include "fixed-string.hpp"

namespace IPADDRESS_NAMESPACE {

template <typename Base>
class ip_address_base : public Base {
public:
    using base_type = typename Base::base_type;

    IPADDRESS_CONSTEXPR explicit ip_address_base(const base_type& bytes) IPADDRESS_NOEXCEPT : _bytes(bytes) {
    }

#ifdef IPADDRESS_NONTYPE_TEMPLATE_PARAMETER
    template <fixed_string FixedString>
    IPADDRESS_NODISCARD static consteval ip_address_base<Base> parse() IPADDRESS_NOEXCEPT {
        constexpr auto str = FixedString;
        auto code = error_code::NO_ERROR;
        auto index = 0;
        auto result = Base::ip_from_string(str.begin(), str.end(), code, index);
        if (code != error_code::NO_ERROR) {
            raise_error(code, index, str.data(), str.size());
        }
        return ip_address_base(result);
    }
#endif

#if IPADDRESS_CPP_VERSION >= 17
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR ip_address_base<Base> parse(std::string_view address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    IPADDRESS_NODISCARD static ip_address_base<Base> parse(std::string_view address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }
#else
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR ip_address_base<Base> parse(const std::string& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        return parse_string(address);
    }

    IPADDRESS_NODISCARD static ip_address_base<Base> parse(const std::string& address, error_code& code) IPADDRESS_NOEXCEPT {
        return parse_string(address, code);
    }
#endif

    template <size_t N>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR ip_address_base<Base> parse(const char(&address)[N]) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto str = make_fixed_string(address);
        return parse_string(str);
    }

    template <size_t N>
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR ip_address_base<Base> parse(const char(&address)[N], error_code& code) IPADDRESS_NOEXCEPT {
        auto str = make_fixed_string(address);
        return parse_string(str, code);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR const base_type& bytes() const IPADDRESS_NOEXCEPT {
        return _bytes;
    }

private:
    template <typename Str>
    static IPADDRESS_CONSTEXPR ip_address_base<Base> parse_string(const Str& address) IPADDRESS_NOEXCEPT_WHEN_NO_EXCEPTIONS {
        auto code = error_code::NO_ERROR;
        auto index = 0;
        auto result = Base::ip_from_string(address.begin(), address.end(), code, index);
        if (code != error_code::NO_ERROR) {
        #ifndef IPADDRESS_NO_EXCEPTIONS
            raise_error(code, index, address.data(), address.size());
        #endif
        }
        return ip_address_base(result);
    }

    template <typename Str>
    static ip_address_base<Base> parse_string(const Str& address, error_code& code) IPADDRESS_NOEXCEPT {
        code = error_code::NO_ERROR;
        auto index = 0;
        return ip_address_base(Base::ip_from_string(address.begin(), address.end(), code, index));
    }

    base_type _bytes;
};

}

#endif
