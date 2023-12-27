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

#include "node.h"
#include "CustomMessage_m.h"
#include "utilities.h"
#include "Logger.h"

Define_Module(Node);

void Node::initialize()
{
    // TODO - Generated method body

    // -------------------------receiver----------------------------
    LP = par("LP").doubleValue();
    expected_seq_num = 0;
    n_msgs_received = 0;
    // ------------------------ sender -----------------------------
    TO = par("TO").doubleValue();
    PT = par("PT").doubleValue();
    TD = par("TD").doubleValue();
    ED = par("ED").doubleValue();
    DD = par("DD").doubleValue();
    window_size = par("WS").intValue(); // 3 by default

    // initialize variables for sender
    w_start = 0;
    w_end = window_size - 1; // index of last element in window
    w_next = 0;
    is_processing = false;

    // ----------------------- both -------------------------------

    TD = par("TD").doubleValue();
    logger = Logger();
    is_receiver = true;
}

void Node::handleMessage(cMessage *msg)
{
    // This message is from the coordinator
    // I'am the Sender now
    if (string(msg->getName()) == "coordinate")
    {
        // the message comes to the sender
        is_receiver = false;
        // read from the file and load messages
        std::string file = std::to_string(getIndex());
        messages = readfile("input" + file + ".txt");
        n_messages = messages.size();
        EV << "messages size: " << messages.size() << "\n";
        EV << "Started processing frame with num: " << w_next << "\n";
        EV << "w_next: " << w_next << "\n";
        EV << "w_start: " << w_start << "\n";
        EV << "w_end: " << w_end << "\n";

        logger.logProcessingStart(0, messages[w_next].get_error_code());
        // send the message at the first begining
        to_proccessing_msg = new cMessage("to_proccessing_msg");
        to_proccessing_msg->setKind(w_next);
        // send to self after processing time
        time_to_finish_proccesing = simTime() + PT;
        scheduleAt(time_to_finish_proccesing, to_proccessing_msg);
        is_processing = true; // we are processing a frame now
        // increment the next frame to be sent
        w_next++;
    }
    else
    {
        // act as a receiver or sender now
        if (is_receiver)
            handleMessage_receiver(msg);
        else
            handleMessage_sender(msg);
    }
}

// receiver member functions
void Node::handleMessage_receiver(cMessage *msg)
{
    CustomMessage_Base *message = check_and_cast<CustomMessage_Base *>(msg);

    bool is_lost = (uniform(0, 1) <= LP);
    // expected message??
    if (message->getHeader() == expected_seq_num)
    {
        // no error in the message
        if (check_checksum(message))
        {
            expected_seq_num = (expected_seq_num + 1) % (window_size + 1);
            unframing_message(message);
            message->setAck_num(expected_seq_num);
            message->setType(control_signal::ACK);
            EV << "received message: " << message->getPayload() << " --> seq_num: " << int(message->getHeader()) << std::endl;
            logger.logPayloadUploading(message->getHeader(), message->getPayload());
            logger.logControlFrameSending(getIndex(), true, expected_seq_num, is_lost);

            if (!is_lost)
                sendDelayed(message, TD, "out");
            else
                EV_ERROR << "ACK will be lost" << std::endl;

            // if the window is full, reset the seq_num
        }
        // error in the message
        else
        {
            EV_ERROR << "There is error in the received message -->"
                     << " seq_num: " << int(message->getHeader()) << std::endl;
            message->setAck_num(expected_seq_num);
            message->setType(control_signal::NACK);
            logger.logControlFrameSending(getIndex(), false, expected_seq_num, is_lost);
            if (!is_lost)
                sendDelayed(message, TD, "out");
            else
                EV_ERROR << "NACK will be lost" << std::endl;
        }
    }
    // out of order message -> send Ack with the needed frame
    else
    {
        // I already received all messages
        // send an ack for all the messages
        message->setAck_num(expected_seq_num);
        message->setType(control_signal::ACK);
        logger.logControlFrameSending(getIndex(), true, expected_seq_num, is_lost);

        if (!is_lost)
            sendDelayed(message, TD, "out");
        else
            EV_ERROR << "ACK will be lost" << std::endl;
    }
}

int Node::ack_distance_from_start(int ack_num, bool is_nack = false)
{
    int max_seq_num = window_size + 1;
    int win_start = (w_start + (is_nack ? 0 : 1)) % (max_seq_num);
    int win_end = (w_end + (is_nack ? 0 : 1)) % (max_seq_num);
    // 1 2 3
    if ((win_start <= win_end && ack_num >= win_start && ack_num <= win_end) ||
        (win_start > win_end && (ack_num >= win_start || ack_num <= win_end)))
    {
        // ACK is within the window
        int distance = (ack_num - (win_start - (is_nack ? 0 : 1)) + max_seq_num) % max_seq_num;
        return distance;
    }
    else
    {
        // ACK is not within the window
        return -1;
    }
}

// sender member functions
void Node::handleMessage_sender(cMessage *msg)
{
    // a message that just finished processing
    if (string(msg->getName()) == "to_proccessing_msg")
    {
        EV << "Message finished processing win_num: " << msg->getKind() << "\n";
        // this is a frame that finished processing and scheduled to be sent now

        // send the frame
        int w_num = msg->getKind();
        // cancelEvent(msg);
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
        logger.logTimeoutEvent(getIndex(), msg->getKind() % (window_size + 1));
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
            // a correct ACK should have a future num in the seq
            // TODO: implement in between function OR
            int d_ack = ack_distance_from_start(ack_num);
            if (d_ack <= 0)
            {
                return; // this is an ack from the past
            }

            // if (ack_num < ((w_start + 1) % (window_size + 1)))
            //     return; // this is a wrong ACK

            EV << "RECEIVED CORRECT ACK with ack_num: " << int(ack_num) << std::endl;
            // slide the window
            w_start += d_ack;
            // update the end of the window if it is not the last frame
            w_end = (w_end + d_ack > messages.size() - 1) ? messages.size() - 1 : w_end + d_ack;

            // TODO: delete from the timeouts vector
            // frame is in order so we can delete the timeout

            // delete the all acked frames
            for (int i = 0; i < d_ack; i++)
            {
                cancelAndDelete(timeouts[0]);
                timeouts.erase(timeouts.begin());
            }

            // if the sender is proceessing some frame then we can't send more frames now
            if (is_processing)
                return; // the frame will be sent when it finishes processing ISA
        }
        else if (message->getType() == control_signal::NACK)
        {
            EV << "NACK in sender\n";
            // check if the NACK is for the first frame in the window or not
            int nack_num = message->getAck_num();

            int d_ack = ack_distance_from_start(nack_num, true);
            if (d_ack < 0)
                return; // this is a wrong NACK from the past

            // This is an Accumulative nack
            w_start += d_ack;
            // update the end of the window if it is not the last frame
            w_end = (w_end + d_ack > messages.size() - 1) ? messages.size() - 1 : w_end + d_ack;

            // TODO: delete from the timeouts vector
            // frame is in order so we can delete the timeout

            // delete the all acked frames
            for (int i = 0; i < d_ack; i++)
            {
                cancelAndDelete(timeouts[0]);
                timeouts.erase(timeouts.begin());
            }

            EV << "NACK with nack_num: " << int(message->getAck_num()) << std::endl;
            reset_window();
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

void Node::send_message_with_error(ErroredMsg message, char seq_num)
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
        string framed_payload = msg->getPayload();
        // generate random index for character in payload
        index = int(uniform(0, framed_payload.size()));
        // generate random bit to flip in character
        bit = uniform(0, 8);
        // flip the bit
        /*message.payload[index] ^= (1 << bit); this is a wrong line because we must flip the bit after framing not before*/
        framed_payload[index] ^= (1 << bit);
        // set the payload again
        msg->setPayload(framed_payload.c_str());
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

void Node::reset_window()
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

    if (is_processing)
    { // make it stop processing the current frame and Resend the frame that caused the NACK or the timeout
        cancelAndDelete(to_proccessing_msg);
        is_processing = false;
    }
}
