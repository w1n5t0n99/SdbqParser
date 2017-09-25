#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <map>

#include "csv.h"
#include "args.hxx"

#include "sdbq_parser.h"
#include "sdbq_writer.h"

int main(int argc, char** argv)
{
	using namespace std::chrono_literals;

	/*

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
	
	*/

	args::ArgumentParser parser("RCPS sqdb parser.", "This goes after the options.");
	args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });
	args::ValueFlag<std::string> file_flag(parser, "file name", "this is the csv file to parse", { 'f' });
	args::Group group(parser, "This group is all exclusive:", args::Group::Validators::Xor);
	args::Flag parse_all_flag(group, "parse-all", "The flag to parse all tests", { "parse-all" });
	
	try
	{
		parser.ParseCLI(argc, argv);
	}
	catch (args::Help)
	{
		std::cout << parser;
		return 0;
	}
	catch (args::ParseError e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}
	catch (args::ValidationError e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}

	if (file_flag)
		std::cout << "file: " << args::get(file_flag) << std::endl;

	if (parse_all_flag)
		std::cout << "parse all tests!" << std::endl;

	std::cout << "\nFinished!\n";
	std::this_thread::sleep_for(1000ms);
	return 0;
}


