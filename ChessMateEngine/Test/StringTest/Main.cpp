#include <string>
#include <vector>

#include <Util/Logging/Logger.h>
#include <Util/String/String.h>

//=============================================================================
bool SplitTest()
{
    int numSectors = 10;
    std::vector<std::string> inputSplit(numSectors);

    std::string input;
    char delim = ' ';

    for (int i = 0; i < numSectors; ++i)
    {
        std::string curr = Util::String::GenerateRandomString(10);

        inputSplit[i] = curr;
        input += curr + delim;
    }

    std::vector<std::string> outputSplit = Util::String::Split(input, delim);

    if (inputSplit.size() != outputSplit.size())
    {
        LOGC("Split produced unexpected size %d, expected %d",
            outputSplit.size(), inputSplit.size());

        return false;
    }

    for (int i = 0; i < numSectors; ++i)
    {
        if (inputSplit[i] != outputSplit[i])
        {
            LOGC("Index %d differs in output %s, expected %s",
                i, outputSplit[i], inputSplit[i]);

            return false;
        }
    }

    return true;
}

//=============================================================================
bool ReplaceAllTest()
{
    std::string source("To Be Replaced! To Be Replaced!");
    std::string search("Be Replaced");
    std::string replace("new value");
    std::string result("To new value! To new value!");

    Util::String::ReplaceAll(source, search, replace);
    return (source == result);
}

//=============================================================================
bool ReplaceAllWithEmptyTest()
{
    std::string source("To Be Replaced! To Be Replaced!");
    std::string search;
    std::string replace("new value");
    std::string result("To Be Replaced! To Be Replaced!");

    Util::String::ReplaceAll(source, search, replace);
    return (source == result);
}

//=============================================================================
bool RemoveAllTest()
{
    std::string source("To Be Replaced! To Be Replaced!");
    std::string search("Be Rep");
    std::string result("To laced! To laced!");

    Util::String::RemoveAll(source, search);
    return (source == result);
}

//=============================================================================
bool RemoveAllWithEmptyTest()
{
    std::string source("To Be Replaced! To Be Replaced!");
    std::string search;
    std::string result("To Be Replaced! To Be Replaced!");

    Util::String::RemoveAll(source, search);
    return (source == result);
}

//=============================================================================
bool GenerateRandomStringTest(unsigned int length)
{
    std::string random = Util::String::GenerateRandomString(length);
    return (length == random.length());
}

//=============================================================================
bool EntropyTest()
{
    std::string str1 = Util::String::GenerateRandomString(1 << 10);
    std::string str2("A quick brown fox jumped over the lazy dog");
    std::string str3("repeatrepeatrepeatrepeatrepeatrepeatrepeat");
    std::string str4("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");

    float ent1 = Util::String::CalculateEntropy(str1);
    float ent2 = Util::String::CalculateEntropy(str2);
    float ent3 = Util::String::CalculateEntropy(str3);
    float ent4 = Util::String::CalculateEntropy(str4);

    LOGC("E1=%f, E2=%f, E3=%f, E4=%f", ent1, ent2, ent3, ent4);

    // Expect entropy to decrease for less random strings
    return ((ent1 > ent2) && (ent2 > ent3) && (ent3 > ent4));
}

//=============================================================================
bool formatTest(const std::string &expected, const std::string &actual)
{
    if (expected != actual)
    {
        LOGC("Expected %s, but got %s", expected, actual);
        return true;
    }

    return false;
}

//=============================================================================
bool FormatTest()
{
    int numErrors = 0;

    numErrors += formatTest("", Util::String::Format(""));
    numErrors += formatTest("%", Util::String::Format("%"));
    numErrors += formatTest("%%", Util::String::Format("%%"));
    numErrors += formatTest("%d", Util::String::Format("%d"));
    numErrors += formatTest("This is a test", Util::String::Format("This is a test"));
    numErrors += formatTest("test 1 true 2.100000 false 1.230000e+002", Util::String::Format("test %d %d %f %d %e", 1, true, 2.1f, false, 123.0));
    numErrors += formatTest("test some string s", Util::String::Format("test %s %c", std::string("some string"), 's'));

    return (numErrors == 0);
}

//=============================================================================
int main()
{
    int numErrors = 0;

    if (SplitTest())
    {
        LOGC("Split test passed");
    }
    else
    {
        LOGC("Split test failed");
        ++numErrors;
    }

    if (ReplaceAllTest() && ReplaceAllWithEmptyTest())
    {
        LOGC("ReplaceAll test passed");
    }
    else
    {
        LOGC("ReplaceAll test failed");
        ++numErrors;
    }

    if (RemoveAllTest() && RemoveAllWithEmptyTest())
    {
        LOGC("RemoveAll test passed");
    }
    else
    {
        LOGC("RemoveAll test failed");
        ++numErrors;
    }

    if (GenerateRandomStringTest(128))
    {
        LOGC("Small random string test passed");
    }
    else
    {
        LOGC("Small random string test failed");
        ++numErrors;
    }

    if (GenerateRandomStringTest(1 << 20))
    {
        LOGC("Large random string test passed");
    }
    else
    {
        LOGC("Large random string test failed");
        ++numErrors;
    }

    if (EntropyTest())
    {
        LOGC("Entropy test passed");
    }
    else
    {
        LOGC("Entropy test failed");
        ++numErrors;
    }

    if (FormatTest())
    {
        LOGC("Format test passed");
    }
    else
    {
        LOGC("Format test failed");
        ++numErrors;
    }

    return numErrors;
}
