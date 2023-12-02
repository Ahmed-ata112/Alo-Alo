// //
// // This program is free software: you can redistribute it and/or modify
// // it under the terms of the GNU Lesser General Public License as published by
// // the Free Software Foundation, either version 3 of the License, or
// // (at your option) any later version.
// //
// // This program is distributed in the hope that it will be useful,
// // but WITHOUT ANY WARRANTY; without even the implied warranty of
// // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// // GNU Lesser General Public License for more details.
// //
// // You should have received a copy of the GNU Lesser General Public License
// // along with this program.  If not, see http://www.gnu.org/licenses/.
// //

// #include "receiver.h"
// #include "CustomMessage_m.h"

// Define_Module(Receiver);

// void Receiver::init()
// {
//     TD = par("TD").doubleValue();
//     LP = par("LP").doubleValue();
//     window_size = par("WS").doubleValue();
// }

// void Receiver::initialize()
// {
//     // TODO - Generated method body
//     init();
//     seq_num = 0;
//     logger = Logger();
// }

// void Receiver::handleMessage(cMessage *msg)
// {
//     // TODO - Generated method body
//     CustomMessage_Base *message = check_and_cast<CustomMessage_Base *>(msg);

//     // expected message
//     if (message->getHeader() == seq_num)
//     {
//         // no error in the message
//         if (check_checksum(message))
//         {
//             unframing_message(message);
//             message->setAck_num(++seq_num);
//             message->setType(control_signal::ACK);
//             EV << "received message: " << message->getPayload() << " --> seq_num: " << int(message->getHeader()) << std::endl;
//             logger.logPayloadUploading(message->getHeader(), message->getPayload());
//             logger.logControlFrameSending(1, true, (uniform(0, 1) <= LP));

//             if (!(uniform(0, 1) <= LP))
//                 sendDelayed(message, TD, "out");
//             else
//                 EV_ERROR << "ACK will be lost" << std::endl;

//             // if the window is full, reset the seq_num
//             if (seq_num == window_size + 1)
//                 seq_num = 0;
//         }
//         // error in the message
//         else
//         {
//             EV_ERROR << "there is error in the received message -->"
//                      << " seq_num: " << int(message->getHeader()) << std::endl;
//             message->setAck_num(seq_num);
//             message->setType(control_signal::NACK);
//             logger.logControlFrameSending(1, false, (uniform(0, 1) <= LP));
//             if (!(uniform(0, 1) <= LP))
//                 sendDelayed(message, TD, "out");
//             else
//                 EV_ERROR << "ACK will be lost" << std::endl;
//         }
//     }
//     else
//         EV_ERROR << "received unexpected message with seq_num: " << int(message->getHeader()) << std::endl;
// }
