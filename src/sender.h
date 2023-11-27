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

#ifndef __ALO_ALO_SENDER_H_
#define __ALO_ALO_SENDER_H_

#include <omnetpp.h>

using namespace omnetpp;
class ErroredMsg;
/**
 * TODO - Generated class
 */
class Sender : public cSimpleModule
{
  int window_size;
  int w_start; // first frame in window
  int w_end;   // last frame in window
  int w_next;  // next frame to be sent
  std::vector<ErroredMsg> messages;

  // timeouts for each frame in the window
  // we need to keep track of them to cancel them when a frame is received correctly OR a timeout occured and we need to resend the window
  std::vector<cMessage *> timeouts;
  bool is_processing; // flag to indicate if a frame is being processed
protected:
  virtual void initialize() override;
  virtual void handleMessage(cMessage *msg) override;
  void send_message_with_error(ErroredMsg message, char seq_num);
};

#endif
