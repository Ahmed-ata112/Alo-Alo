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
#include "CustomMessage_m.h"
#include "utilities.h"

//*
//* Sender and reciever modules and the coord just choose the input file??
//* Ack when proccessing?
//*/

Define_Module(Sender);
using namespace omnetpp;

#define PT 0.5
#define TD 0.5
#define ED 0.5
#define DD 0.5
#define TO 5
#define ACK 1
#define NACK 0

void Sender::send_message_with_error(ErroredMsg message, char seq_num)
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
    double total_delay = TD;

    if (message.is_delayed())
    {
        // if message is delayed, add extra delay
        total_delay += ED;
    }

    if (message.is_modified())
    {
        // generate random index for character in payload
        int index = int(uniform(0, size));
        // generate random bit to flip in character
        int bit = uniform(0, 8);
        // flip the bit
        message.payload[index] ^= (1 << bit);
        // set the payload again
        msg->setPayload(message.payload.c_str());
    }

    // send the original message
    EV << "sending message with seq_num: " << int(seq_num) << "\n";
    sendDelayed(msg, total_delay, "out");

    if (message.is_duplicated())
    {
        CustomMessage_Base *duplicated_msg = msg->dup();
        // add duplication delay
        total_delay += DD;
        // send the duplicated message
        sendDelayed(duplicated_msg, total_delay, "out");
    }
}

void Sender::initialize()
{
    // initialize variables
    window_size = 3; // par("window_size");
    w_start = 0;
    w_end = window_size - 1; // index of last element in window
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
        if (string(msg->getName()) == "timeout")
        {
            EV << "timeout\n";
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
            send_message_with_error(messages[w_num], w_num % (window_size));

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
        if (string(msg->getName()) == "coordinate")
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
    cMessage *to_proccessing_msg = new cMessage("to_proccessing_msg");
    to_proccessing_msg->setKind(w_next);
    // send to self after processing time
    scheduleAt(simTime() + PT, to_proccessing_msg);
    is_processing = true; // we are processing a frame now

    // increment the next frame to be sent
    w_next++;
//    cancelAndDelete(msg);
}
