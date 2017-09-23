#pragma once
#include <string>
#include <vector>
#include <tuple>

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

	using Student = std::pair<std::string, std::string>;
	using QuestionMeta = std::tuple<std::string, std::string, Student>;

	enum MetaKey
	{
		KMetaKey_Difficulty = 0,
		KMetaKey_Response = 1,
		KMetaKey_Student = 2
	};

	struct QuestionStats
	{
		std::string difficulty;
		std::string descriptor;
		std::vector<Student> total_correct;
		std::vector<Student> total_incorrect;
		std::vector<Student> unique_correct;
		std::vector<Student> unique_incorrect;
	};



}
