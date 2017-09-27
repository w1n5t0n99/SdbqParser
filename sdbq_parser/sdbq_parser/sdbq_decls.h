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
		std::string retest;
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

	struct QuestionStats
	{
		std::string difficulty;
		std::string descriptor;
		std::vector<Student> total_correct;
		std::vector<Student> total_incorrect;
		std::vector<Student> unique_correct;
		std::vector<Student> unique_incorrect;
	};

	struct ResultStats
	{
		std::string difficulty;
		std::string descriptor;
		int total_correct;
		int total_incorrect;
		int unique_correct;
		int unique_incorrect;
	};

}
