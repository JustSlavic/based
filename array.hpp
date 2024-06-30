#ifndef BASED__ARRAY_HPP
#define BASED__ARRAY_HPP


#include "base.h"
#include "memory/buffer.hpp"


template <typename T>
struct array
{
    typedef T *iterator;
    typedef T const *const_iterator;

    T *data_;
    usize size_;
    usize capacity_;

    T const *data() const { return data_; }
    T *data() { return data_; }

    usize size() const { return size_; }
    usize capacity() const { return capacity_; }

    void clear() { size_ = 0; }
    bool is_empty() const { return (size_ == 0); }
    void resize(usize new_size) { size_ = new_size; }

    iterator begin() { return data_; }
    const_iterator begin() const { return data_; }
    const_iterator cbegin() const { return data_; }

    iterator end() { return data_ + size_; }
    const_iterator end() const { return data_ + size_; }
    const_iterator cend() const { return data_ + size_; }

    static array<T> from(byte *data, usize size)
    {
        array<T> result;

        auto padding = get_padding(data, alignof(T));

        result.data_ = (T *) (data + padding);
        result.size_ = 0;
        result.capacity_ = (size - padding) / sizeof(T);
        return result;
    }

    static array<T> from(memory_buffer buffer)
    {
        return array<T>::from(buffer.data, buffer.size);
    }

    T& operator[] (usize index)
    {
        ASSERT(index < size_);
        return data_[index];
    }

    T const& operator[] (usize index) const
    {
        ASSERT(index < size_);
        return data_[index];
    }

    T *push_back()
    {
        T* result = 0;
        if (size_ < capacity_)
        {
            usize index = size_++;
            result = data_ + index;
        }
        return result;
    }

    void push_back(T t)
    {
        if (size_ < capacity_)
        {
            data_[size_++] = t;
        }
    }

    void push_front(T t)
    {
        if (size_ < capacity_)
        {
            for (int i = size_; i > 0; i--)
            {
                data_[i] = data_[i - 1];
            }
            data_[0] = t;
            size_ += 1;
        }
    }
};


template <typename T, usize Capacity>
struct static_array
{
    typedef T *iterator;
    typedef T const *const_iterator;

    T data_[Capacity];
    usize size_;

    T const *data() const { return data_; }
    T *data() { return data_; }

    usize size() const { return size_; }
    usize capacity() const { return Capacity; }

    void clear() { size_ = 0; }
    bool empty() { return (size_ == 0); }

    void resize(usize n) { size_ = n; memset(data_, 0, sizeof(T) * n); }
    void resize(usize n, T def_val) { size_ = n; for (int i = 0; i < n; i++) data_[i] = def_val; }

    iterator begin() { return data_; }
    const_iterator begin() const { return data_; }
    const_iterator cbegin() const { return data_; }

    iterator end() { return data_ + size_; }
    const_iterator end() const { return data_ + size_; }
    const_iterator cend() const { return data_ + size_; }

    T& operator[] (usize index)
    {
        ASSERT(index < size_);
        return data_[index];
    }

    T const& operator[] (usize index) const
    {
        ASSERT(index < size_);
        return data_[index];
    }

    T *push_back()
    {
        T* result = 0;
        if (size_ < Capacity)
        {
            usize index = size_++;
            result = data_ + index;
        }
        return result;
    }

    void push_back(T t)
    {
        if (size_ < Capacity)
        {
            usize index = size_++;
            data_[index] = t;
        }
    }

    void erase_not_sorted(usize index)
    {
        if (index < size_)
        {
            data_[index] = data_[size_ - 1];
            size_ -= 1;
        }
    }
};



#endif // BASED__ARRAY_HPP
