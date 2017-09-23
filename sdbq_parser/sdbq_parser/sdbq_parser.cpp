#include "sdbq_parser.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <map>

#include "csv.h"

namespace sdbq
{

	struct QuestionDescriptorHasher
	{
		size_t operator() (const Question& val) const	{ return std::hash<std::string>()(val.descriptor); }
	};

	struct QuestionDescriptorComparer
	{
		bool operator() (const Question& lhs, const Question& rhs) const { return lhs.descriptor == rhs.descriptor; };
	};

	struct StudentHasher
	{
		size_t operator() (const Student& val) const { return std::hash<std::string>()(val.first+val.second); }
	};

	struct StudentComparer
	{
		bool operator() (const Student& lhs, const Student& rhs) const { return (lhs.first == rhs.first) & (lhs.second == rhs.second); };
	};

	struct QuestionTestHasher
	{
		size_t operator() (const Question& val) const { return std::hash<std::string>()(val.test_name); }
	};

	struct QuestionTestComparer
	{
		bool operator() (const Question& lhs, const Question& rhs) const { return lhs.test_name == rhs.test_name; };
	};

	struct QuestionGradeHasher
	{
		size_t operator() (const Question& val) const { return std::hash<std::string>()(val.grade); }
	};

	struct QuestionGradeComparer
	{
		bool operator() (const Question& lhs, const Question& rhs) const { return lhs.grade == rhs.grade; };
	};

	std::vector<Question> ParseSdbq(std::string file_name, int count_estimate)
	{
		std::vector<Question> questions;
		questions.reserve(count_estimate);

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
		std::unordered_set<Question, QuestionGradeHasher, QuestionGradeComparer> unq_questions;

		for (const auto& q : questions)
			unq_questions.insert(q);

		std::vector<std::string> unq_questions_vec;
		unq_questions_vec.reserve(unq_questions.size());

		std::for_each(unq_questions.begin(), unq_questions.end(), [&](const auto& q) {unq_questions_vec.push_back(q.grade); });

		return unq_questions_vec;
	}

	std::vector<std::string> GetUniqueTests(const std::vector<Question>& questions)
	{
		std::unordered_set<Question, QuestionTestHasher, QuestionTestComparer> unq_questions;

		for (const auto& q : questions)
			unq_questions.insert(q);

		std::vector<std::string> unq_questions_vec;
		unq_questions_vec.reserve(unq_questions.size());

		std::for_each(unq_questions.begin(), unq_questions.end(), [&](const auto& q) {unq_questions_vec.push_back(q.test_name); });

		return unq_questions_vec;
	}

	std::vector<std::string> GetUniqueDescriptors(const std::vector<Question>& questions)
	{

		std::unordered_set<Question, QuestionDescriptorHasher, QuestionDescriptorComparer> unq_questions;

		for (const auto& q : questions)
			unq_questions.insert(q);

		std::vector<std::string> unq_questions_vec;
		unq_questions_vec.reserve(unq_questions.size());

		std::for_each(unq_questions.begin(), unq_questions.end(), [&](const auto& q) {unq_questions_vec.push_back(q.descriptor); });

		return unq_questions_vec;
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

	std::vector<Question> GetTest(std::vector<Question>& questions, std::string_view test_name)
	{
		std::vector<Question> test_questions;

		auto it = questions.begin();
		auto end = questions.end();

		while (it != end)
		{
			it = std::find_if(it, end, [&](const auto& val) { return val.test_name == test_name; });
			if (it != end)
			{
				test_questions.push_back(*it);
				it++;
			}
		}

		return test_questions;
	}

	std::vector<Question> GetCorrectQuestions(std::vector<Question>& questions)
	{
		std::vector<Question> cor_questions;

		auto it = questions.begin();
		auto end = questions.end();

		while (it != end)
		{
			it = std::find_if(it, end, [&](const auto& val) { return val.response == "COR"; });
			if (it != end)
			{
				cor_questions.push_back(*it);
				it++;
			}
		}

		return cor_questions;
	}

	std::vector<Question> GetIncorrectQuestions(std::vector<Question>& questions)
	{
		std::vector<Question> incor_questions;

		auto it = questions.begin();
		auto end = questions.end();

		while (it != end)
		{
			it = std::find_if(it, end, [&](const auto& val) { return val.response == "INC"; });
			if (it != end)
			{
				incor_questions.push_back(*it);
				it++;
			}
		}

		return incor_questions;
	}

	std::vector<QuestionStats> GetMeta(std::vector<Question>& questions)
	{

		std::map<std::string, std::vector<QuestionMeta>> descriptors;

		for (const auto& q : questions)
		{
			descriptors[q.descriptor].push_back({ q.difficulty, q.response, {q.first_name, q.last_name} });
		}

		std::vector<QuestionStats> question_stats;
		
		for (const auto& it : descriptors)
		{
			QuestionStats stats;
			stats.descriptor = it.first;
			std::unordered_set<Student, StudentHasher, StudentComparer> inc_students;
			std::unordered_set<Student, StudentHasher, StudentComparer> cor_students;


			for (const auto& student : it.second)
			{
				stats.difficulty = std::get<KMetaKey_Difficulty>(student);
				if (std::get<KMetaKey_Response>(student) == "COR")
				{
					stats.total_correct.push_back(std::get<KMetaKey_Student>(student));
					cor_students.insert(std::get<KMetaKey_Student>(student));
				}
				else
				{
					stats.total_incorrect.push_back(std::get<KMetaKey_Student>(student));
					inc_students.insert(std::get<KMetaKey_Student>(student));
				}
			}

			for (const auto& student : cor_students)
				stats.unique_correct.push_back(student);

			for (const auto& student : inc_students)
				stats.unique_incorrect.push_back(student);

			question_stats.push_back(stats);
		}

		return question_stats;
	}

}