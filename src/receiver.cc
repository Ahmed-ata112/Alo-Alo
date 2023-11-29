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

#include "receiver.h"
#include "CustomMessage_m.h"

#define ACK 1
#define NACK 0
#define TD 0.5
#define LP 0.1 // loss probability of ack and nack
Define_Module(Receiver);

void Receiver::initialize()
{
    // TODO - Generated method body
}

void Receiver::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    CustomMessage_Base *message = check_and_cast<CustomMessage_Base *>(msg);
    EV << "RECEIVED MSG with seq_num: " << int(message->getHeader()) << std::endl;

    message->setAck_num(int(message->getHeader()) + 1); // correct header
    message->setType(ACK);                              // ACK
    sendDelayed(message, TD, "out");
}
