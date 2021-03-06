#pragma once

#include <string>
#include <vector>
#include <optional>

#include "sdbq_decls.h"

namespace sdbq
{
	
	std::optional<std::vector<Question>> ParseSdbq(std::string file_name, int count_estimate);
	std::optional<std::vector<ResultStats>> ParseResults(std::string file_name, int count_estimate);


	std::vector<std::string> GetUniqueGrades(const std::vector<Question>& questions);
	std::vector<std::string> GetUniqueTests(const std::vector<Question>& questions);
	std::vector<std::string> GetUniqueDescriptors(const std::vector<Question>& questions);

	std::vector<std::pair<std::string, std::vector<Question>>> GetUniqueTestsAndQuestions(const std::vector<Question>& questions);

	std::vector<Question> GetGradeQuestions(const std::vector<Question>& questions, std::string_view grade);
	std::vector<Question> GetTestQuestions(const std::vector<Question>& questions, std::string_view test_name);

	std::vector<QuestionStats> GetQuestionStats(const std::vector<Question>& questions);
	std::vector<QuestionStats> MergeQuestionStats(const std::vector<QuestionStats>& stats0, const std::vector<QuestionStats>& stats1);
	void MergeResults(std::vector<ResultStats>& total_stats, const std::vector<ResultStats>& stats);




}