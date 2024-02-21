/**
 * \file      optional.hpp
 * \brief     The class template optional manages an optional contained value.
 * \author    Vladimir Shaleev
 * \copyright MIT License
 * 
 * A common use case for optional is the return value of a function that may fail. 
 * Any instance of `optional<T>` at any given point in time either contains a value or does not contain a value. 
 * 
 * If an `optional<T>` contains a value, the value is guaranteed to be allocated as part of the optional object 
 * footprint, i.e. no dynamic memory allocation ever takes place. 
 * 
 * When an object of type `optional<T>` is contextually converted to `bool`, the conversion returns `true` if the 
 * object contains a value and `false` if it does not contain a value.
 */

#ifndef IPADDRESS_OPTIONAL_HPP
#define IPADDRESS_OPTIONAL_HPP

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * The optional object.
 * 
 * The class template optional manages an optional contained value, 
 * i.e. a value that may or may not be present.
 */
template <typename T>
class optional {
public:
    using value_type = T;

    IPADDRESS_CONSTEXPR optional() IPADDRESS_NOEXCEPT = default;
    IPADDRESS_CONSTEXPR optional(std::nullptr_t) IPADDRESS_NOEXCEPT { };

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional(value_type&& val) IPADDRESS_NOEXCEPT
        :
        _has_value(true),
        _value(val) {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional(const optional<T>& opt) IPADDRESS_NOEXCEPT
        :
        _has_value(opt._has_value),
        _value(opt._value) {
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE optional& operator=(std::nullptr_t) IPADDRESS_NOEXCEPT {
        _has_value = false;
        _value = value_type{};
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE optional& operator=(value_type&& val) IPADDRESS_NOEXCEPT {
        _has_value = true;
        _value = val;
        return *this;
    }

    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE optional& operator=(const optional<T>& opt) IPADDRESS_NOEXCEPT {
        _has_value = opt._has_value;
        _value = opt._value;
        return *this;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE value_type* operator->() IPADDRESS_NOEXCEPT {
        return &_value;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const value_type* operator->() const IPADDRESS_NOEXCEPT {
        return &_value;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE value_type& operator*() IPADDRESS_NOEXCEPT {
        return _value;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const value_type& operator*() const IPADDRESS_NOEXCEPT {
        return _value;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator bool() const IPADDRESS_NOEXCEPT {
        return has_value();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool has_value() const IPADDRESS_NOEXCEPT {
        return _has_value;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE value_type& value() & IPADDRESS_NOEXCEPT {
        return _value;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const value_type& value() const & IPADDRESS_NOEXCEPT {
        return _value;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE value_type&& value() && IPADDRESS_NOEXCEPT {
        return std::move(_value);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const value_type&& value() const && IPADDRESS_NOEXCEPT {
        return std::move(_value);
    }

private:
    bool _has_value{};
    value_type _value{};
};

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_OPTIONAL_HPP
