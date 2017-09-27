#include "sdbq_writer.h"

#include <algorithm>

namespace sdbq
{

	bool CreateStatFile(std::string file_name, const std::vector<QuestionStats> stats)
	{

		std::ofstream file(file_name);

		if (!file)
			return false;

		//create header row
		file << "descriptor,difficulty,total correct,total incorrect,unique correct,unique incorrect\n";

		for (const auto& s : stats)
		{
			file << s.descriptor << "," << s.difficulty << "," << s.total_correct.size() << "," << s.total_incorrect.size() << "," << s.unique_correct.size() << "," << s.unique_incorrect.size() << "\n";
		}

		return true;
	}

	bool CreateStatFile(std::string file_name, const std::vector<ResultStats> stats)
	{
		std::ofstream file(file_name);

		if (!file)
			return false;

		//create header row
		file << "descriptor,difficulty,total correct,total incorrect,unique correct,unique incorrect\n";

		for (const auto& s : stats)
		{
			file << s.descriptor << "," << s.difficulty << "," << s.total_correct << "," << s.total_incorrect << "," << s.unique_correct << "," << s.unique_incorrect << "\n";
		}

		return true;
	}

}