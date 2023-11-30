package alo_alo;

@license(LGPL);
//
// TODO documentation
//
network Network
{
    @display("bgb=440,242");
    submodules:
        coordinator: Coordinator {
            @display("p=196,176");
        }
        receiver: Receiver {
            @display("p=358,66");
        }
        sender: Sender {
            @display("p=51,60");
        }
    connections:
        sender.out --> receiver.in;
        receiver.out --> sender.in;
        coordinator.out[0] --> sender.coord_in;
        coordinator.out[1] --> receiver.coord_in;
}

