#include <iostream>
#include <fstream>
#include <string>
#include <fstream>

#include "csv.h"
#include "sdbq_parser.h"

int main()
{
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

	auto gq = sdbq::GetTest(questions, "Gr 4 Math CAT");

	auto stats = sdbq::GetMeta(gq);

	for (const auto& s : stats)
	{
		log << s.descriptor << " | " << s.difficulty << " | ";
		log << "Total Correct: " << s.total_correct.size() << " Total Incorrect: " << s.total_incorrect.size() << " ";
		log << "Unique Correct: " << s.unique_correct.size() << " Unique Incorrect: " << s.unique_incorrect.size() << std::endl;
	}


	std::cout << "\nFinished!\n";
	std::cin.get();
	return 0;
}