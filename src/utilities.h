#include "CustomMessage_m.h"
#include <iostream>
#include <string>
#include <bitset>
#include <fstream>
#include <vector>
using namespace std;
#define ERROR_BITS 4

using namespace std;

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
            errors.push_back(flag[i] == '0');
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

int main()
{
    vector<ErroredMsg> messages = readfile("input0.txt");
}

void send_message_with_error(ErroredMsg message, char seq_num, int TD, int ED, int DD)
{

    if (message.is_lost())
        return;

    // create message
    CustomMessage_Base *msg = new CustomMessage_Base();
    msg->setPayload(message.payload.c_str());
    frame_message(msg);
    checksum_message(msg);
    msg->setHeader(seq_num);
    msg->setType(0); // data

    int size = message.payload.size();

    // add processing time and transmission delay
    total_delay = TD;

    if (message.is_delayed())
    {
        // if message is delayed, add extra delay
        total_delay += ED;
    }

    if (message.is_modified())
    {
        // generate random index for character in payload
        int index = uniform(0, 1) * size;
        // generate random bit to flip in character
        int bit = uniform(0, 8);
        // flip the bit
        message.payload[index] ^= (1 << bit);
        // set the payload again
        msg->setPayload(message.payload.c_str());
    }

    // send the original message
    sendDelayed(msg, simTime() + total_delay, "out");

    if (message.is_duplicated())
    {
        CustomMessage_Base *duplicated_msg = msg->dup();
        // add duplication delay
        total_delay += DD;
        // send the duplicated message
        sendDelayed(duplicated_msg, simTime() + total_delay, "out");
    }
}
