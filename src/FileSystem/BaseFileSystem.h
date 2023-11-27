/*
 * BaseFileSystem.h
 *
 *  Created on: Nov 27, 2023
 *      Author: Lenovo
 */

#ifndef FILESYSTEM_BASEFILESYSTEM_H_
#define FILESYSTEM_BASEFILESYSTEM_H_

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class BaseFileSystem {
private:
    std::string file_name;
    std::string RELATIVE_PATH = "FileSystem/";
public:
    BaseFileSystem(std::string);
    virtual ~BaseFileSystem();
    std::vector<std::string> readFile();
};

#endif /* FILESYSTEM_BASEFILESYSTEM_H_ */
