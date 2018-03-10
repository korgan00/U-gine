#pragma once

#include <sstream>
#include <fstream>

std::string readString(const std::string& filename) {
	std::ifstream istream(filename.c_str(), std::ios_base::binary);
	std::stringstream sstream;
	sstream << istream.rdbuf();
	return sstream.str();
}
