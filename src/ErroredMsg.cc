#include "ErroredMsg.h"

ErroredMsg::ErroredMsg(string line)
{
    payload = line.substr(ERROR_BITS + 1, line.length() - 1);
    string error_code = line.substr(0, ERROR_BITS);

    for (int i = 0; i < ERROR_BITS; i++)
        errors.push_back(error_code[i] == '1');
}

//[Modification, Loss, Duplication, Delay]
void ErroredMsg::clear_errors()
{
    errors = {false, false, false, false};
}
bool ErroredMsg::is_modified() { return errors[0]; }
bool ErroredMsg::is_duplicated() { return errors[2]; }
bool ErroredMsg::is_lost() { return errors[1]; }
bool ErroredMsg::is_delayed() { return errors[3]; }
string ErroredMsg::get_error_code()
{
    string error_code = "";
    for (int i = 0; i < ERROR_BITS; i++)
        error_code += errors[i] ? "1" : "0";

    return error_code;
}