#pragma once
#include <fstream>

#include "sdbq_decls.h"

namespace sdbq
{

	bool CreateStatFile(std::string file_name, const std::vector<QuestionStats> stats);


}
