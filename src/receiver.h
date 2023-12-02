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

#ifndef __ALO_ALO_RECEIVER_H_
#define __ALO_ALO_RECEIVER_H_
#include "utilities.h"
#include <omnetpp.h>
#include "Logger.h"

using namespace omnetpp;

/**
 * TODO - Generated class
 */
// #define TD 0.5
// #define LP 0.1 // loss probability of ack and nack
// #define WS 3   // window size
class Receiver : public cSimpleModule
{
private:
  float TD;
  float LP;
  int window_size;
  char seq_num;
  Logger logger;

  void init();

protected:
  virtual void initialize() override;
  virtual void handleMessage(cMessage *msg) override;
};

#endif
