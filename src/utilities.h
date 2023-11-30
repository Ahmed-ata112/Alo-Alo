/*
 * utilitiees.h
 *
 *  Created on: Nov 27, 2023
 *      Author: aatta
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "CustomMessage_m.h"
#include <iostream>
#include <string>
#include <bitset>
#include <fstream>
#include <vector>
#include "ErroredMsg.h"

#define ERROR_BITS 4

using namespace std;

enum control_signal
{
    NACK,
    ACK
};

vector<ErroredMsg> readfile(string path);

void frame_message(CustomMessage_Base *msg);

void checksum_message(CustomMessage_Base *msg);

void unframing_message(CustomMessage_Base *msg);

bool check_checksum(CustomMessage_Base *msg);

void coordinator_init(string path, bool &node_id, float &starting_time);

#endif /* UTILITIES_H_ */
