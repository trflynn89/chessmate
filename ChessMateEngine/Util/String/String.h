#pragma once

#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

#include <Util/Utilities.h>
#include <Util/Random/RandomDevice.h>

namespace Util {

/**
 * Static class to provide string utilities not provided by the STL.
 *
 * @author Timothy Flynn (trflynn89@gmail.com)
 * @version May 21, 2016
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
     * Calculate the entropy of the given string. The large the entropy, the
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
     * Concatenate a list of strings with the given separator.
     *
     * @tparam Args Variadic template arguments.
     *
     * @param char Character to use as a separator.
     * @param Args The variadic list of string-like arguments to be joined.
     *
     * @return The resulting join of the given arguments.
     */
    template <typename ... Args>
    static std::string Join(const char &, const Args &...);

private:
    /**
     * Recursively format a string with one argument. The result is streamed
     * into the given ostringstream.
     */
    template <typename T, typename ... Args>
    static void format(std::ostringstream &, const char *, const T &, const Args &...);

    /**
     * Terminator for the variadic template formatter. Stream the rest of the
     * string into the given ostringstream.
     */
    static void format(std::ostringstream &, const char *);

    /**
     * Recursively join one argument into the given ostringstream.
     */
    template <typename T, typename ... Args>
    static typename enable_if_str<T>::type join(
        std::ostringstream &, const char &, const T &, const Args &...);

    /**
     * @throws std::invalid_argument
     */
    template <typename T, typename ... Args>
    static typename disable_if_str<T>::type join(
        std::ostringstream &, const char &, const T &, const Args &...);

    /**
     * Terminator for the variadic template joiner. Join the last string
     * into the given ostringstream. Only valid for string-like types.
     */
    template <typename T>
    static typename enable_if_str<T>::type join(
        std::ostringstream &, const char &, const T &);

    /**
     * @throws std::invalid_argument
     */
    template <typename T>
    static typename disable_if_str<T>::type join(
        std::ostringstream &, const char &, const T &);

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
void String::format(std::ostringstream &stream, const char *fmt, const T &value, const Args &...args)
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
                stream << "0x" << std::hex << value << std::dec;
                break;

            case 'f':
            case 'F':
            case 'g':
            case 'G':
                stream << std::fixed << value;
                stream.unsetf(std::ios_base::fixed);
                break;

            case 'e':
            case 'E':
                stream << std::scientific << value;
                stream.unsetf(std::ios_base::scientific);
                break;

            default:
                stream << std::boolalpha << value;
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
typename enable_if_str<T>::type String::join(
    std::ostringstream &stream,
    const char &separator,
    const T &str,
    const Args &...args
)
{
    stream << str << separator;
    join(stream, separator, args...);
}

//=============================================================================
template <typename T, typename ... Args>
typename disable_if_str<T>::type String::join(
    std::ostringstream &,
    const char &,
    const T &str,
    const Args &...
)
{
    std::string message = Format("Cannot join non-string argument: %s", str);
    throw std::invalid_argument(message);
}

//=============================================================================
template <typename T>
typename enable_if_str<T>::type String::join(
    std::ostringstream &stream,
    const char &,
    const T &str
)
{
    stream << str;
}

//=============================================================================
template <typename T>
typename disable_if_str<T>::type String::join(
    std::ostringstream &,
    const char &,
    const T &str
)
{
    std::string message = Format("Cannot join non-string argument: %s", str);
    throw std::invalid_argument(message);
}

}
