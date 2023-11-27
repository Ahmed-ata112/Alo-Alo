/*
 * BaseFileSystem.cpp
 *
 *  Created on: Nov 27, 2023
 *      Author: Lenovo
 */

#include "BaseFileSystem.h"

BaseFileSystem::BaseFileSystem(std::string file_name) : file_name(file_name) {
    std::cout << "BaseFileSystem constructor" << std::endl;
}

BaseFileSystem::~BaseFileSystem() {
    // TODO Auto-generated destructor stub
}

std::vector<std::string> BaseFileSystem::readFile() {
    // Open a file for reading
    std::ifstream inputFile(this->RELATIVE_PATH + this->file_name);

    // Lines of the file
    std::vector<std::string> lines;

    // Check if the file is opened successfully
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return std::vector<std::string>();
    }

    // Read the file content
    std::string line;
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }

    // Close the file
    inputFile.close();
    return lines;
}
