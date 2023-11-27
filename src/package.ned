package alo_alo;

@license(LGPL);
//
// TODO documentation
//
network Network
{
    submodules:
        sender: Sender {
            @display("p=63,80");
        }
        receiver: Receiver {
            @display("p=164,77");
        }
    connections:
        sender.out --> receiver.in;
        receiver.out --> sender.in;
}

