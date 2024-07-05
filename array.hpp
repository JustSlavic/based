#ifndef BASED__ARRAY_HPP
#define BASED__ARRAY_HPP

#include "base.h"
#include "compile_time.hpp"
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

    iterator insert(const_iterator position, const value_type& value);
    iterator insert(const_iterator position, size_type n, const value_type& value);
    iterator insert(const_iterator position, value_type&& value);
    // iterator insert(const_iterator position, std::initializer_list<value_type> ilist);

    // iterator erase_first(const T& value);
    // iterator erase_first_unsorted(const T& value);
    // reverse_iterator erase_last(const T& value);
    // reverse_iterator erase_last_unsorted(const T& value);

    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);
    iterator erase_unsorted(const_iterator position);

    // reverse_iterator erase(const_reverse_iterator position);
    // reverse_iterator erase(const_reverse_iterator first, const_reverse_iterator last);
    // reverse_iterator erase_unsorted(const_reverse_iterator position);

    // void clear() noexcept;

    // T const *data() const { return data_; }
    // T *data() { return data_; }

    // usize size() const { return size_; }
    // usize capacity() const { return capacity_; }

    // void clear() { size_ = 0; }
    // bool is_empty() const { return (size_ == 0); }
    // void resize(usize new_size) { size_ = new_size; }

    // iterator begin() { return data_; }
    // const_iterator begin() const { return data_; }
    // const_iterator cbegin() const { return data_; }

    // iterator end() { return data_ + size_; }
    // const_iterator end() const { return data_ + size_; }
    // const_iterator cend() const { return data_ + size_; }

    // static array_base<T> from(byte *data, usize size)
    // {
    //     array_base<T> result;

    //     auto padding = get_padding(data, alignof(T));

    //     result.data_ = (T *) (data + padding);
    //     result.size_ = 0;
    //     result.capacity_ = (size - padding) / sizeof(T);
    //     return result;
    // }

    // static array_base<T> from(memory_buffer buffer)
    // {
    //     return array_base<T>::from(buffer.data, buffer.size);
    // }

    // T& operator[] (usize index)
    // {
    //     ASSERT(index < size_);
    //     return data_[index];
    // }

    // T const& operator[] (usize index) const
    // {
    //     ASSERT(index < size_);
    //     return data_[index];
    // }

    // T *push_back()
    // {
    //     T* result = 0;
    //     if (size_ < capacity_)
    //     {
    //         usize index = size_++;
    //         result = data_ + index;
    //     }
    //     return result;
    // }

    // void push_back(T t)
    // {
    //     if (size_ < capacity_)
    //     {
    //         data_[size_++] = t;
    //     }
    // }

    // void push_front(T t)
    // {
    //     if (size() < capacity())
    //     {
    //         for (int i = size(); i > 0; i--)
    //         {
    //             data_[i] = data_[i - 1];
    //         }
    //         data_[0] = t;
    //         size() += 1;
    //     }
    // }

    // void insert(usize index, T t)
    // {
    //     if (size() <= index && index < capacity())
    //     {
    //         push_back(t);
    //     }
    //     else if (index < size() && size() < capacity())
    //     {

    //     }
    // }
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
    // @todo
}

template <typename T, typename Allocator>
FORCE_INLINE void
array_base<T, Allocator>::resize(size_type n)
{
    // @todo
}

/*
    void reserve(size_type n);
    void shrink_to_fit();
*/

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
    pointer p = ::new((void *) (m_allocator.data() + m_size++)) value_type(value);
    return *p;
}

template <typename T, typename Allocator>
FORCE_INLINE void array_base<T, Allocator>::push_back(value_type&& value) noexcept
{
    if (size() >= m_allocator.capacity())
    {
        bool grown = m_allocator.grow();
        ASSERT_MSG(grown, "array_base::push_back() could not grow internal allocator");
    }
    pointer p = ::new((void *) (m_allocator.data() + m_size++)) value_type(type::move(value));
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
array_base<T, Allocator>::insert(const_iterator position, const value_type& value)
{
    ASSERT_MSG(size() == capacity(), "array_base::insert() at full array");
    // @todo
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::iterator
array_base<T, Allocator>::insert(const_iterator position, size_type n, const value_type& value)
{
    ASSERT_MSG(size() == capacity(), "array_base::insert() at full array");
    // @todo
}

template <typename T, typename Allocator>
FORCE_INLINE typename array_base<T, Allocator>::iterator
array_base<T, Allocator>::insert(const_iterator position, value_type&& value)
{
    ASSERT_MSG(size() == capacity(), "array_base::insert() at full array");
    // @todo
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
    ASSERT_MSG(begin() <= position && position < end(), "vector_base::erase(position) with invalid position");

    auto result = (iterator) position;

    position->~value_type(); // Call destructor in case it's not POD
    if ((position + 1) < end())
    {
        *((iterator)position) = type::move(*(end() - 1));
    }
    m_size -= 1;

    return result;
}




















// template <typename T, usize Capacity>
// struct static_array
// {
//     typedef T *iterator;
//     typedef T const *const_iterator;

//     T data_[Capacity];
//     usize size_;

//     T const *data() const { return data_; }
//     T *data() { return data_; }

//     usize size() const { return size_; }
//     usize capacity() const { return Capacity; }

//     void clear() { size_ = 0; }
//     bool empty() { return (size_ == 0); }

//     void resize(usize n) { size_ = n; memset(data_, 0, sizeof(T) * n); }
//     void resize(usize n, T def_val) { size_ = n; for (int i = 0; i < n; i++) data_[i] = def_val; }

//     T& operator[] (usize index)
//     {
//         ASSERT(index < size_);
//         return data_[index];
//     }

//     T const& operator[] (usize index) const
//     {
//         ASSERT(index < size_);
//         return data_[index];
//     }

    // T *push_back()
    // {
    //     T* result = 0;
    //     if (size_ < Capacity)
    //     {
    //         usize index = size_++;
    //         result = data_ + index;
    //     }
    //     return result;
    // }

    // void push_back(T t)
    // {
    //     if (size_ < Capacity)
    //     {
    //         usize index = size_++;
    //         data_[index] = t;
    //     }
    // }

    // void erase(T t)
    // {
    //     int i = 0;
    //     for (; i < size_; i++)
    //     {
    //         if (data_[i] == t) break;
    //     }
    //     for (; i < (size_ - 1); i++)
    //     {
    //         data_[i] = data_[i + 1];
//         }
//         size_ -= 1;
//     }

//     void erase_not_sorted(usize index)
//     {
//         if (index < size_)
//         {
//             data_[index] = data_[size_ - 1];
//             size_ -= 1;
//         }
//     }
// };


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
};

// template <typename T, size_type N>
// struct array_memory_dynamic_non_resizable
// {

// };

// struct array_memory_allocator
// {

// };

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


} // namespace internal


template <typename T, typename internal::size_type N>
using static_array = internal::array_base<T, internal::array_memory_static<T, N>>;



#endif // BASED__ARRAY_HPP
