#pragma once
#include <string>

namespace sdbq
{

	struct Question
	{
		std::string grade;
		std::string test_name;
		std::string group_name;
		// question
		std::string response;
		std::string difficulty;
		std::string descriptor;
		//child 
		std::string last_name;
		std::string first_name;
		std::string middle_initial;
		// location
		std::string division_name;
		std::string school_name;
	};

}
