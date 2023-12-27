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

#ifndef LOGGER_H_
#define LOGGER_H_
#include <iostream>
#include <fstream>

using namespace std;

class Logger
{
public:
    Logger();
    ~Logger();
    void logProcessingStart(int node_id, string error_code);
    void logFrameTransmission(int node_id, int seq_num,
                              const std::string &payload, char trailer, int modified,
                              bool lost, int dup, float delay);
    void logTimeoutEvent(int node_id, int seq_num);
    void logControlFrameSending(int node_id, bool ack_nack, int ack_num, bool loss);
    void logPayloadUploading(int seq_num, const std::string &payload);
};

#endif /* LOGGER_H_ */
