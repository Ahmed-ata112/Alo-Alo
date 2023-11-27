/*
 * CoordinatorFileSystem.h
 *
 *  Created on: Nov 27, 2023
 *      Author: Lenovo
 */

#ifndef FILESYSTEM_COORDINATORFILESYSTEM_H_
#define FILESYSTEM_COORDINATORFILESYSTEM_H_

#include "BaseFileSystem.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

struct CoordinatorFileInfo {
    int starting_node;
    int starting_time;
};

class CoordinatorFileSystem: public BaseFileSystem{
public:
    CoordinatorFileSystem(std::string file_name);
    virtual ~CoordinatorFileSystem();
    CoordinatorFileInfo getCoordinatorFileInfo();
};

#endif /* FILESYSTEM_COORDINATORFILESYSTEM_H_ */
