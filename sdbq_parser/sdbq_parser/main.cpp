#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <map>

#include "csv.h"
#include "sdbq_parser.h"
#include "sdbq_writer.h"

int main(int argc, char** argv)
{
	using namespace std::chrono_literals;

	if (argc > 2)
	{
		std::cout << "Too many paramters specified!\n";
		std::this_thread::sleep_for(1000ms);
		return 1;
	}
	else if (argc < 2)
	{
		std::cout << "No file specified\n";
		std::this_thread::sleep_for(1000ms);
		return 1;
	}

	std::string csv_file(argv[1]);

	auto total_questions = sdbq::ParseSdbq(csv_file, 50000);
	if (!total_questions)
	{
		std::cout << "File not found! - " << csv_file << "\n";
		std::this_thread::sleep_for(1000ms);
		return 1;
	}

	std::map<std::string, std::vector<sdbq::QuestionStats>> test_map;

	auto tests = sdbq::GetUniqueTests(*total_questions);
	for (const auto& test : tests)
	{
		std::cout << "parsing - " << test << " ...\n";
		auto test_questions = sdbq::GetTestQuestions(*total_questions, test);
		auto stats = sdbq::GetQuestionStats(test_questions);

		sdbq::CreateStatFile(test + ".csv", stats);
		test_map.insert({ test, std::move(stats) });
	}

	const auto& stat0 = test_map["Gr 6 Math CAT"];
	const auto& stat1 = test_map["Gr 6 Mathematics"];

	const auto stat_merge = sdbq::MergeQuestionStats(stat0, stat1);
	sdbq::CreateStatFile("Grade-6-Math-Total.csv", stat_merge);
	
	std::cout << "\nFinished!\n";
	std::this_thread::sleep_for(1000ms);
	return 0;
}