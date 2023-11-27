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

#define ERROR_BITS 4

using namespace std;

class ErroredMsg
{
public:
    string payload;
    vector<bool> errors;
    ErroredMsg(string line)
    {
        payload = line.substr(ERROR_BITS + 1, line.length() - 1);
        string flag = line.substr(0, ERROR_BITS);
        for (int i = 0; i < ERROR_BITS; i++)
            errors.push_back(flag[i] == '1');
    }
    //[Modification, Loss, Duplication, Delay]
    void clear_errors() { errors = {false, false, false, false}; }
    bool is_modified() { return errors[0]; }
    bool is_duplicated() { return errors[2]; }
    bool is_lost() { return errors[1]; }
    bool is_delayed() { return errors[3]; }
};

vector<ErroredMsg> readfile(string path)
{
    vector<ErroredMsg> Messages;
    ifstream file(path);
    string str;
    while (getline(file, str))
    {
        ErroredMsg msg(str);
        Messages.push_back(msg);
    }
    return Messages;
}

void frame_message(CustomMessage_Base *msg)
{
    // frame payload with byte stuffing
    string payload = msg->getPayload();
    string framed_payload = "";
    int size = payload.size();
    for (int i = 0; i < size; i++)
    {
        if (payload[i] == '$' || payload[i] == '/')
            framed_payload.push_back('/');
        framed_payload.push_back(payload[i]);
    }
    msg->setPayload(framed_payload.c_str());
}

void checksum_message(CustomMessage_Base *msg)
{
    string payload = msg->getPayload();
    int size = payload.size();
    char checksum = 0;
    for (int i = 0; i < size; i++)
    {
        checksum ^= payload[i];
    }
    checksum = ~checksum;
    msg->setTrailer(checksum);
}


#endif /* UTILITIES_H_ */
