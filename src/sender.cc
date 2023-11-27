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

#include "sender.h"
#include "utilities.h"
#include "CustomMessage_m.h"

Define_Module(Sender);

#define PT 0.5
#define TD 0.5
#define ED 0.5
#define DD 0.5
#define TO 5
#define ACK 1
#define NACK 0
void Sender::initialize()
{
    // initialize variables
    window_size = 4; // par("window_size");
    w_start = 0;
    w_end = window_size - 1;
    w_next = 0;
    is_processing = false;
    messages = readfile("input0.txt");
}

void Sender::handleMessage(cMessage *msg)
{
    // check if the message is a self message
    if (msg->isSelfMessage())
    {
        // this msg is a self message
        // it can be either a timeout or a frame finished processing

        // check if it is a timeout
        if (msg->getName() == "timeout")
        {

            // this is a timeout on the first frame in the window
            // resend all frames in the window

            // clear errors from the first frame that caused the timeout
            messages[w_start].clear_errors();

            // reset the next frame to be sent to send the window again from the start
            w_next = w_start;

            // clear all the timeouts in the event queue
            for (int i = 0; i < timeouts.size(); i++)
            {
                cancelAndDelete(timeouts[i]);
            }
            timeouts.clear();      // clear the timeouts vector
            is_processing = false; // we are not processing any frame now
        }
        else
        {
            // this is a frame scheduled to be sent now
            // send the frame
            int w_num = msg->getKind();
            // send the frame with the corresponding error
            send_message_with_error(messages[w_num], w_num % (window_size), TD, ED, DD);

            // schedule a timeout for the frame
            cMessage *timeout = new cMessage("timeout");
            // time out message at w_num( the frame to be sent)
            timeout->setKind(w_num);
            timeouts.push_back(timeout);
            // schedule the timeout after the timeout interval
            scheduleAt(simTime() + TO, timeout);
            is_processing = false; // we are not processing any frame now
        }
    }
    else
    {
        // the message is from the coordinator or the receiver

        // from the coordinator
        // we will send the first frame in the window
        // and schedule a timeout for it
        if (msg->getName() == "coordinate")
        {
            // NOTHING TO DO here for now
        }
        else
        {
            // from the receiver
            // we will check if the frame is ACK or NACK
            // if it is ACK, we will slide the window
            // if it is NACK, we will resend the frame

            CustomMessage_Base *message = check_and_cast<CustomMessage_Base *>(msg);

            if (message->getType() == ACK)
            {
                // this is an ACK

                int ack_num = message->getAck_num();
                // a correct ACK should havee the second frame's seqNum in the window
                if (ack_num != (w_start % window_size) + 1)
                    return; // this is a wrong ACK

                // slide the window
                w_start++;
                w_end++;
                // TODO: delete from the timeouts vector
                // if the sender is proceessing some frame then we can't send more frames now
                if (is_processing)
                    return; // the frame will be sent when it finishes processing ISA
            }
            else
            {
            }
        }
    }

    if (w_next > w_end) // if the pointer reached the end of the window
        return;         // we can't send more frames until we receive ACKs or NACKs from the receiver or a timeout occurs

    if (w_next >= messages.size())
        return; // we finished sending all the frames

    // send the message
    cMessage *to_proccessing_msg = new cMessage("msg");
    to_proccessing_msg->setKind(w_next);
    // send to self after processing time
    scheduleAt(simTime() + PT, to_proccessing_msg);
    is_processing = true; // we are processing a frame now

    // increment the next frame to be sent
    w_next++;
    cancelAndDelete(msg);
}
