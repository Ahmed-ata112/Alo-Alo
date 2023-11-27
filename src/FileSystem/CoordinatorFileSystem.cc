/*
 * CoordinatorFileSystem.cpp
 *
 *  Created on: Nov 27, 2023
 *      Author: Lenovo
 */

#include "CoordinatorFileSystem.h"

CoordinatorFileSystem::CoordinatorFileSystem(std::string file_name):BaseFileSystem(file_name) {
    // TODO Auto-generated constructor stub
    std::cout<<"CoordinatorFileSystem"<<std::endl;
}

CoordinatorFileSystem::~CoordinatorFileSystem() {
    // TODO Auto-generated destructor stub
}

CoordinatorFileInfo CoordinatorFileSystem::getCoordinatorFileInfo(){
    std::vector<std::string> lines = this->readFile();
    CoordinatorFileInfo info;
    info.starting_node = lines[0][0] - '0';


    // Extract the starting time string
    std::string numberString = lines[0].substr(2);

    // Convert the substring to an integer
    std::istringstream iss(numberString);
    iss >> info.starting_time;
    return info;
}


