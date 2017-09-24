#include <iostream>
#include <fstream>
#include <string>
#include <fstream>

#include "csv.h"
#include "sdbq_parser.h"
#include "sdbq_writer.h"

int main()
{
	/*
	using LogStream = std::ofstream;

	LogStream log("test_results.txt");

	auto questions = sdbq::ParseSdbq("RCE Spring 17 SDBQ.csv", 50000);


	std::cout << "Total questions found: " << questions.size() << std::endl;

	auto grades = sdbq::GetUniqueGrades(questions);

	for (const auto& g : grades)
		std::cout << g << std::endl;
		
	auto tests = sdbq::GetUniqueTests(questions);

	std::cout << std::endl;

	for (const auto& t : tests)
		std::cout << t << std::endl;

	std::cout << std::endl;

	auto gq = sdbq::GetTest(questions, "Gr 3 Reading CAT");

	auto stats = sdbq::GetMeta(gq);

	
	for (const auto& s : stats)
	{
		log << s.descriptor << " | " << s.difficulty << " | ";
		log << "Total Correct: " << s.total_correct.size() << " Total Incorrect: " << s.total_incorrect.size() << " ";
		log << "Unique Correct: " << s.unique_correct.size() << " Unique Incorrect: " << s.unique_incorrect.size() << std::endl;
	}
	

	auto& d = stats[3].descriptor;
	auto& s = stats[3].total_correct;

	std::cout << d << std::endl;
	for (auto& i : s)
		std::cout << i.second << " " << i.first << std::endl;
		*/

	auto total_questions = sdbq::ParseSdbq("RCE Spring 17 SDBQ.csv", 50000);

	auto tests = sdbq::GetUniqueTests(total_questions);
	for (const auto& test : tests)
	{
		std::cout << "parsing - " << test << " ...\n";
		auto test_questions = sdbq::GetTest(total_questions, test);
		auto stats = sdbq::GetQuestionStats(test_questions);

		sdbq::CreateStatFile(test + ".csv", stats);
	}


	std::cout << "\nFinished!\n";
	std::cin.get();
	return 0;
}