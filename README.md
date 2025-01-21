# Reliable Data Transfer Protocol Implementation

A network simulation project implementing a reliable data transfer protocol using OMNeT++ framework. The implementation includes selective repeat ARQ with sliding window mechanism for error control and flow control.

## Features

- Selective Repeat ARQ protocol implementation
- Sliding window mechanism 
- Error simulation including:
  - Message modification
  - Message loss
  - Message duplication 
  - Message delay
- ACK/NACK control frames
- Timeout-based retransmission
- Frame checksum verification
- Byte stuffing for frame delimiting

## Project Structure

- `src/` - Source code files
  - `node.{h,cc}` - Node implementation for sender/receiver
  - `coordinator.{h,cc}` - Coordinator module for simulation control  
  - `CustomMessage.msg` - Message definition
  - `utilities.{h,cc}` - Helper functions
  - `Logger.{h,cc}` - Logging functionality
  - `ErroredMsg.{h,cc}` - Error simulation

- `simulations/` - Simulation configuration
  - `omnetpp.ini` - Main configuration file
  - `package.ned` - Network definition

- `inputs/` - Test input files

## Configuration Parameters

The following parameters can be configured in `omnetpp.ini`:

- `WS` - Window size (default: 3)
- `TO` - Timeout duration (default: 10.0)
- `PT` - Processing time (default: 0.5) 
- `TD` - Transmission delay (default: 1.0)
- `ED` - Extra delay for delayed messages (default: 5.0)
- `DD` - Duplication delay (default: 0.1)
- `LP` - Loss probability (default: 0.3)

## Building and Running

1. Install OMNeT++ 6.0 or later
2. Build the project:
```sh
make
```

3. Run simulation:
```sh
./simulations/run
```

## Error Codes

Messages in input files use 4-bit error codes in the format:
```
[Modification, Loss, Duplication, Delay]
```

For example:
- `1000` - Message modification only
- `0011` - Message duplication and delay
- `0100` - Message loss only

## Test Cases

The project includes various test scenarios in the `tests.txt` file covering:
- Simple timeouts
- NACK retransmissions
- Different error combinations
- ACK/NACK loss scenarios
- Window sliding behavior

