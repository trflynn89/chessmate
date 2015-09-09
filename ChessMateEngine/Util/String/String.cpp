#include "String.h"

#include <cmath>
#include <cstdlib>
#include <sstream>

namespace Util {

//=============================================================================
const std::string String::s_alphaNum =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

const unsigned int String::s_asciiSize = 256;

UniformIntegerDevice<unsigned int, std::mt19937> String::s_randomDevice(0, String::s_alphaNum.size() - 1);

//=============================================================================
std::vector<std::string> String::Split(const std::string &input, char delim)
{
    std::string item;
    std::stringstream ss(input);
    std::vector<std::string> elems;

    while (std::getline(ss, item, delim))
    {
        if (!item.empty())
        {
            elems.push_back(item);
        }
    }

    return elems;
}

//=============================================================================
void String::ReplaceAll(std::string &target, const std::string &search, const std::string &replace)
{
    size_t pos = target.find(search);

    while (!search.empty() && (pos != std::string::npos))
    {
        target.replace(pos, search.length(), replace);
        pos = target.find(search);
    }
}

//=============================================================================
void String::RemoveAll(std::string &target, const std::string &search)
{
    static const std::string empty;
    ReplaceAll(target, search, empty);
}

//=============================================================================
std::string String::GenerateRandomString(const unsigned int len)
{
    std::string ret;
    ret.reserve(len);

    for (unsigned int i = 0; i < len; ++i)
    {
        ret += s_alphaNum[s_randomDevice()];
    }

    return ret;
}

//=============================================================================
float String::CalculateEntropy(const std::string &source)
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

//=============================================================================
void String::format(std::ostringstream &stream, const char *fmt)
{
    stream << fmt;
}

}
