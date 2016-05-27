#pragma once

#include <iostream>
#include <type_traits>

/**
 * Custom type traits not provided by the STL.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version May 26, 2016
 */
namespace Util {

/**
 * Define SFINAE tests for whether a function is declared for a type.
 *
 * For example, to test if a class of type T declares a function called Foo:
 *
 *      DECL_TESTS(foo, T, std::declval<const T &>().Foo());
 *
 * This will define these wrappers around std::enable_if:
 *
 *      Util::if_foo::enabled<T>
 *      Util::if_foo::disabled<T>
 *
 * And may be used as SFINAE tests to, e.g., define a function depending on
 * whether or not Foo() was declared:
 *
 *      template <typename T, Util::if_foo::enabled<T> = 0>
 *      void foo_wrapper() { T.Foo(); }
 *
 *      template <typename T, Util::if_foo::disabled<T> = 0>
 *      void foo_wrapper() { }
 *
 * @param label The name to give the test.
 * @param Type The type to be tested.
 * @param functor The function to test for.
 */
#define DECL_TESTS(label, Type, functor) \
namespace if_##label \
{ \
    namespace \
    { \
        template <typename Type> auto test(Type *) -> decltype(functor); \
        template <typename Type> auto test(...) -> std::false_type; \
        \
        template <typename Type> \
        using is_defined = std::integral_constant<bool, \
            !std::is_same<decltype(test<Type>(0)), std::false_type>::value \
        >; \
    } \
    \
    template <typename Type, typename S = int> \
    using enabled = typename std::enable_if<is_defined<Type>::value, S>::type; \
    \
    template <typename Type, typename S = int> \
    using disabled = typename std::enable_if<!is_defined<Type>::value, S>::type; \
}

/**
 * Tests for whether a given type is string-like.
 */
namespace if_string
{
    namespace
    {
        template <typename T>
        using is_string = std::integral_constant<bool,
            std::is_same<char,         typename std::decay<T>::type>::value ||
            std::is_same<char *,       typename std::decay<T>::type>::value ||
            std::is_same<char const *, typename std::decay<T>::type>::value ||
            std::is_same<std::string,  typename std::decay<T>::type>::value>;
    }

    template <typename T, typename S = void>
    using enabled = std::enable_if<is_string<T>::value, S>;

    template <typename T, typename S = void>
    using disabled = std::enable_if<!is_string<T>::value, S>;
}

/**
 * Tests for whether type defines operator<<.
 */
DECL_TESTS(ostream, T, std::declval<std::ostream &>() << std::declval<const T &>());

}
