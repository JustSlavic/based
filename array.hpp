#ifndef BASED__ARRAY_HPP
#define BASED__ARRAY_HPP

#include "base.h"
#include "compile_time.hpp"
#include "memory/allocator.hpp"
#include <new>


namespace internal {


typedef usize size_type;


template <typename T, typename Allocator>
struct array_base
{
    typedef array_base<T, Allocator> this_type;
    typedef T                        value_type;
    typedef T *                      pointer;
    typedef T const *                const_pointer;
    typedef T &                      reference;
    typedef T const&                 const_reference;
    typedef T *                      iterator;
    typedef T const *                const_iterator;

    Allocator m_allocator;
    size_type m_size;

    iterator       begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator       end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    bool      empty() const noexcept;
    size_type size() const noexcept;
    size_type capacity() const noexcept;

    void resize(size_type n, const_reference value);
    void resize(size_type n);
/*
    void reserve(size_type n);
    void shrink_to_fit();
*/

    pointer       data() noexcept;
    const_pointer data() const noexcept;

    reference       operator[](size_type n) noexcept;
    const_reference operator[](size_type n) const noexcept;

    reference       front() noexcept;
    const_reference front() const noexcept;

    reference       back() noexcept;
    const_reference back() const noexcept;

    reference push_back() noexcept;
    reference push_back_uninitialized() noexcept;
    void      push_back(const_reference value) noexcept;
    void      push_back(value_type&& value) noexcept;
    void      pop_back() noexcept;

    iterator insert(const_iterator position, const_reference value);
    iterator insert(const_iterator position, value_type&& value);
    iterator insert(const_iterator position, size_type n, const_reference value);
    // iterator insert(const_iterator position, std::initializer_list<value_type> ilist);

    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);
    iterator erase_unsorted(const_iterator position);

    iterator erase_first(const_reference value);
    iterator erase_first_unsorted(const_reference value);

    void clear() noexcept;
    void deallocate() noexcept;
};


template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::iterator
array_base<T, Allocator>::begin() noexcept
{
    return m_allocator.data();
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::const_iterator
array_base<T, Allocator>::begin() const noexcept
{
    return m_allocator.data();
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::const_iterator
array_base<T, Allocator>::cbegin() const noexcept
{
    return m_allocator.data();
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::iterator
array_base<T, Allocator>::end() noexcept
{
    return begin() + m_size;
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::const_iterator
array_base<T, Allocator>::end() const noexcept
{
    return begin() + m_size;
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::const_iterator
array_base<T, Allocator>::cend() const noexcept
{
    return begin() + m_size;
}

template <typename T, typename Allocator>
FORCE_INLINE bool
array_base<T, Allocator>::empty() const noexcept
{
    return m_size == 0;
}

template <typename T, typename Allocator>
FORCE_INLINE size_type
array_base<T, Allocator>::size() const noexcept
{
    return m_size;
}

template <typename T, typename Allocator>
FORCE_INLINE size_type
array_base<T, Allocator>::capacity() const noexcept
{
    return m_allocator.capacity();
}

template <typename T, typename Allocator>
FORCE_INLINE void
array_base<T, Allocator>::resize(size_type n, const_reference value)
{
    if (n < m_size)
        for (uint32 i = 0; i < (m_size - n); i++)
            (m_allocator.data() + n + i)->~value_type();

    if (n > capacity())
    {
        bool grown = m_allocator.grow(n);
        ASSERT_MSG(grown, "array_base::resize() could not grow internal allocator");
        if (!grown) return;
    }

    if (n > m_size)
        for (size_type i = m_size; i < n; i++)
            *(m_allocator.data() + i) = value;

    m_size = n;
}

template <typename T, typename Allocator>
FORCE_INLINE void
array_base<T, Allocator>::resize(size_type n)
{
    if (n < size())
        for (uint32 i = 0; i < (size() - n); i++)
            (data() + n + i)->~value_type();

    if (n > capacity())
    {
        bool grown = m_allocator.grow(n);
        ASSERT_MSG(grown, "array_base::resize() could not grow internal allocator");
        if (!grown) return;
    }
    m_size = n;
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::pointer
array_base<T, Allocator>::data() noexcept
{
    return m_allocator.data();
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::const_pointer
array_base<T, Allocator>::data() const noexcept
{
    return m_allocator.data();
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::reference
array_base<T, Allocator>::operator[](size_type n) noexcept
{
    ASSERT_MSG(n < size(), "array_base<T, Allocator>::operator[] out of range error");
    return *(m_allocator.data() + n);
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::const_reference
array_base<T, Allocator>::operator[](size_type n) const noexcept
{
    ASSERT_MSG(n < size(), "array_base::operator[] out of range error");
    return *(m_allocator.data() + n);
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::reference
array_base<T, Allocator>::front() noexcept
{
    ASSERT_MSG(empty(), "array_base::front() on empty array");
    return *m_allocator.data();
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::const_reference
array_base<T, Allocator>::front() const noexcept
{
    ASSERT_MSG(empty(), "array_base::front() on empty array");
    return *m_allocator.data();
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::reference
array_base<T, Allocator>::back() noexcept
{
    ASSERT_MSG(empty(), "array_base::back() on empty array");
    return *(m_allocator.data() + size() - 1);
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::const_reference
array_base<T, Allocator>::back() const noexcept
{
    ASSERT_MSG(empty(), "array_base::back() on empty array");
    return *(m_allocator.data() + size() - 1);
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::reference
array_base<T, Allocator>::push_back() noexcept
{
    if (size() >= m_allocator.capacity())
    {
        bool grown = m_allocator.grow();
        ASSERT_MSG(grown, "array_base::push_back() could not grow internal allocator");
    }
    pointer p = ::new((void *) (m_allocator.data() + m_size++)) value_type();
    return *p;
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::reference
array_base<T, Allocator>::push_back_uninitialized() noexcept
{
    if (size() >= m_allocator.capacity())
    {
        bool grown = m_allocator.grow();
        ASSERT_MSG(grown, "array_base::push_back() could not grow internal allocator");
    }
    return *(m_allocator.data() + m_size++);
}

template <typename T, typename Allocator>
FORCE_INLINE void array_base<T, Allocator>::push_back(const_reference value) noexcept
{
    if (size() >= m_allocator.capacity())
    {
        bool grown = m_allocator.grow();
        ASSERT_MSG(grown, "array_base::push_back() could not grow internal allocator");
    }
    ::new((void *) (m_allocator.data() + m_size++)) value_type(value);
}

template <typename T, typename Allocator>
FORCE_INLINE void array_base<T, Allocator>::push_back(value_type&& value) noexcept
{
    if (size() >= m_allocator.capacity())
    {
        bool grown = m_allocator.grow();
        ASSERT_MSG(grown, "array_base::push_back() could not grow internal allocator");
    }
    ::new((void *) (m_allocator.data() + m_size++)) value_type(type::move(value));
}

template <typename T, typename Allocator>
FORCE_INLINE void array_base<T, Allocator>::pop_back() noexcept
{
    ASSERT_MSG(size() > 0, "array_base::pop_back() on empty array");
    m_size--;
    (m_allocator.data() + size())->~value_type(); // Call destructor in case it's not POD
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::iterator
array_base<T, Allocator>::insert(const_iterator position, const_reference value)
{
    ASSERT_MSG(begin() <= position && position < end(), "array_base::insert(position) with invalid position");

    size_type n = position - begin();
    if (size() == capacity()) m_allocator.grow();
    for (int i = size(); i > n; i--)
        *(m_allocator.data() + i) = type::move(*(m_allocator.data() + i - 1));
    pointer p = ::new(*(m_allocator.data() + n)) value_type(value);
    m_size++;
    return p;
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::iterator
array_base<T, Allocator>::insert(const_iterator position, value_type&& value)
{
    ASSERT_MSG(begin() <= position && position < end(), "array_base::insert(position) with invalid position");

    size_type n = position - begin();
    if (size() == capacity()) m_allocator.grow();
    for (size_type i = size(); i > n; i--)
        *(m_allocator.data() + i) = type::move(*(m_allocator.data() + i - 1));
    *(m_allocator.data() + n) = type::move(value);
    m_size++;
    return begin() + n;
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::iterator
array_base<T, Allocator>::insert(const_iterator position, size_type count, const_reference value)
{
    ASSERT_MSG(begin() <= position && position < end(), "array_base::insert(position) with invalid position");

    size_type n = position - begin();
    if (size() + count >= capacity()) m_allocator.grow();
    for (size_type i = size() + count; i > n + count; i--)
        *(m_allocator.data() + i - 1) = type::move(*(m_allocator.data() + i - count - 1));
    for (size_type i = n; i < n + count; i++)
        ::new(m_allocator.data() + i) value_type(value);
    m_size += count;
    return begin() + n;
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::iterator
array_base<T, Allocator>::erase(const_iterator position)
{
    ASSERT_MSG(begin() <= position && position < end(), "vector_base::erase(position) with invalid position");

    auto result = (iterator) position;

    position->~value_type(); // Call destructor in case it's not POD
    while ((position + 1) < end())
    {
        *((iterator)position) = type::move(*(position + 1));
        position += 1;
    }
    m_size -= 1;

    return result;
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::iterator
array_base<T, Allocator>::erase(const_iterator first, const_iterator last)
{
    ASSERT_MSG(begin() <= first && first < end(), "vector_base::erase(position) with invalid first position");
    ASSERT_MSG(begin() <= last && last < end(), "vector_base::erase(position) with invalid last position");
    ASSERT_MSG(first < last, "vector_base::erase(position) with invalid first position greater than last");

    auto n = (size_type) (last - first);
    auto result = (iterator) first;
    for (auto it = first; it != last; it++)
        it->~value_type(); // Call destructors for all elements that are erasing in case they're not POD
    auto write_it = (iterator) first;
    auto read_it = last;
    while (read_it < end())
        *write_it++ = type::move(*read_it++);
    m_size -= n;
    return result;
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::iterator
array_base<T, Allocator>::erase_unsorted(const_iterator position)
{
    ASSERT_MSG(begin() <= position && position < end(), "vector_base::erase_unsorted(position) with invalid position");

    auto result = (iterator) position;

    position->~value_type(); // Call destructor in case it's not POD
    if ((position + 1) < end())
    {
        *((iterator)position) = type::move(*(end() - 1));
    }
    m_size -= 1;

    return result;
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::iterator
array_base<T, Allocator>::erase_first(const_reference value)
{
    iterator it = begin();
    for (; it != end(); it++)
        if (*it == value) break;

    return erase(it);
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::iterator
array_base<T, Allocator>::erase_first_unsorted(const_reference value)
{
    iterator it = begin();
    for (; it != end(); it++)
        if (*it == value) break;

    return erase_unsorted(it);
}

template <typename T, typename Allocator>
FORCE_INLINE void array_base<T, Allocator>::clear() noexcept
{
    for (size_type i = 0; i < m_size; i++)
        (m_allocator.data() + i)->~value_type();
    m_size = 0;
}

template <typename T, typename Allocator>
FORCE_INLINE void array_base<T, Allocator>::deallocate() noexcept
{
    clear();
    m_allocator.deallocate();
}


template <typename T, size_type N>
struct array_memory_static
{
    typedef T         value_type;
    typedef T *       pointer;
    typedef T const * const_pointer;

    value_type m_data[N];

    pointer       data() noexcept;
    const_pointer data() const noexcept;
    size_type capacity() const noexcept;
    bool          grow() noexcept;
    bool          grow(size_type at_least) noexcept;
    void    deallocate() noexcept;
};

template <typename T>
struct array_memory_dynamic_non_resizeable
{
    typedef T         value_type;
    typedef T *       pointer;
    typedef T const * const_pointer;

    value_type *m_data;
    size_type   m_capacity;

    pointer       data() noexcept;
    const_pointer data() const noexcept;
    size_type capacity() const noexcept;
    bool          grow() noexcept;
    bool          grow(size_type at_least) noexcept;
    void    deallocate() noexcept;
};

template <typename T>
struct array_memory_dynamic_resizeable
{
    typedef T         value_type;
    typedef T *       pointer;
    typedef T const * const_pointer;

    value_type *m_data;
    size_type   m_size_in_bytes;
    size_type   m_capacity;
    memory_allocator *m_allocator;

    pointer       data() noexcept;
    const_pointer data() const noexcept;
    size_type capacity() const noexcept;
    bool          grow() noexcept;
    bool          grow(size_type at_least) noexcept;
    void    deallocate() noexcept;
};

template <typename T, size_type N>
FORCE_INLINE typename array_memory_static<T, N>::pointer
array_memory_static<T, N>::data() noexcept
{
    return m_data;
}

template <typename T, size_type N>
FORCE_INLINE typename array_memory_static<T, N>::const_pointer
array_memory_static<T, N>::data() const noexcept
{
    return m_data;
}

template <typename T, size_type N>
FORCE_INLINE size_type
array_memory_static<T, N>::capacity() const noexcept
{
    return N;
}

template <typename T, size_type N>
FORCE_INLINE bool
array_memory_static<T, N>::grow() noexcept
{
    ASSERT_FAIL("Could not grow static array!");
}

template <typename T, size_type N>
FORCE_INLINE bool
array_memory_static<T, N>::grow(size_type at_least) noexcept
{
    ASSERT_FAIL("Could not grow static array!");
}

template <typename T, size_type N>
FORCE_INLINE void
array_memory_static<T, N>::deallocate() noexcept
{
    // Do nothing.
}

template <typename T>
FORCE_INLINE typename array_memory_dynamic_non_resizeable<T>::pointer
array_memory_dynamic_non_resizeable<T>::data() noexcept
{
    return m_data;
}

template <typename T>
FORCE_INLINE typename array_memory_dynamic_non_resizeable<T>::const_pointer
array_memory_dynamic_non_resizeable<T>::data() const noexcept
{
    return m_data;
}

template <typename T>
FORCE_INLINE size_type
array_memory_dynamic_non_resizeable<T>::capacity() const noexcept
{
    return m_capacity;
}

template <typename T>
FORCE_INLINE bool
array_memory_dynamic_non_resizeable<T>::grow() noexcept
{
    ASSERT_FAIL("Could not grow non_resizeable policy array!");
}

template <typename T>
FORCE_INLINE bool
array_memory_dynamic_non_resizeable<T>::grow(size_type at_least) noexcept
{
    ASSERT_FAIL("Could not grow non_resizeable policy array!");
}

template <typename T>
FORCE_INLINE void
array_memory_dynamic_non_resizeable<T>::deallocate() noexcept
{
}

} // namespace internal


template <typename T, typename internal::size_type N>
using static_array = internal::array_base<T, internal::array_memory_static<T, N>>;

template <typename T>
using array = internal::array_base<T, internal::array_memory_dynamic_non_resizeable<T>>;

template <typename T>
using dynamic_array = internal::array_base<T, internal::array_memory_dynamic_resizeable<T>>;

template <typename internal::size_type N, typename T, typename... U>
static_array<T, N> make_static_array(T t, U... ts)
{
    static_array<T, N> result = {{ t, ts... }};
    result.m_size = 1 + sizeof...(ts);
    return result;
}

template <typename internal::size_type N, typename T>
static_array<T, N> make_static_array(T t)
{
    static_array<T, N> result = {{ t }};
    result.m_size = 1;
    return result;
}

template <typename internal::size_type N, typename T>
static_array<T, N> make_static_array()
{
    static_array<T, N> result = {};
    return result;
}

template <typename T, typename... U>
array<T> make_array(memory_buffer memory, T t, U... ts)
{
    ASSERT_MSG((memory.size - get_padding(memory.data, alignof(T))) / sizeof(T),
        "make_array() - provided memory is too small to init array");

    array<T> result = {};
    result.m_allocator.m_data = (T *) align_pointer(memory.data, alignof(T));
    result.m_allocator.m_capacity = (memory.size - get_padding(memory.data, alignof(T))) / sizeof(T);

    result.push_back(t);
    int dummy[sizeof...(U)] = {(result.push_back(ts), 0)...};
    UNUSED(dummy);

    result.m_size = 1 + sizeof...(ts);
    return result;
}

template <typename T>
array<T> make_array(memory_buffer memory, T t)
{
    array<T> result = {};
    result.m_allocator.m_data = (T *) align_pointer(memory.data, alignof(T));
    result.m_allocator.m_capacity = (memory.size - get_padding(memory.data, alignof(T))) / sizeof(T);

    *(result.data()) = t;
    result.m_size += 1;
    return result;
}

template <typename T>
array<T> make_array(memory_buffer memory)
{
    array<T> result = {};
    result.m_allocator.m_data = (T *) align_pointer(memory.data, alignof(T));
    result.m_allocator.m_capacity = (memory.size - get_padding(memory.data, alignof(T))) / sizeof(T);
    return result;
}

#endif // BASED__ARRAY_HPP
