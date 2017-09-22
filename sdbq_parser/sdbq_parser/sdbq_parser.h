#pragma once

#include <string>
#include <vector>

#include "sdbq.h"

namespace sdbq
{
	
	std::vector<Question> ParseByGrade(std::string file_name, int grade);

	std::vector<Question> GetMissedQuestions(std::vector<Question>& questions);

	std::vector<std::pair<int, std::string>> GetDescriptorData(std::vector<Question>& questions);

	void SortByQuestionType(std::vector<Question>& questions);


	std::vector<Question> ParseSdbq(std::string file_name);

	std::vector<std::string> GetUniqueGrades(const std::vector<Question>& questions);
	std::vector<std::string> GetUniqueTests(const std::vector<Question>& questions);


	std::vector<Question> GetGrade(std::vector<Question>& questions, std::string_view grade);
	std::vector<Question> GetTest(std::vector<Question>& questions, std::string_view test);
	std::vector<Question> GetCorrectQuestions(std::vector<Question>& questions);
	std::vector<Question> GetIncorrectQuestions(std::vector<Question>& questions);




}