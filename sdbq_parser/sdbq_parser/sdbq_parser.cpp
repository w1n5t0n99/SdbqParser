#include "sdbq_parser.h"

#include <algorithm>
#include <iostream>

#include "csv.h"

namespace sdbq
{

	std::vector<Question> ParseByGrade(std::string file_name, int grade)
	{
		std::vector<Question> questions;

		io::CSVReader<11, typename io::trim_chars<' ', '\t'>, typename io::double_quote_escape<',', '\"'> > in("RCE Spring 17 SDBQ.csv");
		in.read_header(io::ignore_extra_column,
			"Last Name",
			"First Name",
			"MI", 
			"Grade",
			"Div Name",
			"Sch Name",
			"Group Name",
			"Test",
			"Item Descriptor", 
			"Item Difficulty", 
			"Response");
		//child
		std::string last_name = {};
		std::string first_name = {};
		std::string middle_initial = {};
		// question
		std::string item_descriptor = {};
		std::string response = {};
		std::string difficulty = {};
		// location 
		std::string division_name = {};
		std::string school_name = {};

		std::string grade_str = {};
		std::string test_name = {};
		std::string group_name = {};

		auto search_grade_str = std::to_string(grade);

		while (in.read_row(
			last_name,
			first_name,
			middle_initial,
			grade_str, 
			division_name, 
			school_name,
			group_name, 
			test_name,
			item_descriptor,
			difficulty,
			response))
		{
			if (grade_str.find(search_grade_str) != std::string::npos && test_name.find("Math") != std::string::npos)
			{
				questions.push_back({grade_str, test_name, group_name, response, difficulty, item_descriptor,
					last_name, first_name, middle_initial, division_name, school_name });
			}
		}

		return questions;
	}

	std::vector<Question> GetMissedQuestions(std::vector<Question>& questions)
	{
		std::vector<Question> missed_questions;

		for (auto& q : questions)
		{
			if(q.response.find("INC") != std::string::npos)
				missed_questions.push_back(q);
		}

		return missed_questions;
	}

	std::vector<std::pair<int, std::string>> GetDescriptorData(std::vector<Question>& questions)
	{
		std::vector<std::pair<int, std::string>> descriptor_data_vec = {};
		int count = 0;
		std::string cur_descriptor = questions[0].descriptor;

		for (const auto& q : questions)
		{
			if (cur_descriptor != q.descriptor)
			{
				//std::cout << count << ": missed - " << cur_descriptor << std::endl;
				descriptor_data_vec.push_back({ count, cur_descriptor });

				cur_descriptor = q.descriptor;
				count = 0;
			}

			++count;
		}

		std::sort(descriptor_data_vec.begin(), descriptor_data_vec.end(), [](const std::pair<int, std::string>& lhs, const std::pair<int, std::string>& rhs) {return rhs.first < lhs.first; });

		return descriptor_data_vec;
	}

	void SortByQuestionType(std::vector<Question>& questions)
	{

		auto response_comparer = [](const Question& lhs, const Question& rhs) -> bool {return lhs.difficulty < rhs.difficulty; };
		auto descrip_comparer = [](const Question& lhs, const Question& rhs) -> bool {return lhs.descriptor < rhs.descriptor; };

		std::sort(questions.begin(), questions.end(), response_comparer);

		Question response_test_question{};
		
		
		response_test_question.difficulty = "H";
		auto h_lb = std::lower_bound(questions.begin(), questions.end(), response_test_question, response_comparer);
		auto h_ub = std::upper_bound(questions.begin(), questions.end(), response_test_question, response_comparer);


		response_test_question.difficulty = "M";
		auto m_lb = std::lower_bound(questions.begin(), questions.end(), response_test_question, response_comparer);
		auto m_ub = std::upper_bound(questions.begin(), questions.end(), response_test_question, response_comparer);

		response_test_question.difficulty = "L";
		auto l_lb = std::lower_bound(questions.begin(), questions.end(), response_test_question, response_comparer);
		auto l_ub = std::upper_bound(questions.begin(), questions.end(), response_test_question, response_comparer);
		
		std::sort(h_lb, h_ub, descrip_comparer);
		std::sort(m_lb, m_ub, descrip_comparer);
		std::sort(l_lb, l_ub, descrip_comparer);

		std::cout << "H questions: " << std::distance(h_lb, h_ub) << std::endl;
		std::cout << "M questions: " << std::distance(m_lb, m_ub) << std::endl;
		std::cout << "L questions: " << std::distance(l_lb, l_ub) << std::endl;

	}

	//==============================================================
	//==============================================================
	std::vector<Question> ParseSdbq(std::string file_name)
	{
		std::vector<Question> questions;

		io::CSVReader<11, typename io::trim_chars<' ', '\t'>, typename io::double_quote_escape<',', '\"'> > in("RCE Spring 17 SDBQ.csv");
		in.read_header(io::ignore_extra_column,
			"Last Name",
			"First Name",
			"MI",
			"Grade",
			"Div Name",
			"Sch Name",
			"Group Name",
			"Test",
			"Item Descriptor",
			"Item Difficulty",
			"Response");
		//child
		std::string last_name = {};
		std::string first_name = {};
		std::string middle_initial = {};
		// question
		std::string item_descriptor = {};
		std::string response = {};
		std::string difficulty = {};
		// location 
		std::string division_name = {};
		std::string school_name = {};

		std::string grade_str = {};
		std::string test_name = {};
		std::string group_name = {};

		while (in.read_row(
			last_name,
			first_name,
			middle_initial,
			grade_str,
			division_name,
			school_name,
			group_name,
			test_name,
			item_descriptor,
			difficulty,
			response))
		{
		
			questions.push_back({ grade_str, test_name, group_name, response, difficulty, item_descriptor,
				last_name, first_name, middle_initial, division_name, school_name });
		}

		return questions;
	}

	std::vector<std::string> GetUniqueGrades(const std::vector<Question>& questions)
	{
		std::vector<std::string> grades;

		std::for_each(questions.begin(), questions.end(), 
			[&](auto q) 
			{
				if (std::find(grades.begin(), grades.end(), q.grade) == grades.end())
				{
					grades.push_back(q.grade);
				}
				
			});

		return grades;
	}

	std::vector<std::string> GetUniqueTests(const std::vector<Question>& questions)
	{
		std::vector<std::string> tests;

		std::for_each(questions.begin(), questions.end(),
			[&](auto q)
		{
			if (std::find(tests.begin(), tests.end(), q.test_name) == tests.end())
			{
				tests.push_back(q.test_name);
			}

		});

		return tests;
	}

	std::vector<Question> GetGrade(std::vector<Question>& questions, std::string_view grade)
	{
		std::vector<Question> grade_questions;

		auto it = questions.begin();
		auto end = questions.end();

		while (it != end)
		{
			it = std::find_if(it, end, [&](const auto& val) { return val.grade == grade; });
			if (it != end)
			{
				grade_questions.push_back(*it);
				it++;
			}
		}

		return grade_questions;
	}

}