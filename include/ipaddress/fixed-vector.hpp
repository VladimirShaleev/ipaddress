/**
 * @file      fixed-vector.hpp
 * @brief     Provides a fixed-size vector class template
 * @author    Vladimir Shaleev
 * @copyright MIT License
 * 
 * The fixed_vector template class is a lightweight container that encapsulates a fixed-size array.
 * It provides a subset of the functionality of std::vector, optimized for scenarios where the size
 * of the container is known at compile-time. This class is designed to work in constexpr contexts,
 * enabling compile-time operations on fixed-size arrays. It ensures efficient memory usage and
 * avoids dynamic memory allocation.
 */

#ifndef IPADDRESS_FIXED_VECTOR_HPP
#define IPADDRESS_FIXED_VECTOR_HPP
 
#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

namespace internal {

template <typename It>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE typename std::iterator_traits<It>::difference_type get_distance(It first, It last, std::random_access_iterator_tag) IPADDRESS_NOEXCEPT {
    return last - first;
}

template <typename It, typename Tag>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE typename std::iterator_traits<It>::difference_type get_distance(It first, It last, Tag) IPADDRESS_NOEXCEPT {
    typename std::iterator_traits<It>::difference_type count = 0;
    for (auto it = first; it != last; ++it) {
        ++count;
    }
    return count;
}

template<class It>
IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE typename std::iterator_traits<It>::difference_type distance(It first, It last) IPADDRESS_NOEXCEPT {
    return get_distance(first, last, typename std::iterator_traits<It>::iterator_category{});
}

} // namespace IPADDRESS_NAMESPACE::internal

/**
 * A fixed-size vector iterator class template.
 * 
 * The fixed_vector_iterator class template provides an iterator for the fixed_vector class template.
 * It allows for iteration over the elements in the fixed_vector.
 * 
 * @tparam T type of the elements in the vector.
 */
IPADDRESS_EXPORT template <typename T>
class fixed_vector_iterator {
public:
    using iterator_category = std::random_access_iterator_tag; /**< The iterator category. */
    using value_type        = T; /**< The type of the elements. */
    using difference_type   = ptrdiff_t; /**< The type used for representing differences between iterators. */
    using pointer           = T*; /**< Pointer to the element type. */
    using reference         = T&; /**< Reference to the element type. */

    /**
     * Default constructor.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector_iterator() IPADDRESS_NOEXCEPT = default;

    /**
     * Copy constructor.
     * 
     * @param[in] it The iterator to copy from.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector_iterator(const fixed_vector_iterator& it) IPADDRESS_NOEXCEPT = default;
    
    /**
     * Copy assignment operator.
     * 
     * @param[in] it The iterator to copy from.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector_iterator& operator=(const fixed_vector_iterator& it) IPADDRESS_NOEXCEPT = default;

    /**
     * Constructs a fixed_vector_iterator from a pointer.
     * 
     * @param[in] ptr The pointer to the element type.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit fixed_vector_iterator(pointer ptr) IPADDRESS_NOEXCEPT : _ptr(ptr) {
    }

    /**
     * Constructs a const-compatible fixed_vector_iterator from a non-const fixed_vector_iterator.
     * 
     * @param[in] it The iterator to convert.
     */
    template <typename U, typename = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>  
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector_iterator(const fixed_vector_iterator<U>& it) IPADDRESS_NOEXCEPT : _ptr(it.operator->()) {
    }

    /**
     * Dereference operator.
     * 
     * @return A reference to the element pointed to by the iterator.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator*() const IPADDRESS_NOEXCEPT {
        return *_ptr;
    }

    /**
     * Pointer access operator.
     * 
     * @return A pointer to the element pointed to by the iterator.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer operator->() const IPADDRESS_NOEXCEPT {
        return _ptr;
    }

    /**
     * Access operator.
     * 
     * @param[in] n The index of the element to access.
     * @return A reference to the element at index n.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator[](size_t n) const IPADDRESS_NOEXCEPT {
        return *(_ptr + n);
    }

    /**
     * Pre-increment operator.
     * 
     * @return A reference to the iterator after incrementing.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector_iterator& operator++() IPADDRESS_NOEXCEPT {
        ++_ptr;
        return *this;
    }

    /**
     * Post-increment operator.
     * 
     * @return A copy of the iterator before incrementing.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector_iterator operator++(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        ++_ptr;
        return tmp;
    }

    /**
     * Pre-decrement operator.
     * 
     * @return A reference to the iterator after decrementing.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector_iterator& operator--() IPADDRESS_NOEXCEPT {
        --_ptr;
        return *this;
    }

    /**
     * Post-decrement operator.
     * 
     * @return A copy of the iterator before decrementing.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector_iterator operator--(int) IPADDRESS_NOEXCEPT {
        auto tmp = *this;
        --_ptr;
        return tmp;
    }

    /**
     * Addition assignment operator.
     * 
     * @param[in] n The number of elements to add.
     * @return A reference to the iterator after addition.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector_iterator& operator+=(difference_type n) IPADDRESS_NOEXCEPT {
        _ptr += n;
        return *this;
    }

    /**
     * Subtraction assignment operator.
     * 
     * @param[in] n The number of elements to subtract.
     * @return A reference to the iterator after subtraction.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector_iterator& operator-=(difference_type n) IPADDRESS_NOEXCEPT {
        _ptr -= n;
        return *this;
    }

    /**
     * Addition operator.
     * 
     * @param[in] n The number of elements to add.
     * @return A new fixed_vector_iterator instance after addition.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector_iterator operator+(difference_type n) const IPADDRESS_NOEXCEPT {
        return fixed_vector_iterator(_ptr + n);
    }
    
    /**
     * Addition operator.
     * 
     * @param[in] n The number of elements to add.
     * @param[in] it The iterator to add to.
     * @return A new fixed_vector_iterator instance after addition.
     */
    IPADDRESS_NODISCARD friend IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector_iterator operator+(difference_type n, const fixed_vector_iterator& it) IPADDRESS_NOEXCEPT {
        return it + n;
    }

    /**
     * Subtraction operator.
     * 
     * @param[in] n The number of elements to subtract.
     * @return A new fixed_vector_iterator instance after subtraction.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector_iterator operator-(difference_type n) const IPADDRESS_NOEXCEPT {
        return fixed_vector_iterator(_ptr - n);
    }

    /**
     * Subtraction operator.
     * 
     * @param[in] it The iterator to subtract from.
     * @return The difference between the two iterators.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE difference_type operator-(const fixed_vector_iterator& it) const IPADDRESS_NOEXCEPT {
        return _ptr - it._ptr;
    }

    /**
     * Equality operator.
     * 
     * @param[in] it The iterator to compare with.
     * @return true if this iterator is equal to the other iterator, false otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator==(const fixed_vector_iterator& it) const IPADDRESS_NOEXCEPT {
        return _ptr == it._ptr;
    }

    /**
     * Inequality operator.
     * 
     * @param[in] it The iterator to compare with.
     * @return true if this iterator is not equal to the other iterator, false otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator!=(const fixed_vector_iterator& it) const IPADDRESS_NOEXCEPT {
        return !(*this == it);
    }

    
#ifdef IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Spaceship operator.
     * 
     * @param[in] it The iterator to compare with.
     * @return A strong_ordering value representing the comparison result.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE std::strong_ordering operator<=>(const fixed_vector_iterator& it) const IPADDRESS_NOEXCEPT {
        return _ptr <=> it._ptr;
    }

#else // !IPADDRESS_HAS_SPACESHIP_OPERATOR

    /**
     * Less than operator.
     * 
     * @param[in] it The iterator to compare with.
     * @return true if this iterator is less than the other iterator, false otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<(const fixed_vector_iterator& it) const IPADDRESS_NOEXCEPT {
        return _ptr < it._ptr;
    }
    
    /**
     * Less than or equal to operator.
     * 
     * @param[in] it The iterator to compare with.
     * @return true if this iterator is less than or equal to the other iterator, false otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator<=(const fixed_vector_iterator& it) const IPADDRESS_NOEXCEPT {
        return !(it < *this);
    }
    
    /**
     * Greater than operator.
     * 
     * @param[in] it The iterator to compare with.
     * @return true if this iterator is greater than the other iterator, false otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>(const fixed_vector_iterator& it) const IPADDRESS_NOEXCEPT {
        return it < *this;
    }
    
    /**
     * Greater than or equal to operator.
     * 
     * @param[in] it The iterator to compare with.
     * @return true if this iterator is greater than or equal to the other iterator, false otherwise.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool operator>=(const fixed_vector_iterator& it) const IPADDRESS_NOEXCEPT {
        return !(*this < it);
    }

#endif // !IPADDRESS_HAS_SPACESHIP_OPERATOR

private:
    pointer _ptr{};
}; // fixed_vector_iterator;

/**
 * A fixed-size vector class template.
 * 
 * The fixed_vector class template provides a simple fixed-size array wrapper that allows for compile-time operations.
 * 
 * @tparam T type of the elements in the vector.
 * @tparam N capacity of the vector.
 * @remark The fixed_vector class is a simple fixed-size array wrapper that provides a subset of the functionality of std::vector.
 *         This class is designed to work in constexpr contexts, allowing compile-time operations on fixed-size arrays.
 */
IPADDRESS_EXPORT template <typename T, size_t N>
class fixed_vector {
public:
    using value_type             = T; /**< type of the elements in the vector. */
    using size_type              = size_t; /**< type used for size representation. */
    using difference_type        = ptrdiff_t; /**< type used for representing differences between iterators. */
    using pointer                = value_type*; /**< type used for pointer to elements. */
    using const_pointer          = const value_type*; /**< type used for pointer to constant elements. */
    using reference              = value_type&; /**< type used for reference to elements. */
    using const_reference        = const value_type&; /**< type used for reference to constant elements. */
    using iterator               = fixed_vector_iterator<value_type>; /**< type used for iterator to elements.  */
    using const_iterator         = fixed_vector_iterator<const value_type>; /**< type used for iterator to constant elements. */
    using reverse_iterator       = std::reverse_iterator<iterator>; /**< type used for reverse iterator. */
    using const_reverse_iterator = std::reverse_iterator<const_iterator>; /**< type used for reverse iterator to constant elements. */

    /**
     * Default constructor.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector() IPADDRESS_NOEXCEPT = default;

    /**
     * Constructs a fixed_vector with the specified number of default-initialized elements.
     * 
     * @param[in] n The number of elements to initialize.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit fixed_vector(size_type n) IPADDRESS_NOEXCEPT {
        assert(n <= max_size());
        _size = n;
    }

    /**
     * Constructs a fixed_vector with the specified number of elements initialized to the given value.
     * 
     * @param[in] n The number of elements to initialize.
     * @param[in] value The value to initialize the elements with.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE explicit fixed_vector(size_type n, const_reference value) IPADDRESS_NOEXCEPT {
        assign(n, value);
    }

    /**
     * Constructs a fixed_vector from a range of elements.
     * 
     * @tparam It The iterator type.
     * @param[in] first The beginning iterator of the range.
     * @param[in] last The ending iterator of the range.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    template <class It>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector(It first, It last) IPADDRESS_NOEXCEPT {
        assign(first, last);
    }

    /**
     * Constructs a fixed_vector from an initializer list.
     * 
     * @param[in] init_list The initializer list of elements.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector(std::initializer_list<value_type> init_list) IPADDRESS_NOEXCEPT : fixed_vector(init_list.begin(), init_list.end()) {
    }

    /**
     * Replaces the contents with the specified number of copies of the given value.
     * 
     * @param[in] n The number of elements to assign.
     * @param[in] value The value to assign to the elements.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void assign(size_type n, const_reference value) IPADDRESS_NOEXCEPT {
        assert(n <= max_size());
        _size = n;
        for (size_type i = 0; i < n; ++i) {
            _data[i] = value;
        }
    }

    /**
     * Replaces the contents with the elements in the specified range.
     * 
     * @tparam It The iterator type.
     * @param[in] first The beginning iterator of the range.
     * @param[in] last The ending iterator of the range.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    template <class It>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void assign(It first, It last) IPADDRESS_NOEXCEPT {
        _size = 0;
        for (auto it = first; it != last; ++it) {
            assert(_size < max_size());
            _data[_size++] = *it;
        }
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void assign(std::initializer_list<value_type> init_list) IPADDRESS_NOEXCEPT {
        assign(init_list.begin(), init_list.end());
    }

    /**
     * Accesses an element by index.
     * 
     * @param[in] n The index of the element.
     * @return The element at the specified index.
     * @remark n must be less than the size of the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference at(size_type n) IPADDRESS_NOEXCEPT {
        assert(n < size());
        return _data[n];
    }

    /**
     * Accesses an element by index.
     * 
     * @param[in] n The index of the element.
     * @return The element at the specified index.
     * @remark n must be less than the size of the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference at(size_type n) const IPADDRESS_NOEXCEPT {
        assert(n < size());
        return _data[n];
    }

    /**
     * Accesses an element by index.
     * 
     * @param[in] n The index of the element.
     * @return The element at the specified index.
     * @remark n must be less than the size of the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator[](size_type n) IPADDRESS_NOEXCEPT {
        return at(n);
    }

    /**
     * Accesses an element by index.
     * 
     * @param[in] n The index of the element.
     * @return The element at the specified index.
     * @remark n must be less than the size of the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference operator[](size_type n) const IPADDRESS_NOEXCEPT {
        return at(n);
    }

    /**
     * Accesses the first element in the vector.
     * 
     * @return A reference to the first element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference front() IPADDRESS_NOEXCEPT {
        assert(!empty());
        return _data[0];
    }

    /**
     * Accesses the first element in the vector.
     * 
     * @return A reference to the first element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference front() const IPADDRESS_NOEXCEPT {
        assert(!empty());
        return _data[0];
    }

    /**
     * Accesses the last element in the vector.
     * 
     * @return A reference to the last element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference back() IPADDRESS_NOEXCEPT {
        assert(!empty());
        return _data[_size - 1];
    }

    /**
     * Accesses the last element in the vector.
     * 
     * @return A reference to the last element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference back() const IPADDRESS_NOEXCEPT {
        assert(!empty());
        return _data[_size - 1];
    }

    /**
     * Returns a pointer to the underlying array.
     * 
     * @return A pointer to the underlying array of type value_type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer data() IPADDRESS_NOEXCEPT {
        return _data;
    }

    /**
     * Returns a const pointer to the underlying array (const version).
     * 
     * @return A const pointer to the underlying array of type value_type.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_pointer data() const IPADDRESS_NOEXCEPT {
        return _data;
    }

    /**
     * Return iterator to the first element.
     * 
     * @return An iterator to the first element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator begin() IPADDRESS_NOEXCEPT {
        return iterator(_data);
    }

    /**
     * Return const iterator to the first element
     * 
     * @return A const iterator to the first element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator begin() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data);
    }

    /**
     * Return const iterator to the first element.
     * 
     * @return A const iterator to the first element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cbegin() const IPADDRESS_NOEXCEPT {
        return begin();
    }

    /**
     * Returns an reverse iterator to the first element.
     * 
     * @return A reverse iterator to the first element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reverse_iterator rbegin() IPADDRESS_NOEXCEPT {
        return reverse_iterator(end());
    }

    /**
     * Returns a const iterator to the first element.
     * 
     * @return A const reverse iterator to the first element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator rbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(end());
    }

    /**
     * Returns a const reverse iterator to the first element.
     * 
     * @return A const reverse iterator to the first element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator crbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cend());
    }

    /**
     * Returns an iterator to the element following the last element.
     * 
     * @return An iterator to the element following the last element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator end() IPADDRESS_NOEXCEPT {
        return iterator(_data + _size);
    }

    /**
     * Returns a const iterator to the element following the last element.
     * 
     * @return A const iterator to the element following the last element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator end() const IPADDRESS_NOEXCEPT {
        return const_iterator(_data + _size);
    }

    /**
     * Returns a const iterator to the element following the last element.
     * 
     * @return A const iterator to the element following the last element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cend() const IPADDRESS_NOEXCEPT {
        return end();
    }

    /**
     * Returns a reverse iterator to the element following the last element.
     * 
     * @return A reverse iterator to the element following the last element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reverse_iterator rend() IPADDRESS_NOEXCEPT {
        return reverse_iterator(begin());
    }
    
    /**
     * Returns a const reverse iterator to the element following the last element.
     * 
     * @return A const reverse iterator to the element following the last element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator rend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(begin());
    }
    
    /**
     * Returns a const reverse iterator to the element following the last element.
     * 
     * @return A const reverse iterator to the element following the last element in the vector.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator crend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cbegin());
    }

    /**
     * Determines whether the container is empty.
     * 
     * @return true if the container is empty; otherwise, false.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool empty() const IPADDRESS_NOEXCEPT {
        return _size == 0;
    }

    /**
     * Returns the number of elements in the container.
     * 
     * @return The number of elements in the container.
     */
    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_type size() const IPADDRESS_NOEXCEPT {
        return _size;
    }

    /**
     * Returns the maximum number of elements that the container can hold.
     * 
     * @return The maximum number of elements that the container can hold.
     */
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_type max_size() IPADDRESS_NOEXCEPT {
        return N;
    }

    /**
     * Returns the maximum number of elements that the container can hold.
     * 
     * @return The maximum number of elements that the container can hold.
     */
    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_type capacity() IPADDRESS_NOEXCEPT {
        return N;
    }

    /**
     * Resizes the container to the specified size.
     * 
     * If n is less than the current size, the container is truncated.
     * If n is greater, default-inserted elements are added.
     * 
     * @param[in] n The new size of the container.
     * @remark n must not exceed the maximum size.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void resize(size_type n) IPADDRESS_NOEXCEPT {
        resize(n, value_type{});
    }

    /**
     * Resizes the container to the specified size and initializes new elements with the given value.
     * 
     * If n is less than the current size, the container is truncated.
     * If n is greater, copies of value are appended.
     * 
     * @param[in] n The new size of the container.
     * @param[in] value The value to initialize new elements with.
     * @remark n must not exceed the maximum size.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void resize(size_type n, const_reference value) IPADDRESS_NOEXCEPT {
        if (n < _size) {
            _size = n;
        } else {
            assert(n < max_size());
            for (size_type i = _size; i < n; ++i) {
                _data[i] = value;
            }
            _size = n;
        }
    }

    /**
     * Reserves space for the specified number of elements.
     * 
     * @param[in] n The number of elements to reserve space for.
     * @note This function does not change the size of the container.
     *       It is a no-op for fixed_vector, as the size is fixed at compile time.
     *       The function is provided for compatibility with std::vector.
     * @remark n must not exceed the maximum size.
     */
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void reserve(size_type n /* NOLINT(misc-unused-parameters) */) IPADDRESS_NOEXCEPT {
        assert(n < max_size());
    }

    /**
     * Shrinks the container to fit its current size.
     * 
     * @note This function does not change the size of the container.
     *       It is a no-op for fixed_vector, as the size is fixed at compile time.
     *       The function is provided for compatibility with std::vector.
     */
    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void shrink_to_fit() IPADDRESS_NOEXCEPT {
    }

    /**
     * Inserts copies of the given value at the specified position.
     * 
     * @param[in] pos The position to insert the value at.
     * @param[in] value The value to insert.
     * @return An iterator to the inserted element.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator insert(const_iterator pos, const_reference value) IPADDRESS_NOEXCEPT {
        return insert_n(pos, 1, value);
    }

    /**
     * Moves the given value at the specified position.
     * 
     * @param[in] pos The position to insert the value at.
     * @param[in] value The value to insert.
     * @return An iterator to the inserted element.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator insert(const_iterator pos, value_type&& value) IPADDRESS_NOEXCEPT {
        return insert_n(pos, 1, std::move(value));
    }

    /**
     * Inserts copies of the given value at the specified position.
     * 
     * @param[in] pos The position to insert the value at.
     * @param[in] n The number of copies to insert.
     * @param[in] value The value to insert.
     * @return An iterator pointing to the first inserted element.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator insert(const_iterator pos, size_type n, const_reference value) IPADDRESS_NOEXCEPT {
        return insert_n(pos, n, value);
    }

    /**
     * Inserts range of elements at the specified position.
     * 
     * @tparam It The iterator type.
     * @param[in] pos The position to insert the elements at.
     * @param[in] first The beginning iterator of the range.
     * @param[in] last The ending iterator of the range.
     * @return An iterator pointing to the first inserted element.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    template <typename It>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator insert(const_iterator pos, It first, It last) IPADDRESS_NOEXCEPT {
        return insert_iterators(pos, first, last);
    }

    /**
     * Inserts initializer list of elements at the specified position.
     * 
     * @param[in] pos The position to insert the elements at.
     * @param[in] init_list The initializer list of elements.
     * @return An iterator pointing to the first inserted element.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator insert(const_iterator pos, std::initializer_list<value_type> init_list) IPADDRESS_NOEXCEPT {
        return insert_iterators(pos, init_list.begin(), init_list.end());
    }

    /**
     * Inserts a new element at the specified position, constructed in-place with the given arguments.
     * 
     * @tparam Args The argument types for constructing the element.
     * @param[in] pos The position to insert the element at.
     * @param[in] args The arguments for constructing the element.
     * @return An iterator pointing to the inserted element.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    template <typename... Args>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator emplace(const_iterator pos, Args&&... args) IPADDRESS_NOEXCEPT {
        return insert_n(pos, 1, value_type(std::forward<Args>(args)...));
    }

    /**
     * Inserts a new element at the end of the vector, constructed in-place with the given arguments.
     * 
     * @tparam Args The argument types for constructing the element.
     * @param[in] args The arguments for constructing the element.
     * @return A reference to the inserted element.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    template <typename... Args>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference emplace_back(Args&&... args) IPADDRESS_NOEXCEPT {
        assert(size() < max_size());
        _data[_size++] = value_type(std::forward<Args>(args)...);
        return _data[_size - 1];
    }

    /**
     * Inserts a new element at the end of the vector, constructed in-place with the given arguments.
     * 
     * @tparam Args The argument types for constructing the element.
     * @param[in] args The arguments for constructing the element.
     * @return A pointer to the inserted element, or nullptr if the insertion failed.
     */
    template <typename... Args>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer try_emplace_back(Args&&... args) IPADDRESS_NOEXCEPT {
        if (size() < max_size()) {
            _data[_size] = value_type(std::forward<Args>(args)...);
            return &_data[_size++];
        } else {
            return nullptr;
        }
    }

    /**
     * Inserts a new element at the end of the vector, constructed in-place with the given arguments.
     * 
     * @tparam Args The argument types for constructing the element.
     * @param[in] args The arguments for constructing the element.
     * @return A reference to the inserted element.
     * @remark The condition size() < capacity() must hold true before calling this function. Otherwise, the behavior is undefined.
     */
    template <typename... Args>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference unchecked_emplace_back(Args&&... args) IPADDRESS_NOEXCEPT {
        _data[_size++] = value_type(std::forward<Args>(args)...);
        return _data[_size - 1];
    }

    /**
     * Adds a new element at the end of the vector.
     * 
     * @param[in] value The value to add.
     * @return A reference to the added element.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference push_back(const_reference value) IPADDRESS_NOEXCEPT {
        assert(size() < max_size());
        _data[_size++] = value;
        return _data[_size - 1];
    }

    /**
     * Adds a new element at the end of the vector.
     * 
     * @param[in] value The value to add.
     * @return A reference to the added element.
     * @remark The number of elements must not exceed the maximum size of the vector.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference push_back(value_type&& value) IPADDRESS_NOEXCEPT {
        assert(size() < max_size());
        _data[_size++] = std::move(value);
        return _data[_size - 1];
    }

    /**
     * Adds a new element at the end of the vector.
     * 
     * @param[in] value The value to add.
     * @return A pointer to the added element, or nullptr if the insertion failed.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer try_push_back(const_reference value) IPADDRESS_NOEXCEPT {
        if (size() < max_size()) {
            _data[_size] = value;
            return &_data[_size++];
        } else {
            return nullptr;
        }
    }

    /**
     * Adds a new element at the end of the vector.
     * 
     * @param[in] value The value to add.
     * @return A pointer to the added element, or nullptr if the insertion failed.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer try_push_back(value_type&& value) IPADDRESS_NOEXCEPT {
        if (size() < max_size()) {
            _data[_size] = std::move(value);
            return &_data[_size++];
        } else {
            return nullptr;
        }
    }

    /**
     * Adds a new element at the end of the vector without checking the size.
     * 
     * @param[in] value The value to add.
     * @return A reference to the added element.
     * @remark The condition size() < capacity() must hold true before calling this function. Otherwise, the behavior is undefined.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference unchecked_push_back(const_reference value) IPADDRESS_NOEXCEPT {
        _data[_size++] = value;
        return _data[_size - 1];
    }

    /**
     * Adds a new element at the end of the vector without checking the size.
     * 
     * @param[in] value The value to add.
     * @return A reference to the added element.
     * @remark The condition size() < capacity() must hold true before calling this function. Otherwise, the behavior is undefined.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference unchecked_push_back(value_type&& value) IPADDRESS_NOEXCEPT {
        _data[_size++] = std::move(value);
        return _data[_size - 1];
    }

    /**
     * Removes the last element from the vector.
     * 
     * @remark The vector must not be empty before calling this function.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void pop_back() IPADDRESS_NOEXCEPT {
        assert(!empty());
        --_size;
    }

    /**
     * Removes all elements from the vector.
     * 
     * @remark The vector will be empty after calling this function.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void clear() IPADDRESS_NOEXCEPT {
        _size = 0;
    }

    /**
     * Removes the element at the specified position.
     * 
     * @param[in] pos The position of the element to remove.
     * @return An iterator to the next element after the removed element.
     * @remark The vector must not be empty before calling this function.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator erase(const_iterator pos) IPADDRESS_NOEXCEPT {
        return erase(pos, pos + 1);
    }
    
    /**
     * Remove range of elements from the vector.
     * 
     * @param[in] first The position of the first element to remove.
     * @param[in] last The position of the last element to remove.
     * @return An iterator to the next element after the last removed element.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator erase(const_iterator first, const_iterator last) IPADDRESS_NOEXCEPT {
        assert(first >= begin() && last <= end() && first <= last);
        const auto count = internal::distance(first, last);
        const auto index = internal::distance(cbegin(), first);
        for (size_type i = index; i < _size - count; ++i) {
            _data[i] = std::move(_data[i + count]);
        }
        _size -= count;
        return begin() + index;
    }

    /**
     * Swaps the contents of this vector with another vector.
     * 
     * @param[in] other The other vector to swap with.
     */
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(fixed_vector& other) IPADDRESS_NOEXCEPT {
        const auto tmp_size = _size;
        _size = other._size;
        other._size = tmp_size;
        auto size = _size < other._size ? other._size : _size;
        for (size_type i = 0; i < size; ++i) {
            const auto tmp_value = _data[i];
            _data[i] = other._data[i];
            other._data[i] = tmp_value;
        }
    }

private:
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator insert_n(const_iterator pos, size_type n, const_reference value) IPADDRESS_NOEXCEPT {
        assert(pos >= begin() && pos <= end());
        if (pos == end()) {
            assert(_size + n <= max_size());
            auto result = end();
            for (size_type i = 0; i < n; ++i) {
                _data[_size++] = value;
            }
            return result;
        } else {
            const auto index = internal::distance(cbegin(), pos);
            assert(_size + n <= max_size());
            for (auto i = difference_type(_size) - 1; i >= index; --i) {
                _data[i + difference_type(n)] = _data[i];
            }
            for (auto i = 0; i < difference_type(n); ++i) {
                _data[index + i] = value;
            }
            _size += n;
            return begin() + index;
        }
    }

    template <typename It>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator insert_iterators(const_iterator pos, It first, It last) IPADDRESS_NOEXCEPT {
        assert(pos >= begin() && pos <= end() && first <= last);
        if (pos == end()) {
            auto result = end();
            for (auto it = first; it != last; ++it) {
                assert(_size < max_size());
                _data[_size++] = *it;
            }
            return result;
        } else {
            const auto count = internal::distance(first, last);
            const auto index = internal::distance(cbegin(), pos);
            assert(_size + count <= max_size());
            for (auto i = difference_type(_size) - 1; i >= index; --i) {
                _data[i + count] = _data[i];
            }
            for (auto i = 0; i < count; ++i) {
                _data[index + i] = *first++;
            }
            _size += count;
            return begin() + index;
        }
    }

    value_type _data[N]{};
    size_type _size{};
}; // fixed_vector

IPADDRESS_EXPORT template <typename T>
class fixed_vector<T, 0> {
public:
    using value_type             = T;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using iterator               = fixed_vector_iterator<value_type>;
    using const_iterator         = fixed_vector_iterator<const value_type>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector() IPADDRESS_NOEXCEPT = default;

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector(size_type) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot be constructed with a size.");
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector(size_type, const_reference) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot be constructed with a size and value.");
    }

    template <class It>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector(It, It) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot be constructed with iterators.");
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE fixed_vector(std::initializer_list<value_type>) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot be constructed with an initializer list.");
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void assign(size_type, const_reference) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot be assigned a size and value.");
    }

    template <class It>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void assign(It, It) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot be assigned iterators.");
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void assign(std::initializer_list<value_type>) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot be assigned an initializer list.");
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference at(size_type) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot access elements.");
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference at(size_type) const IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot access elements.");
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference operator[](size_type) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot access elements.");
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference operator[](size_type) const IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot access elements.");
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference front() IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot access elements.");
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference front() const IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot access elements.");
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference back() IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot access elements.");
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reference back() const IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot access elements.");
        return *data();
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer data() IPADDRESS_NOEXCEPT {
        return nullptr;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_pointer data() const IPADDRESS_NOEXCEPT {
        return nullptr;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator begin() IPADDRESS_NOEXCEPT {
        return iterator(nullptr);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator begin() const IPADDRESS_NOEXCEPT {
        return const_iterator(nullptr);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cbegin() const IPADDRESS_NOEXCEPT {
        return const_iterator(nullptr);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reverse_iterator rbegin() IPADDRESS_NOEXCEPT {
        return reverse_iterator(end());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator rbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(end());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator crbegin() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cend());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator end() IPADDRESS_NOEXCEPT {
        return iterator(nullptr);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator end() const IPADDRESS_NOEXCEPT {
        return const_iterator(nullptr);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_iterator cend() const IPADDRESS_NOEXCEPT {
        return const_iterator(nullptr);
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reverse_iterator rend() IPADDRESS_NOEXCEPT {
        return reverse_iterator(begin());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator rend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(begin());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE const_reverse_iterator crend() const IPADDRESS_NOEXCEPT {
        return const_reverse_iterator(cbegin());
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE bool empty() const IPADDRESS_NOEXCEPT {
        return true;
    }

    IPADDRESS_NODISCARD IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_type size() const IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_type max_size() IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_NODISCARD static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE size_type capacity() IPADDRESS_NOEXCEPT {
        return 0;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void resize(size_type) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot be resized.");
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void resize(size_type, const_reference) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot be resized.");
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void reserve(size_type) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot reserve memory.");
    }

    static IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void shrink_to_fit() IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot shrink to fit.");
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator insert(const_iterator, const_reference) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot insert elements.");
        return nullptr;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator insert(const_iterator, value_type&&) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot insert elements.");
        return nullptr;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator insert(const_iterator, size_type, const_reference) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot insert elements.");
        return nullptr;
    }

    template <typename It>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator insert(const_iterator, It, It) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot insert elements.");
        return nullptr;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator insert(const_iterator, std::initializer_list<value_type>) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot insert elements.");
        return nullptr;
    }

    template <typename... Args>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator emplace(const_iterator, Args&&...) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot emplace elements.");
        return nullptr;
    }

    template <typename... Args>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference emplace_back(Args&&...) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot emplace elements.");
        return *data();
    }

    template <typename... Args>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer try_emplace_back(Args&&...) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot emplace elements.");
        return nullptr;
    }

    template <typename... Args>
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference unchecked_emplace_back(Args&&...) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot emplace elements.");
        return *data();
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference push_back(const_reference) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot push back elements.");
        return *data();
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference push_back(value_type&&) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot push back elements.");
        return *data();
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer try_push_back(const_reference) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot push back elements.");
        return nullptr;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE pointer try_push_back(value_type&&) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot push back elements.");
        return nullptr;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference unchecked_push_back(const_reference) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot push back elements.");
        return *data();
    }
    
    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE reference unchecked_emplace_back(value_type&&) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot emplace elements.");
        return *data();
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void pop_back() IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot pop back elements.");
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void clear() IPADDRESS_NOEXCEPT {
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator erase(const_iterator) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot erase elements.");
        return nullptr;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE iterator erase(const_iterator, const_iterator) IPADDRESS_NOEXCEPT {
        assert(!"fixed_vector<T, 0> cannot erase elements.");
        return nullptr;
    }

    IPADDRESS_CONSTEXPR IPADDRESS_FORCE_INLINE void swap(fixed_vector&) IPADDRESS_NOEXCEPT {
    }
}; // fixed_vector<T, 0>

} // namespace IPADDRESS_NAMESPACE

#endif // IPADDRESS_FIXED_VECTOR_HPP
