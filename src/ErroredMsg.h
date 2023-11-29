#ifndef ERRORED_MSG_H_
#define ERRORED_MSG_H_

#include <iostream>
#include <string>
#include <vector>

#define ERROR_BITS 4

using namespace std;

class ErroredMsg
{
public:
    string payload;
    string error_code;
    vector<bool> errors;
    ErroredMsg(string line);
    //[Modification, Loss, Duplication, Delay]
    void clear_errors();
    bool is_modified();
    bool is_duplicated();
    bool is_lost();
    bool is_delayed();
    string get_error_code();
};
#endif