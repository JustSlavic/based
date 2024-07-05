#ifndef BASED__COMPILE_TIME_HPP
#define BASED__COMPILE_TIME_HPP

#include "base.h"


namespace type {


template <typename T, T V>
struct constant
{
    typedef T value;
    typedef constant<T, V> type;
};

using true_type  = constant<bool, true>;
using false_type = constant<bool, false>;

template <typename T, typename U> struct is_same       : false_type {};
template <typename T>             struct is_same<T, T> : true_type  {};

template <typename T> struct remove_reference     { typedef T type; };
template <typename T> struct remove_reference<T&> { typedef T type; };
template <typename T> struct remove_reference<T&&>{ typedef T type; };

template <typename T>
typename remove_reference<T>::type&& move(T&& x) noexcept
{
    return static_cast<typename remove_reference<T>::type&&>(x);
}

} // namespace type


#endif // BASED__COMPILE_TIME_HPP
