#ifndef MYSTL_HPP
#define MYSTL_HPP
#include <type_traits>
namespace my {

    template<class T> 
    struct remove_reference
    {
        typedef T type;
    };

    template<class T>
    struct remove_reference<T&>
    {
        typedef T type;
    };

    template<class T>
    struct remove_reference<T&&>
    {
        typedef T type;
    };

    template<typename T>
    typename remove_reference<T>::type&& move(T&& t) {
        return static_cast<typename remove_reference<T>::type&&>(t);
    }

    template<class T>
    T&& forward(typename remove_reference<T>::type& a)
    {
        return static_cast<T&&>(a);
    }

    template<class T>
    T&& forward(typename remove_reference<T>::type&& a)
    {
        static_assert(!std::is_lvalue_reference<T>::value,
	  "my::forward must not be used to convert an rvalue to an lvalue");
        return static_cast<T&&>(a);
    }
}
#endif /* MYSTL_HPP */
