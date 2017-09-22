#include <iostream>
#include <fstream>
#include <string>
#include <fstream>

#include "csv.h"
#include "sdbq_parser.h"

int main()
{

	/*
	io::CSVReader<5, typename io::trim_chars<' ', '\t'>, typename io::double_quote_escape<',', '\"'> > in("RCE Spring 17 SDBQ.csv");
	in.read_header(io::ignore_extra_column, "Last Name", "First Name", "MI", "Item Descriptor", "Response");
	std::string last_name = {};
	std::string first_name = {};
	std::string middle_initial = {};
	std::string item_descriptor = {};
	std::string response = {};
	int index = 0;

	while (in.read_row(last_name, first_name, middle_initial, item_descriptor, response))
	{
		if (last_name.find("WISE") != std::string::npos && first_name.find("JOSEPH") != std::string::npos)
			std::cout << ++index << " " << last_name << " " << first_name << " " << item_descriptor << " " << response << std::endl;
	}
	*/

	/*
	int grade = 4;
	auto questions = sdbq::ParseByGrade("RCE Spring 17 SDBQ.csv", grade);

	std::ofstream file("sdbq-results.txt");
	*/

	auto questions = sdbq::ParseSdbq("RCE Spring 17 SDBQ.csv");


	std::cout << "Total questions found: " << questions.size() << std::endl;

	/*
	auto missed_questions = sdbq::GetMissedQuestions(questions);

	file << "Missed questions found: " << missed_questions.size() << " in grade " << grade << " Math" << std::endl;

	sdbq::SortByQuestionType(missed_questions);
	auto descrip_data = sdbq::GetDescriptorData(missed_questions);


	for (auto& d : descrip_data)
		file << d.first << " Missed | " << d.second << std::endl;

		*/

	auto grades = sdbq::GetUniqueGrades(questions);

	for (const auto& g : grades)
		std::cout << g << std::endl;
		
	auto tests = sdbq::GetUniqueTests(questions);

	std::cout << std::endl;

	for (const auto& t : tests)
		std::cout << t << std::endl;

	std::cout << std::endl;

	auto gq = sdbq::GetGrade(questions, grades[0]);


	std::cout << gq[0].grade << std::endl;

	std::cout << "\nFinished!\n";
	std::cin.get();
	return 0;
}