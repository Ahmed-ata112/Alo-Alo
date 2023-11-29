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

bool Receiver::loss()
{
    return (uniform(0, 1) <= LP);
}

void Receiver::initialize()
{
    // TODO - Generated method body
    seq_num = 0;
}

void Receiver::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    CustomMessage_Base *message = check_and_cast<CustomMessage_Base *>(msg);

    // expected message
    if (message->getHeader() == seq_num)
    {
        // error in the message -> sent NACK
        if (check_checksum(message))
        {
            unframing_message(message);
            message->setAck_num(++seq_num);
            message->setType(ACK);
            EV << "RECEIVED MSG with seq_num: " << int(message->getHeader()) << std::endl;
            EV << "MESSAGE HAS ERROR IN IT" << std::endl;
            EV_WARN << "HAppy";
            if (!loss())
                sendDelayed(message, TD, "out");
        }
        // no error in the message
        else
        {
            message->setPayload("Error happen in the message");
            message->setAck_num(seq_num);
            message->setType(NACK);
            if (!loss())
                sendDelayed(message, TD, "out");
        }
    }
    EV << "RECEIVED MSG with seq_num: " << int(message->getHeader()) << std::endl;
}
