#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <map>

#include "csv.h"
#include "args.hxx"

#include "sdbq.h"

int main(int argc, char** argv)
{
	using namespace std::chrono_literals;

	args::ArgumentParser parser("rcps sqdb parser.");
	args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });

	args::Group cmd_group(parser, "possible commands you can execute", args::Group::Validators::Xor);

	args::Flag parse_flag(cmd_group, "parse", "the flag to parse all questions in csv", { 'p', "parse" });
	args::Flag merge_flag(cmd_group, "merge", "the flag to merge several results", { 'm', "merge" });

	args::ValueFlag<std::string> output_value(parser, "output file name", "this is the output file to use", { 'o', "output" });
	args::ValueFlagList<std::string> input_value(parser, "input files", "this is the input file(s) to use", { 'i', "input" });

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

		auto res = sdbq::Parse(args::get(input_value)[0]);
		if(!res.first)
		{
			std::cerr << res.second << std::endl;
			return 1;
		}
	}

	if (merge_flag)
	{
		std::cout << "merging... " << std::endl;
		merge = true;

		if (!output_value)
		{
			std::cerr << "error! - no output specified" << std::endl;
			return 1;
		}

		if (!input_value)
		{
			std::cerr << "error! - no inputs specified" << std::endl;
			return 1;
		}


		auto res = sdbq::Merge(args::get(output_value), args::get(input_value));
		if (!res.first)
		{
			std::cerr << res.second << std::endl;
			return 1;
		}
	}

	std::cout << "\nfinished!\n";
	return 0;
}


