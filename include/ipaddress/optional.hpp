/**
 * @file      optional.hpp
 * @brief     Manages an optional contained value within a class template
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * The `optional` class template is designed to handle situations where a value
 * may or may not be present. It encapsulates a value in a way that does not require
 * dynamic memory allocation, ensuring that the value, if present, is part of the
 * `optional` object's footprint. This makes `optional` ideal for use as a return
 * type for functions that may fail to return a value. An `optional` object can be
 * contextually converted to `bool`, indicating whether a value is contained or not.
 */

#ifndef IPADDRESS_OPTIONAL_HPP
#define IPADDRESS_OPTIONAL_HPP

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

/**
 * A template class to manage an optional contained value.
 * 
 * The `optional` class template is often used to represent the outcome of a function 
 * that might not succeed. An `optional<T>` can either hold a value or be empty at any 
 * moment. When it does hold a value, that value is stored within the optional object 
 * itself, meaning there’s no need for separate dynamic memory allocation. If you 
 * check an `optional<T>` in a boolean context, such as in an if-statement, it will 
 * evaluate to true if there’s a value present, and false otherwise.
 * 
 * @tparam T the type of the value to manage initialization state for
 */
template <typename T>
class optional {
public:
    using value_type = T; /**< The type of the value to manage initialization state for */

    /**
     * Default constructor that constructs an `optional` object without a contained value.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional() IPADDRESS_NOEXCEPT = default;
    
    /**
     * Constructs an `optional` object that does not contain a value.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional(std::nullptr_t) IPADDRESS_NOEXCEPT { };

    /**
     * Move constructor that constructs an `optional` object with a contained value, initializing it with \a val.
     * 
     * @param[in,out] val the value to move into the `optional` object
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional(value_type&& val) IPADDRESS_NOEXCEPT
        :
        _has_value(true),
        _value(val) {
    }

    /**
     * Copy constructor that constructs an `optional` object with a contained value, copying it from \a opt.
     * 
     * @param[in] opt the `optional` object to copy the value from
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE optional(const optional<T>& opt) IPADDRESS_NOEXCEPT
        :
        _has_value(opt._has_value),
        _value(opt._value) {
    }

    /**
     * Assignment operator that clears the contained value of the `optional` object.
     * 
     * @return A reference to the `optional` object.
     */
    IPADDRESS_CONSTEXPR_14 IPADDRESS_FORCE_INLINE optional& operator=(std::nullptr_t) IPADDRESS_NOEXCEPT {
        _has_value = false;
        _value = value_type{};
        return *this;
    }

    /**
     * Move assignment operator that sets the contained value of the `optional` object to \a val.
     * 
     * @param[in,out] val the value to move into the `optional` object
     * @return        A reference to the `optional` object.
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
