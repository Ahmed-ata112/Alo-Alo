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

#include "Logger.h"

#include <fstream>
#include <omnetpp.h>
#include <iostream>
#include <bitset>
using namespace omnetpp;

using namespace std;

std::ofstream log_file;

Logger::Logger()
{
    // delete old log file
    std::remove("output.txt");
}

Logger::~Logger()
{
    if (log_file.is_open())
    {
        log_file.close();
    }
}

void Logger::logProcessingStart(int node_id, string error_code)
{
    log_file.open("output.txt", std::ios::out | std::ios::app);

    // log_file << "At time [" << simTime() << "] starting processing, Node[" << node_id << "], Introducing channel error with code [" << error_code << "]."
    //          << endl;

    string msg = "At time [" + std::to_string(simTime().dbl()) + "] starting processing, Node[" + std::to_string(node_id) + "], Introducing channel error with code [" + error_code + "].";
    EV << msg << endl;
    log_file << msg << endl;
    log_file.close();
}

void Logger::logFrameTransmission(int node_id, int seq_num,
                                  const std::string &payload, char trailer, int modified,
                                  bool lost, int dup, float delay)
{

    log_file.open("output.txt", std::ios::out | std::ios::app);

    // log_file << "At time [" << simTime() << "] starting sending frame with seq_num=[" << (int)seq_num
    //          << "] and payload=[" << payload << "] and trailer=[" << (int)trailer << "], Modified ["
    //          << modified << "], Lost [" << (lost ? "Yes" : "NO") << "], Duplicate [" << dup << "] ,  Delay [" << delay << "]." << endl;

    string msg = "At time [" + std::to_string(simTime().dbl()) + "] starting sending frame with seq_num=[" + std::to_string(seq_num) + "] and payload=[" + payload + "] and trailer=[" + std::bitset<8>(trailer).to_string() + "], Modified [" + std::to_string(modified) + "], Lost [" + (lost ? "Yes" : "NO") + "], Duplicate [" + std::to_string(dup) + "] ,  Delay [" + std::to_string(delay) + "].";
    EV << msg << endl;
    log_file << msg << endl;
    log_file.close();
}

void Logger::logTimeoutEvent(int node_id, int seq_num)
{
    log_file.open("output.txt", std::ios::out | std::ios::app);

    std::string str = "Time out event at time [" + std::to_string(simTime().dbl()) + "], at Node[" + std::to_string(node_id) + "] for frame with seq_num=[" + std::to_string(seq_num) + "].";
    EV << str << endl;
    log_file << str << endl;

    log_file.close();
}

void Logger::logControlFrameSending(int node_id, bool ack_nack, int ack_num, bool loss)
{
    log_file.open("output.txt", std::ios::out | std::ios::app);

    // log_file << "At time [" << (simTime().dbl()) << "] Node[" << node_id << "] Sending [" << (ack_nack ? "ACK" : "NACK") << "] with loss [" << (loss ? "YES" : "NO") << "].";

    string msg = "At time [" + std::to_string(simTime().dbl()) + "] Node[" + std::to_string(node_id) + "] Sending [" + (ack_nack ? "ACK" : "NACK") + "] with number [" + std::to_string(ack_num) + "] with loss [" + (loss ? "YES" : "NO") + "].";
    EV << msg << endl;
    log_file << msg << endl;

    log_file.close();
}

void Logger::logPayloadUploading(int seq_num, const std::string &payload)
{
    log_file.open("output.txt", std::ios::out | std::ios::app);
    // log_file << "Uploading payload=[" << payload << "] and seq_num=[" << (int)seq_num << "] to the network layer.";
    string msg = "Uploading payload=[" + payload + "] and seq_num=[" + std::to_string(seq_num) + "] to the network layer.";
    EV << msg << endl;
    log_file << msg << endl;
    log_file.close();
}
