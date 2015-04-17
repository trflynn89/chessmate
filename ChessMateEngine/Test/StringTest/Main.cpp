#include <string>
#include <vector>

#include <Util/Logging/Logger.h>
#include <Util/String/String.h>

using std::string;
using std::vector;

using Util::String::String;

//=============================================================================
bool SplitTest()
{
	int numSectors = 10;
	vector<string> inputSplit(numSectors);

	string input;
	char delim = ' ';

	for (int i = 0; i < numSectors; ++i)
	{
		string curr = String::GenerateRandomString(10);

		inputSplit[i] = curr;
		input += curr + delim;
	}

	vector<string> outputSplit = String::Split(input, delim);

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
			LOGC("Index %d differs in output \"%s\", expected \"%s\"",
				outputSplit[i].c_str(), inputSplit[i].c_str());

			return false;
		}
	}

	return true;
}

//=============================================================================
bool ReplaceAllTest()
{
	string source("To Be Replaced! To Be Replaced!");
	string search("Be Replaced");
	string replace("new value");
	string result("To new value! To new value!");

	String::ReplaceAll(source, search, replace);
	return (source == result);
}

//=============================================================================
bool ReplaceAllWithEmptyTest()
{
	string source("To Be Replaced! To Be Replaced!");
	string search;
	string replace("new value");
	string result("To Be Replaced! To Be Replaced!");

	String::ReplaceAll(source, search, replace);
	return (source == result);
}

//=============================================================================
bool RemoveAllTest()
{
	string source("To Be Replaced! To Be Replaced!");
	string search("Be Rep");
	string result("To laced! To laced!");

	String::RemoveAll(source, search);
	return (source == result);
}

//=============================================================================
bool RemoveAllWithEmptyTest()
{
	string source("To Be Replaced! To Be Replaced!");
	string search;
	string result("To Be Replaced! To Be Replaced!");

	String::RemoveAll(source, search);
	return (source == result);
}

//=============================================================================
bool GenerateRandomStringTest(unsigned int length)
{
	string random = String::GenerateRandomString(length);
	return (length == random.length());
}

//=============================================================================
bool EntropyTest()
{
	string str1 = String::GenerateRandomString(1 << 10);
	string str2("A quick brown fox jumped over the lazy dog");
	string str3("repeatrepeatrepeatrepeatrepeatrepeatrepeat");
	string str4("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");

	float ent1 = String::CalculateEntropy(str1);
	float ent2 = String::CalculateEntropy(str2);
	float ent3 = String::CalculateEntropy(str3);
	float ent4 = String::CalculateEntropy(str4);

	LOGC("E1=%f, E2=%f, E3=%f, E4=%f", ent1, ent2, ent3, ent4);

	// Expect entropy to decrease for less random strings
	return ((ent1 > ent2) && (ent2 > ent3) && (ent3 > ent4));
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

	if (GenerateRandomStringTest(1 << 30))
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

	return numErrors;
}
