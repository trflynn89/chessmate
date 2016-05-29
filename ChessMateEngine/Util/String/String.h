#pragma once

#include <sstream>
#include <string>
#include <vector>

#include <Util/Utilities.h>
#include <Util/Random/RandomDevice.h>
#include <Util/Traits/TypeTraits.h>

namespace Util {

/**
 * Static class to provide string utilities not provided by the STL.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version May 29, 2016
 */
class String
{
public:
    /**
     * Split a string into a vector of strings.
     *
     * @param string The string to split.
     * @param char The delimiter to split the string on.
     *
     * @return A vector containing the split strings.
     */
    static std::vector<std::string> Split(const std::string &, char);

    /**
     * Replace all instances of a substring in a string with another substring.
     *
     * @param string The string container which will be modified.
     * @param string The string to search for and replace.
     * @param string The replacement string.
     */
    static void ReplaceAll(std::string &, const std::string &, const std::string &);

    /**
     * Remove all instances of a substring in a string.
     *
     * @param string The string container which will be modified.
     * @param string The string to search for and remove.
     */
    static void RemoveAll(std::string &, const std::string &);

    /**
     * Generate a random string of the given size.
     *
     * @param unsigned int The length of the string to generate.
     *
     * @return The generated string.
     */
    static std::string GenerateRandomString(const unsigned int);

    /**
     * Calculate the entropy of the given string. The larger the entropy, the
     * more random the string. An entropy of 0.0 implies completely non-random.
     *
     * @param string The string to examine.
     *
     * @return An entropy value in the range [0.0, inf).
     */
    static float CalculateEntropy(const std::string &);

    /**
     * Format a string with variadic template arguments. This is type safe in
     * in that the type doesn't matter with the format specifier (e.g. there's
     * no error if %s is given an integer). However, specifiers such as %x are
     * still attempted to be handled. That is, if the matching argument for %x
     * is numeric, then it will be printed in hexadecimal.
     *
     * There is also no checking done on the number of format specifiers and
     * the number of arguments. The format specifiers will be replaced one at a
     * time until all arguments are exhausted, then the rest of the string is
     * taken as-is. Any extra specifiers will be in the string. Any extra
     * arguments are dropped.
     *
     * @tparam Args Variadic template arguments.
     *
     * @param char* The string to format.
     * @param Args The variadic list of arguments to be formatted.
     *
     * @return A string that has been formatted with the given arguments.
     */
    template <typename ... Args>
    static std::string Format(const char *, const Args &...);

    /**
     * Concatenate a list of objects with the given separator.
     *
     * @tparam Args Variadic template arguments.
     *
     * @param char Character to use as a separator.
     * @param Args The variadic list of arguments to be joined.
     *
     * @return The resulting join of the given arguments.
     */
    template <typename ... Args>
    static std::string Join(const char &, const Args &...);

private:
    /**
     * Recursively format a string with one argument. The result is streamed
     * into the given ostream.
     */
    template <typename T, typename ... Args>
    static void format(std::ostream &, const char *, const T &, const Args &...);

    /**
     * Terminator for the variadic template formatter. Stream the rest of the
     * string into the given ostream.
     */
    static void format(std::ostream &, const char *);

    /**
     * Recursively join one argument into the given ostream.
     */
    template <typename T, typename ... Args>
    static void join(std::ostream &, const char &, const T &, const Args &...);

    /**
     * Terminator for the variadic template joiner. Join the last argument
     * into the given ostream.
     */
    template <typename T>
    static void join(std::ostream &, const char &, const T &);

    /**
     * Stream the given value into the given stream.
     */
    template <typename T, if_ostream::enabled<T> = 0>
    static bool getValue(std::ostream &, const T &);

    /**
     * Stream the hash of the given value into the given stream.
     */
    template <typename T, if_ostream::disabled<T> = 0, if_hash::enabled<T> = 0>
    static bool getValue(std::ostream &, const T &);

    /**
     * Streaming/hashing is not enabled for this type, so do nothing.
     */
    template <typename T, if_ostream::disabled<T> = 0, if_hash::disabled<T> = 0>
    static bool getValue(std::ostream &, const T &);

    /**
     * String to contain all alphanumeric characters with both capitalizations.
     */
    static const std::string s_alphaNum;

    /**
     * Number of ASCII characters.
     */
    static const unsigned int s_asciiSize;

    /**
     * A RNG for uniform integers.
     */
    static UniformIntegerDevice<size_t, std::mt19937> s_randomDevice;

};

//=============================================================================
template <typename ... Args>
std::string String::Format(const char *fmt, const Args &...args)
{
    std::ostringstream stream;
    stream.precision(6);

    if (fmt != NULL)
    {
        format(stream, fmt, args...);
    }

    return stream.str();
}

//=============================================================================
template <typename T, typename ... Args>
void String::format(
    std::ostream &stream,
    const char *fmt,
    const T &value,
    const Args &...args
)
{
    for ( ; *fmt != '\0'; ++fmt)
    {
        if (*fmt == '%')
        {
            char type = *(fmt + 1);

            switch (type)
            {
            case '\0':
                stream << *fmt;
                return;

            case 'x':
            case 'X':
                stream << "0x" << std::hex;
                getValue(stream, value);
                stream << std::dec;
                break;

            case 'f':
            case 'F':
            case 'g':
            case 'G':
                stream << std::fixed;
                getValue(stream, value);
                stream.unsetf(std::ios_base::fixed);
                break;

            case 'e':
            case 'E':
                stream << std::scientific;
                getValue(stream, value);
                stream.unsetf(std::ios_base::scientific);
                break;

            default:
                getValue(stream, value);
                break;
            }

            format(stream, fmt + 2, args...);
            return;
        }

        stream << *fmt;
    }
}

//=============================================================================
template <typename ... Args>
std::string String::Join(const char &separator, const Args &...args)
{
    std::ostringstream stream;
    join(stream, separator, args...);

    return stream.str();
}

//=============================================================================
template <typename T, typename ... Args>
void String::join(
    std::ostream &stream,
    const char &separator,
    const T &value,
    const Args &...args
)
{
    if (getValue(stream, value))
    {
        stream << separator;
    }

    join(stream, separator, args...);
}

//=============================================================================
template <typename T>
void String::join(std::ostream &stream, const char &, const T &value)
{
    getValue(stream, value);
}

//=============================================================================
template <typename T, if_ostream::enabled<T>>
bool String::getValue(std::ostream &stream, const T &value)
{
    stream << std::boolalpha << value;
    return true;
}

//=============================================================================
template <typename T, if_ostream::disabled<T>, if_hash::enabled<T>>
bool String::getValue(std::ostream &stream, const T &value)
{
    static std::hash<T> hasher;
    stream << "[0x" << std::hex << hasher(value) << std::dec << ']';
    return true;
}

//=============================================================================
template <typename T, if_ostream::disabled<T>, if_hash::disabled<T>>
bool String::getValue(std::ostream &, const T &)
{
    return false;
}

}
