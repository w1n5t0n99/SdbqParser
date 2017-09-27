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

	args::ArgumentParser parser("RCPS-SDBQ-Parser.", "Must include extentions (.csv) with file names\n");
	args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });

	args::Group cmd_group(parser, "Possible commands you can execute", args::Group::Validators::Xor);

	args::ValueFlag<std::string> parse_flag(cmd_group, "parse", "Parse sdbq file", { 'p', "parse" });
	args::Group merge_group(cmd_group, "Flags needed to execute merge", args::Group::Validators::All);

	args::Flag merge_flag(merge_group, "merge", "Merge multiple results files", { 'm', "merge" });
	args::ValueFlag<std::string> output_value(merge_group, "output file name", "This is the output file to use", { 'o', "output" });
	args::ValueFlagList<std::string> input_value(merge_group, "input files names", "This is the input file(s) to use", { 'i', "input" });

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

	if (parse_flag)
	{
		
		std::cout << "parsing... " << std::endl;

		auto res = sdbq::Parse(args::get(parse_flag));
		if(!res.first)
		{
			std::cerr << res.second << std::endl;
			return 1;
		}
	}

	if (merge_flag)
	{
		std::cout << "merging... " << std::endl;

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


