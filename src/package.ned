package alo_alo;

@license(LGPL);
//
// TODO documentation
//
network Network
{
    @display("bgb=440,242");
    submodules:
        sender: Sender {
            @display("p=63,80");
        }
        receiver: Receiver {
            @display("p=362,85");
        }
        coordinator: Coordinator {
            @display("p=208,176");
        }
    connections:
        sender.out --> receiver.in;
        receiver.out --> sender.in;
        coordinator.out --> sender.coord_in;
}

