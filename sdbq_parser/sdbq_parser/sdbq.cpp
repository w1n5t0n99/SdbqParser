#include "sdbq.h"

#include <map>

#include "sdbq_decls.h"
#include "sdbq_parser.h"
#include "sdbq_writer.h"

namespace sdbq
{

	std::pair<bool, std::string> Parse(const std::string file_name)
	{
		auto total_questions = ParseSdbq(file_name, 50000);
		if (!total_questions)
			return { false, "cannot read file: " + file_name };

		std::map<std::string, std::vector<sdbq::QuestionStats>> test_map;

		auto tests = sdbq::GetUniqueTestsAndQuestions(*total_questions);
		for (const auto& test : tests)
		{
			auto stats = GetQuestionStats(test.second);

			CreateStatFile(test.first + ".csv", stats);
			test_map.insert({ test.first, std::move(stats) });
		}

		return { true, "file parsing completed." };
	}

	std::pair<bool, std::string> Merge(const std::string output_file, std::vector<std::string> input_files)
	{
		std::vector<std::vector<ResultStats>> results;

		for (const auto& f : input_files)
		{
			auto res = ParseResults(f, 150);
			if (!res)
				return { false, "cannot read file: " + f };

			results.push_back(std::move(*res));
		}

		std::vector<ResultStats> total_results;

		for (const auto& r : results)
			MergeResults(total_results, r);

		if( output_file.find(".csv") == std::string::npos)
			return { false, "cannot wrtie to file: " + output_file };

		CreateStatFile(output_file, total_results);
		
		return { true, "file parsing completed." };

	}

}