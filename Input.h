#pragma once
#include <string>
#include <vector>
int readInt(const std::string& prompt, int min, int max);
std::string readNonEmpty(const std::string& prompt);
std::vector<std::string> split(const std::string& line, char delimiter);
int parseInt(const std::string& value, int min, int max);
