/*
 * utilitiees.h
 *
 *  Created on: Nov 27, 2023
 *      Author: aatta
 */
#include "utilities.h"

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

void unframing_message(CustomMessage_Base *msg)
{
    string payload = msg->getPayload();
    string unframed_payload = "";
    int size = payload.size();
    for (int i = 0; i < size; i++)
    {
        if (payload[i] == '/' && payload[i + 1] == '/')
        {
            unframed_payload.push_back('/');
            i++;
        }
        else if (payload[i] == '/' && payload[i + 1] != '/')
            continue;
        unframed_payload.push_back(payload[i]);
    }
    msg->setPayload(unframed_payload.c_str());
}

bool check_checksum(CustomMessage_Base *msg)
{
    string payload = msg->getPayload();
    int size = payload.size();
    char checksum = 0;
    for (int i = 0; i < size; i++)
    {
        checksum ^= payload[i];
    }
    checksum = ~checksum;
    return (msg->getTrailer() == checksum);
}