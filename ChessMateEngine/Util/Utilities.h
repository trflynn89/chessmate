#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>

// Determine operating system
#if defined(_WIN32)
    #define BUILD_WINDOWS
#elif defined(__linux__)
    #define BUILD_LINUX
#else
    #error Unsupported operating system. Only Windows and Linux are supported.
#endif

// Define macros to treat numeric constants as 64-bit
#if defined(BUILD_WINDOWS)
    #define I64(n) (n##ll)
    #define U64(n) (n##ull)

    #include <BaseTsd.h>
    typedef SSIZE_T ssize_t;
#elif defined(BUILD_LINUX)
    #define I64(n) __INT64_C(n)
    #define U64(n) __UINT64_C(n)
#endif

// Typedefs for class name and smart pointers
#define DEFINE_CLASS_PTRS(classname)                    \
    class classname;                                    \
    typedef std::shared_ptr<classname> classname##Ptr;  \
    typedef std::unique_ptr<classname> classname##UPtr; \
    typedef std::weak_ptr<classname> classname##WPtr;

// Custom type traits
namespace Util
{
    template <typename T>
    using base_type =
        typename std::decay<
            typename std::remove_pointer<
                typename std::remove_all_extents<
                    T
                >::type
            >::type
        >::type;

    template <typename T>
    using is_string = std::integral_constant<bool,
        std::is_same<char,        base_type<T>>::value ||
        std::is_same<std::string, base_type<T>>::value>;

    template <typename T, typename S = void>
    using enable_if_str = std::enable_if<is_string<T>::value, S>;

    template <typename T, typename S = void>
    using disable_if_str = std::enable_if<!is_string<T>::value, S>;
}
