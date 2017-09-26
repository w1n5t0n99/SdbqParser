#pragma once
#include <string>
#include <vector>

namespace sdbq
{
	
	std::pair<bool, std::string> Parse(const std::string file_name);

	std::pair<bool, std::string> Merge(const std::string output_file, std::vector<std::string> input_files);
		
}
