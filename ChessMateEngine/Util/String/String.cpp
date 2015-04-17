/**
 * Implementation of the String interface.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version September 7, 2014
 */
#include "String.h"

#include <cmath>
#include <cstdlib>
#include <sstream>

using std::getline;
using std::mt19937;
using std::string;
using std::stringstream;
using std::vector;

using Util::Random::UniformIntegerDevice;

namespace Util { namespace String {

//=============================================================================
const string String::s_alphaNum =
	"0123456789"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz";

const unsigned int String::s_asciiSize = 256;

UniformIntegerDevice<unsigned int, mt19937> String::s_randomDevice(0, String::s_alphaNum.size() - 1);

//=============================================================================
vector<string> String::Split(const string &input, char delim)
{
	string item;
	stringstream ss(input);
	vector<string> elems;

	while (getline(ss, item, delim))
	{
		if (!item.empty())
		{
			elems.push_back(item);
		}
	}

	return elems;
}

//=============================================================================
void String::ReplaceAll(string &target, const string &search, const string &replace)
{
	size_t pos = target.find(search);

	while (!search.empty() && (pos != string::npos))
	{
		target.replace(pos, search.length(), replace);
		pos = target.find(search);
	}
}

//=============================================================================
void String::RemoveAll(string &target, const string &search)
{
	static const string empty;
	ReplaceAll(target, search, empty);
}

//=============================================================================
string String::GenerateRandomString(const unsigned int len)
{
	string ret;
	ret.reserve(len);

	for (unsigned int i = 0; i < len; ++i)
	{
		ret += s_alphaNum[s_randomDevice()];
	}

	return ret;
}

//=============================================================================
float String::CalculateEntropy(const string &source)
{
	long charCount[s_asciiSize] = { 0 };

	// Count the number of occurences of each ASCII character in the string
	for (auto it = source.begin(); it != source.end(); ++it)
	{
		unsigned int ascii = static_cast<unsigned int>(*it);

		if (ascii < s_asciiSize)
		{
			++charCount[ascii];
		}
	}

	float entropy = 0.0;
	float length = static_cast<float>(source.length());

	// Calculate the entropy
	for (unsigned int i = 0; i < s_asciiSize; ++i)
	{
		long count = charCount[i];

		if (count > 0)
		{
			float pct = static_cast<float>(count) / length;
			entropy -= (pct * log2(pct));
		}
	}

	return entropy;
}

}}
