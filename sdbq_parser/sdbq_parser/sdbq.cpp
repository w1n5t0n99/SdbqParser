#include "sdbq.h"

#include <map>

#include "sdbq_decls.h"
#include "sdbq_parser.h"
#include "sdbq_writer.h"

namespace sdbq
{

	std::pair<bool, std::string> Parse(const std::string file_name)
	{
		auto total_questions = sdbq::ParseSdbq(file_name, 50000);
		if (!total_questions)
			return { false, "cannot read file: " + file_name };

		std::map<std::string, std::vector<sdbq::QuestionStats>> test_map;

		auto tests = sdbq::GetUniqueTests(*total_questions);
		for (const auto& test : tests)
		{
			auto test_questions = sdbq::GetTestQuestions(*total_questions, test);
			auto stats = sdbq::GetQuestionStats(test_questions);

			sdbq::CreateStatFile(test + ".csv", stats);
			test_map.insert({ test, std::move(stats) });
		}

		return { true, "file parsing completed." };
	}

	std::pair<bool, std::string> Merge(const std::string output_file, std::vector<std::string> input_files)
	{
		return {false, "temp"};
	}

}