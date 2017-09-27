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
		size_t operator() (const Question& val) const	{ return std::hash<std::string>()(val.descriptor+val.difficulty); }
	};

	struct QuestionDescriptorComparer
	{
		bool operator() (const Question& lhs, const Question& rhs) const { return (lhs.descriptor == rhs.descriptor) & (lhs.difficulty == rhs.difficulty); };
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

	std::optional<std::vector<Question>> ParseSdbq(std::string file_name, int count_estimate)
	{
		std::vector<Question> questions;
		questions.reserve(count_estimate);

		try
		{
			io::CSVReader<12, typename io::trim_chars<' ', '\t'>, typename io::double_quote_escape<',', '\"'> > in(file_name);
			in.read_header(io::ignore_extra_column,
				"Last Name",
				"First Name",
				"MI",
				"Grade",
				"Div Name",
				"Sch Name",
				"Group Name",
				"Test",
				"Retest",
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
			std::string retest = {};
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
				retest,
				item_descriptor,
				difficulty,
				response))
			{

				questions.push_back({ grade_str, test_name, retest, group_name, response, difficulty, item_descriptor,
					last_name, first_name, middle_initial, division_name, school_name });
			}
		}
		catch (io::error::can_not_open_file& err)
		{
			return {};
		}

		return questions;
	}

	std::optional<std::vector<ResultStats>> ParseResults(std::string file_name, int count_estimate)
	{
		std::vector<ResultStats> stats;
		stats.reserve(count_estimate);

		try
		{
			io::CSVReader<6, typename io::trim_chars<' ', '\t'>, typename io::double_quote_escape<',', '\"'> > in(file_name);
			in.read_header(io::ignore_extra_column,
				"descriptor",
				"difficulty",
				"total correct",
				"total incorrect",
				"unique correct",
				"unique incorrect");
			
			std::string descriptor = {};
			std::string difficulty = {};
			int total_correct = {};
			int total_incorrect = {};
			int unique_correct = {};
			int unique_incorrect = {};

			while (in.read_row(
				descriptor,
				difficulty,
				total_correct,
				total_incorrect,
				unique_correct,
				unique_incorrect))
			{

				stats.push_back({ difficulty, descriptor, total_correct, total_incorrect, unique_correct, unique_incorrect });
			}
		}
		catch (io::error::can_not_open_file& err)
		{
			return {};
		}

		return stats;
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

	std::vector<Question> GetGradeQuestions(std::vector<Question>& questions, std::string_view grade)
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

	std::vector<Question> GetTestQuestions(std::vector<Question>& questions, std::string_view test_name)
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

	std::vector<QuestionStats> GetQuestionStats(std::vector<Question>& questions)
	{
		std::vector<QuestionStats> question_stats;
		// map the descriptor and difficulty to students
		std::map<std::pair<std::string, std::string>, std::vector<Question>> descriptors;

		for (const auto& q : questions)
		{
			descriptors[{q.descriptor, q.difficulty}].push_back(q);
		}

		for (const auto& [d, qs] : descriptors)
		{
			QuestionStats stats;
			stats.descriptor = d.first;
			stats.difficulty = d.second;

			std::map<Student, std::string> unique_students;
			for (const auto& q : qs)
			{

				const auto& res = q.response;
				Student student = { q.first_name, q.last_name };

				if (res == "COR")
				{

					stats.total_correct.push_back(student);
					// we want to update an existing key to correct
					unique_students[student] = res;
				}
				else
				{
					stats.total_incorrect.push_back(student);
					// we dont want to change an existing key to incorrect
					unique_students.insert({ student, res });
				}

			}

			for (const auto& s : unique_students)
			{
				if(s.second == "COR")
					stats.unique_correct.push_back(s.first);
				else
					stats.unique_incorrect.push_back(s.first);
			}

			std::replace(stats.descriptor.begin(), stats.descriptor.end(), ',', ';');

			question_stats.push_back(stats);
		}

		return question_stats;
	}

	std::vector<QuestionStats> MergeQuestionStats(const std::vector<QuestionStats>& stats0, const std::vector<QuestionStats>& stats1)
	{
		std::vector<QuestionStats> merged_questions;
		std::map<std::pair<std::string, std::string>, QuestionStats> question_map;

		for (const auto& q : stats0)
			question_map.insert({ {q.descriptor, q.difficulty}, q });

		for (const auto& q : stats1)
		{
			auto& [it, success] = question_map.insert({ { q.descriptor, q.difficulty }, q });
			
			if (!success)
			{
				auto& merged_total_correct = it->second.total_correct;
				merged_total_correct.insert(merged_total_correct.end(), q.total_correct.begin(), q.total_correct.end());

				auto& merged_total_incorrect = it->second.total_incorrect;
				merged_total_incorrect.insert(merged_total_incorrect.end(), q.total_incorrect.begin(), q.total_incorrect.end());

				auto& merged_unique_correct = it->second.unique_correct;
				merged_unique_correct.insert(merged_unique_correct.end(), q.unique_correct.begin(), q.unique_correct.end());

				auto& merged_unique_incorrect = it->second.unique_incorrect;
				merged_unique_incorrect.insert(merged_unique_incorrect.end(), q.unique_incorrect.begin(), q.unique_incorrect.end());
			}
		}

		for (auto& s : question_map)
			merged_questions.push_back(std::move(s.second));

		return merged_questions;
	}

	void MergeResults(std::vector<ResultStats>& total_stats, const std::vector<ResultStats>& stats)
	{
		std::map<std::pair<std::string, std::string>, ResultStats> result_map;
		for (const auto& s : total_stats)
			result_map.insert({ { s.descriptor, s.difficulty }, s });

		for (const auto& s : stats)
		{
			auto&[it, success] = result_map.insert({ {s.descriptor, s.difficulty}, s });

			if (!success)
			{
				it->second.total_correct += s.total_correct;
				it->second.total_incorrect += s.total_incorrect;
				it->second.unique_correct += s.unique_correct;
				it->second.unique_incorrect += s.unique_incorrect;
			}
		}

		total_stats.clear();
		for (auto& r : result_map)
			total_stats.push_back(std::move(r.second));

	}

}