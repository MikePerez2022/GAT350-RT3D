#include "StringUtils.h"
#include <vector>

namespace nc
{
	std::string StringUtils::ToUpper(std::string string)
	{
		//Edge Cases
		if (string.empty()) return "";

		std::string result = "";

		//2 strings that contain all neccessary options becuase the base is a char array and is unnecesary to make a vector
		std::string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		std::string lower = "abcdefghijklmnopqrstuvwxyz";

		for (char s : string)
		{
			bool done = false;

			for (int i = 0; i < lower.size(); i++)
			{
				//speed up process if the char was already mdified
				if (done) continue;

				//check for any matches if so then change the s to lowercase
				if (s == lower[i])
				{
					s = upper[i];
					done = true;
				}
			}
			result += s;
		}

		return result;
	}

	std::string StringUtils::ToLower(std::string string)
	{
		//Only one change from ToUpper
		if (string.empty()) return "";

		std::string result = "";

		std::string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		std::string lower = "abcdefghijklmnopqrstuvwxyz";

		for (char s : string)
		{
			bool done = false;

			for (int i = 0; i < upper.size(); i++)
			{
				if (done) continue;

				//swap upper and lower
				if (s == upper[i])
				{
					s = lower[i];
					done = true;
				}
			}
			result += s;
		}

		return result;
	}

	bool StringUtils::IsEqualIgnoreCase(std::string string1, std::string string2)
	{
		//Edge cases
		if (string1.empty() || string2.empty()) return false;

		//Lowercases both strings to ignor case and compare
		std::string check1 = ToLower(string1);
		std::string check2 = ToLower(string2);

		//checks and returns true or false using a turnurary op
		return (check1 == check2) ? true : false; 
	}

	void StringUtils::CreateUnique(std::string& string)
	{
		//make the unigned int in the method  --  static makes the increments to the int last
		static unsigned int unique = 0;
		unique++;

		//Appends unique unsigned int and changes the string because it's a refrence
		string += std::to_string(unique);
	}

	std::string StringUtils::ChangeCase(std::string string, bool toUpper)
	{
		//Edge Cases
		if (string.empty()) return "";

		std::string result = "";

		for (char s : string)
		{
			// if upper all chars are uppercased else they are lowercased  --  I used std::ignore to stop the 2 warnings
			if (toUpper) std::ignore = std::toupper(s);
			else std::ignore = std::tolower(s);

			result += s;
		}

		//I didn't know if you could used the <cType.h> for the methods so I made it without them above and I will switch it to this code later

		return result;
	}
}