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
 * 
 * @tparam T the type of the value to manage initialization state for
 */
template <typename T>
class optional {
public:
    using value_type = T; /**< The type of the value to manage initialization state for */

    /**
     * Default constructor.
     * 
     * Constructs an object that does not contain a value.
     */
    IPADDRESS_CONSTEXPR optional() IPADDRESS_NOEXCEPT = default;
    
    /**
     * Constructs the optional object.
     * 
     * Constructs an object that does *not* contain a value.
     */
    IPADDRESS_CONSTEXPR optional(std::nullptr_t) IPADDRESS_NOEXCEPT { };

    /**
     * Constructs the optional object.
     * 
     * Move constructor.
     * 
     * @param val of move object
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional(value_type&& val) IPADDRESS_NOEXCEPT
        :
        _has_value(true),
        _value(val) {
    }

    /**
     * Constructs the optional object.
     * 
     * Copy constructor.
     * 
     * @param opt of copy object
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional(const optional<T>& opt) IPADDRESS_NOEXCEPT
        :
        _has_value(opt._has_value),
        _value(opt._value) {
    }

    /**
     * Assigns contents.
     * 
     * Clear contained value.
     * 
     * @return This object.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE optional& operator=(std::nullptr_t) IPADDRESS_NOEXCEPT {
        _has_value = false;
        _value = value_type{};
        return *this;
    }

    /**
     * Assigns contents.
     * 
     * Move operator for value.
     * 
     * @return This object.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE optional& operator=(value_type&& val) IPADDRESS_NOEXCEPT {
        _has_value = true;
        _value = val;
        return *this;
    }

    /**
     * Assigns contents.
     * 
     * Copy operator for other optional object.
     * 
     * @return This object.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE optional& operator=(const optional<T>& opt) IPADDRESS_NOEXCEPT {
        _has_value = opt._has_value;
        _value = opt._value;
        return *this;
    }

    /**
     * Accesses the contained value.
     * 
     * Returns a pointer to the contained value. The behavior is 
     * undefined if `*this` does not contain a value.
     * 
     * @return Pointer to the contained value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE value_type* operator->() IPADDRESS_NOEXCEPT {
        return &_value;
    }

    /**
     * Accesses the contained value.
     * 
     * Returns a pointer to the contained value. The behavior is 
     * undefined if `*this` does not contain a value.
     * 
     * @return Constant pointer to the contained value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const value_type* operator->() const IPADDRESS_NOEXCEPT {
        return &_value;
    }

    /**
     * Accesses the contained value.
     * 
     * Returns a reference to the contained value. The behavior is 
     * undefined if `*this` does not contain a value.
     * 
     * @return Reference to the contained value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE value_type& operator*() IPADDRESS_NOEXCEPT {
        return _value;
    }

    /**
     * Accesses the contained value.
     * 
     * Returns a reference to the contained value. The behavior is 
     * undefined if `*this` does not contain a value.
     * 
     * @return Constant reference to the contained value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const value_type& operator*() const IPADDRESS_NOEXCEPT {
        return _value;
    }

    /**
     * Checks whether contains a value.
     * 
     * @return `true` if contains a value, `false` if does not contain a value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit operator bool() const IPADDRESS_NOEXCEPT {
        return has_value();
    }

    /**
     * Checks whether contains a value.
     * 
     * @return `true` if contains a value, `false` if does not contain a value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool has_value() const IPADDRESS_NOEXCEPT {
        return _has_value;
    }

    /**
     * Returns a reference to the contained value.
     * 
     * @return A reference to the contained value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE value_type& value() & IPADDRESS_NOEXCEPT {
        return _value;
    }

    /**
     * Returns a reference to the contained value.
     * 
     * @return A constant reference to the contained value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const value_type& value() const & IPADDRESS_NOEXCEPT {
        return _value;
    }

    /**
     * Move the contained value.
     * 
     * @return Contained value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE value_type&& value() && IPADDRESS_NOEXCEPT {
        return std::move(_value);
    }

    /**
     * Move the contained value.
     * 
     * @return Constant value.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const value_type&& value() const && IPADDRESS_NOEXCEPT {
        return std::move(_value);
    }

private:
    bool _has_value{};
    value_type _value{};
};

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_OPTIONAL_HPP
