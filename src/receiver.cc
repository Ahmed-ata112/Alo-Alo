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
        // no error in the message
        if (check_checksum(message))
        {
            unframing_message(message);
            message->setAck_num(++seq_num);
            message->setType(ACK);
            EV << "received message: " << message->getPayload() << " --> seq_num: " << int(message->getHeader()) << std::endl;
            if (!loss())
                sendDelayed(message, TD, "out");
            else
                EV_ERROR << "ACK will be lost" << std::endl;
        }
        // error in the message
        else
        {
            EV_ERROR << "there is error in the received message -->"
                     << " seq_num: " << int(message->getHeader()) << std::endl;
            message->setAck_num(seq_num);
            message->setType(NACK);
            if (!loss())
                sendDelayed(message, TD, "out");
            else
                EV_ERROR << "ACK will be lost" << std::endl;
        }
    }
    else
        EV_ERROR << "received unexpected message with seq_num: " << int(message->getHeader()) << std::endl;
}
