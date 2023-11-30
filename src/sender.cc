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
#include "Logger.h"

//*
//* Sender and reciever modules and the coord just choose the input file??
//* Ack when proccessing?
//*/

Define_Module(Sender);
using namespace omnetpp;

void Sender::init()
{
    window_size = par("WS").doubleValue();
    TO = par("TO").doubleValue();
    PT = par("PT").doubleValue();
    TD = par("TD").doubleValue();
    ED = par("ED").doubleValue();
    DD = par("DD").doubleValue();
}

void Sender::send_message_with_error(ErroredMsg message, char seq_num)
{

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
    int index;
    int bit;
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

    logger.logFrameTransmission(0, seq_num, msg->getPayload(), msg->getTrailer(),
                                message.is_modified() ? index * 8 + bit : -1,
                                message.is_lost(), message.is_duplicated() ? 1 : 0, message.is_delayed() ? ED : 0);

    if (message.is_lost()) // the message lost
        return;

    sendDelayed(msg, total_delay, "out");

    if (message.is_duplicated())
    {
        CustomMessage_Base *duplicated_msg = msg->dup();
        // add duplication delay
        total_delay += DD;
        // send the duplicated message
        logger.logFrameTransmission(0, seq_num, duplicated_msg->getPayload(), duplicated_msg->getTrailer(),
                                    message.is_modified() ? index * 8 + bit : -1,
                                    message.is_lost(), 2, message.is_delayed() ? ED : 0);
        sendDelayed(duplicated_msg, total_delay, "out");
    }
}

void Sender::reset_window()
{
    // clear errors from the first frame that caused the timeout
    messages[w_start].clear_errors();

    // reset the next frame to be sent to send the window again from the start
    w_next = w_start;

    // clear all the timeouts in the event queue
    for (int i = 0; i < timeouts.size(); i++)
    {
        cancelAndDelete(timeouts[i]);
    }
    timeouts.clear(); // clear the timeouts vector
}

void Sender::initialize()
{
    std::string file = std::to_string(getIndex());
    // initialize variables
    w_start = 0;
    w_end = window_size - 1; // index of last element in window
    w_next = 0;
    is_processing = false;
    messages = readfile("input" + file + ".txt");
    logger = Logger();
    EV << "messages size: " << messages.size() << "\n";
}

void Sender::handleMessage(cMessage *msg)
{
    // from the coordinator
    if (string(msg->getName()) == "coordinate")
    {
        // NOTHING TO DO here for now -> we wil start sending the first frame in the window
    }
    // a message that just finished processing
    else if (string(msg->getName()) == "to_proccessing_msg")
    {
        EV << "Message finished processing win_num: " << msg->getKind() << "\n";
        // this is a frame that finished processing and scheduled to be sent now

        // send the frame
        int w_num = msg->getKind();
        // send the frame with the corresponding error
        send_message_with_error(messages[w_num], w_num % (window_size + 1));
        // schedule a timeout for the frame
        cMessage *timeout = new cMessage("timeout");
        // time out message at w_num( the frame to be sent)
        timeout->setKind(w_num);
        timeouts.push_back(timeout);
        // schedule the timeout after the timeout interval
        scheduleAt(simTime() + TO, timeout);
        is_processing = false; // we are not processing any frame now
    }
    // check if it is a timeout
    else if (string(msg->getName()) == "timeout")
    {
        // this is a timeout on the first frame in the window
        // resend all frames in the window
        EV << "timeout\n";
        logger.logTimeoutEvent(0, msg->getKind() % (window_size + 1));
        reset_window();
        is_processing = false; // we are not processing any frame now
    }
    else
    {
        // we will check if the frame is ACK or NACK
        // if it is ACK, we will slide the window
        // if it is NACK, we will resend the frame

        CustomMessage_Base *message = check_and_cast<CustomMessage_Base *>(msg);
        if (message->getType() == control_signal::ACK)
        {
            EV << "ACK in sender\n";
            // this is an ACK
            int ack_num = message->getAck_num();
            // a correct ACK should havee the second frame's seqNum in the window
            if (ack_num != (w_start % (window_size + 1)) + 1)
                return; // this is a wrong ACK

            EV << "RECEIVED CORRECT ACK with ack_num: " << int(ack_num) << std::endl;
            // slide the window
            w_start++;
            // update the end of the window if it is not the last frame
            w_end = (w_end == messages.size() - 1) ? w_end : w_end + 1;

            // TODO: delete from the timeouts vector
            // frame is in order so we can delete the timeout
            cancelAndDelete(timeouts[0]);
            timeouts.erase(timeouts.begin());
            // if the sender is proceessing some frame then we can't send more frames now
            if (is_processing)
                return; // the frame will be sent when it finishes processing ISA
        }
        else if (message->getType() == control_signal::NACK)
        {
            EV << "NACK in sender\n";
            // check if the NACK is for the first frame in the window or not
            int nack_num = message->getAck_num();
            if (nack_num != w_start % window_size)
                return; // this is a wrong NACK

            EV << "NACK with nack_num: " << int(message->getAck_num()) << std::endl;

            if (is_processing)
                // make it stop processing the current frame and Resend the frame that caused the NACK
                cancelAndDelete(to_proccessing_msg);
        }
    }

    if (w_next > w_end) // if the pointer exceeds the end of the window
        return;         // we can't send more frames until we receive ACKs or NACKs from the receiver or a timeout occurs

    if (w_next >= messages.size())
        return; // we finished sending all the frames el7amdolelah

    EV << "Started processing frame with num: " << w_next << "\n";
    EV << "w_next: " << w_next << "\n";
    EV << "w_start: " << w_start << "\n";
    EV << "w_end: " << w_end << "\n";

    logger.logProcessingStart(0, messages[w_next].get_error_code());
    // send the message
    to_proccessing_msg = new cMessage("to_proccessing_msg");
    to_proccessing_msg->setKind(w_next);
    // send to self after processing time
    time_to_finish_proccesing = simTime() + PT;
    scheduleAt(time_to_finish_proccesing, to_proccessing_msg);
    is_processing = true; // we are processing a frame now

    // increment the next frame to be sent
    w_next++;
}
