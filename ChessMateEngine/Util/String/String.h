/**
 * Static class to provide string utilities not provided by the STL.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version September 7, 2014
 */
#pragma once

#include <string>
#include <vector>

#include <Util/Random/RandomDevice.h>

namespace Util { namespace String {

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

private:
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
	static Util::Random::UniformIntegerDevice<unsigned int, std::mt19937> s_randomDevice;
};

}}
