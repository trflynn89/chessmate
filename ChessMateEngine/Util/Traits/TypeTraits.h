#pragma once

#include <functional>
#include <iostream>
#include <type_traits>

/**
 * Custom type traits not provided by the STL.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version May 29, 2016
 */
namespace Util {

/**
 * Utility to get the type member for type T.
 */
template <typename T>
using invoke = typename T::type;

/**
 * Wrapper to invoke a std::conditional.
 */
template <typename If, typename Then, typename Else>
using conditional = invoke<std::conditional<If::value, Then, Else>>;

/**
 * Wrapper around a set of conditionals to check if all conditions are true.
 */
template <typename ...T>
struct all : std::true_type { };

/**
 * Wrapper around a set of conditionals to check if all conditions are true.
 */
template <typename Head, typename ...Tail>
struct all<Head, Tail...> : conditional<Head, all<Tail...>, std::false_type> { };

/**
 * Wrapper around std::enable_if for multiple conditions. For example:
 *
 *      template <typename T, enable_if_all<std::is_class<T>, std::is_empty<T>>...>
 *      void func(const T &) { }
 */
template <typename ...Conditions>
using enable_if_all = invoke<std::enable_if<all<Conditions...>::value>>;

/**
 * Wrapper around std::integral_constant for constant boolean values.
 */
template <bool B>
using bool_constant = std::integral_constant<bool, B>;

/**
 * Define SFINAE tests for whether a function is declared for a type.
 *
 * For example, to test if a class of type T declares a function called Foo:
 *
 *      DECL_TESTS(foo, T, std::declval<const T &>().Foo());
 *
 * This will define these bool_constant wrappers around std::enable_if:
 *
 *      if_foo::enabled<T>
 *      if_foo::disabled<T>
 *
 * And may be used as SFINAE tests to, e.g., define a function depending on
 * whether or not Foo() was declared:
 *
 *      template <typename T, enable_if_all<if_foo::enabled<T>>...>
 *      void foo_wrapper(const T &arg) { arg.Foo(); }
 *
 *      template <typename T, enable_if_all<if_foo::disabled<T>>...>
 *      void foo_wrapper(const T &) { }
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
        template <typename Type> auto test_##label(Type *) -> decltype(functor); \
        template <typename Type> auto test_##label(...) -> std::false_type; \
        \
        template <typename Type> \
        using is_undefined = std::is_same<decltype(test_##label<Type>(0)), std::false_type>; \
    } \
    \
    template <typename Type, typename S = bool> \
    using enabled = bool_constant<!is_undefined<Type>::value>; \
    \
    template <typename Type, typename S = bool> \
    using disabled = bool_constant<is_undefined<Type>::value>; \
}

/**
 * Tests for whether a given type is string-like.
 */
namespace if_string
{
    namespace
    {
        template <typename T>
        using is_string = bool_constant<
            std::is_same<char,         invoke<std::decay<T>>>::value ||
            std::is_same<char *,       invoke<std::decay<T>>>::value ||
            std::is_same<char const *, invoke<std::decay<T>>>::value ||
            std::is_same<std::string,  invoke<std::decay<T>>>::value>;
    }

    template <typename T, typename S = bool>
    using enabled = bool_constant<is_string<T>::value>;

    template <typename T, typename S = bool>
    using disabled = bool_constant<!is_string<T>::value>;
}

/**
 * Tests for whether a type defines operator<<.
 */
DECL_TESTS(ostream, T, std::declval<std::ostream &>() << std::declval<const T &>());

/**
 * Tests for whether a type is hashable with std::hash.
 */
DECL_TESTS(hash, T, std::declval<const std::hash<T> &>() (std::declval<const T &>()));

}
