#pragma once
#include "Framework/Singleton.h"
#include <string>

//Only for Singletons
#define SUtils nc::StringUtils::Instance()

namespace nc
{
	class StringUtils : public Singleton<StringUtils>
	{
	public:
		std::string ToUpper(std::string string);
		std::string ToLower(std::string string);
		bool IsEqualIgnoreCase(std::string string1, std::string string2);
		void CreateUnique(std::string& string);

	private:
		std::string ChangeCase(std::string string, bool toUpper); //I didn't know if you could used the <cType.h> for the methods so I made it without them above
		
	};
}