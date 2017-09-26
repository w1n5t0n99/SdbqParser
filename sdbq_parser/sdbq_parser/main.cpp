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

	args::Group cmd_group(parser, "possible commands you can execute", args::Group::Validators::Xor);

	args::Flag parse_flag(cmd_group, "parse", "the flag to parse all questions in csv", { 'p', "parse" });
	args::Flag merge_flag(cmd_group, "merge", "the flag to merge several results", { 'm', "merge" });

	args::ValueFlag<std::string> output_value(parser, "output file name", "this is the output file to use", { 'o', "output" });
	args::ValueFlagList<std::string> input_value(parser, "input files", "this is the file(s) to use", { 'i', "input" });

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

	bool parse = false;
	bool merge = false;

	if (parse_flag)
	{
		if (input_value &&  args::get(input_value).size() > 1)
		{
			std::cerr << "error! - can only specify one input for parsing" << std::endl;
			return 1;
		}

		if(output_value)
		{
			std::cerr << "error! - cannot specify output for parsing" << std::endl;
			return 1;
		}

		std::cout << "parsing... " << std::endl;
		parse = true;

		std::cout << "input: " << args::get(input_value)[0] << std::endl;
	}

	if (merge_flag)
	{
		std::cout << "merging... " << std::endl;
		merge = true;

		std::cout << "output: " << args::get(output_value) << std::endl;

		std::cout << "input: ";
		for (const auto& in : args::get(input_value))
			std::cout << in << "  ";
		
		std::cout << std::endl;
	}

	std::cout << "\nFinished!\n";
	//std::this_thread::sleep_for(1000ms);
	return 0;
}


