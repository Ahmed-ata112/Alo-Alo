//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "node.h"
#include "iostream"
#include "string"
#include "vector"
#include "FileSystem/CoordinatorFileSystem.h"
Define_Module(Node);

void Node::initialize()
{
    CoordinatorFileSystem fileSystem = CoordinatorFileSystem("coordinator.txt");
    std::vector<std::string> lines = fileSystem.readFile();
    for (int i = 0; i < lines.size(); ++i) {
           std::cout<< lines[i] <<std::endl;
    }

    CoordinatorFileInfo info = fileSystem.getCoordinatorFileInfo();
    std::cout<<"info = "<<info.starting_node <<" "<<info.starting_time<<std::endl;
}

void Node::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}
