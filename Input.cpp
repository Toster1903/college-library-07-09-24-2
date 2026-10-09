#include "Input.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
int parseInt(const std::string& value, int min, int max) {
    std::istringstream stream(value); int number; char extra;
    if (!(stream >> number) || (stream >> extra) || number < min || number > max)
        throw std::invalid_argument("Некорректное число");
    return number;
}
int readInt(const std::string& prompt, int min, int max) {
    for (;;) {
        std::cout << prompt; std::string line;
        if (!std::getline(std::cin, line)) throw std::runtime_error("Ввод завершён");
        try { return parseInt(line, min, max); }
        catch (const std::invalid_argument&) { std::cout << "Введите целое число от " << min << " до " << max << ".\n"; }
    }
}
std::string readNonEmpty(const std::string& prompt) {
    for (;;) {
        std::cout << prompt; std::string line;
        if (!std::getline(std::cin, line)) throw std::runtime_error("Ввод завершён");
        auto first = line.find_first_not_of(" \t\r");
        if (first != std::string::npos && line.find(';') == std::string::npos)
            return line.substr(first, line.find_last_not_of(" \t\r") - first + 1);
        std::cout << "Строка должна быть непустой и без символа ;\n";
    }
}
std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> result; std::size_t begin = 0, end;
    while ((end = line.find(delimiter, begin)) != std::string::npos) {
        result.push_back(line.substr(begin, end - begin)); begin = end + 1;
    }
    result.push_back(line.substr(begin)); return result;
}
