#pragma once
#include <fstream>

#include "sdbq.h"

namespace sdbq
{

	bool CreateStatFile(std::string file_name, const std::vector<QuestionStats> stats);


}
