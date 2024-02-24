#ifndef BASED__ARRAY_HPP
#define BASED__ARRAY_HPP


#include "base.h"
#include "memory_buffer.hpp"


template <typename T>
struct array
{
    T *data_;
    usize size_;
    usize capacity_;

    T const *data() const { return data_; }
    T *data() { return data_; }

    usize size() const { return size_; }
    usize capacity() const { return capacity_; }

    static array<T> from(memory_buffer buffer)
    {
        array<T> result;

        auto padding = get_padding(buffer.data, alignof(T));

        result.data_ = (T *) (buffer.data + padding);
        result.size_ = 0;
        result.capacity_ = (buffer.size - padding) / sizeof(T);
        return result;
    }

    void clear() { size_ = 0; }
    bool is_empty() const { return (size_ == 0); }
    void resize(usize new_size) { size_ = new_size; }

    T& operator[] (usize index)
    {
        ASSERT(index < size);
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
};


template <typename T, usize Capacity>
struct static_array
{
    T data_[Capacity];
    usize size_;

    T const *data() const { return data_; }
    T *data() { return data_; }

    usize size() const { return size_; }
    usize capacity() const { return Capacity; }

    void clear() { size_ = 0; }
    bool empty() { return (size_ == 0); }

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
            data_[index] = data_[size - 1];
            size_ -= 1;
        }
    }

    struct iterator
    {
        T *data;
        usize size;
        usize index;

        bool operator == (iterator const & other) { return (data == other.data) && (size == other.size) && (index == other.index); }
        bool operator != (iterator const & other) { return !(*this == other); }

        iterator& operator ++ () { index += 1; return *this; }
        iterator  operator ++ (int) { iterator old = *this; index += 1; return old; }

        iterator& operator -- () { index -= 1; return *this; }
        iterator  operator -- (int) { iterator old = *this; index -= 1; return old; }

        T operator * () const { return (index < size) ? *(data + index) : T{}; }
    };

    iterator begin() { iterator result = { data_, size_, 0 }; return result; }
    iterator end() { iterator result = { data_, size_, size_ }; return result; }
};



#endif // BASED__ARRAY_HPP
