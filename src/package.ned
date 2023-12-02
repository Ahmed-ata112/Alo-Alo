package alo_alo;

@license(LGPL);
//
// TODO documentation
//
network Network
{
    @display("bgb=440,242");
    submodules:
        node[2] : Node;
        coordinator: Coordinator {
            @display("p=197,180");
        }
    connections:
        node[0].out --> node[1].in;
        node[1].out --> node[0].in;
        coordinator.out[0] --> node[0].coord_in;
        coordinator.out[1] --> node[1].coord_in;
}

