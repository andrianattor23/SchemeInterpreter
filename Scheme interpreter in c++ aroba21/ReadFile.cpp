#include "ReadFile.hpp"

std::string ReadFile::readLine() {
    std::string line;
    std::getline(std::cin, line);
    return (line.find_first_not_of(" ") == std::string::npos) ? "" : line;
}

std::string ReadFile::readFile(std::string fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) return "";

    std::string content, line;
    while (std::getline(file, line)) content += line + '\n';

    return content;
}



