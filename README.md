# TCP/IP Stack Project with TUN/TAP Device

This project is a custom implementation of a TCP/IP stack that interfaces with a TUN/TAP virtual network device. The goal is to emulate a basic network stack capable of handling Ethernet, IP, and TCP traffic, while providing a platform for understanding and experimenting with network protocols at a low level.

### features
#### TUN/TAP Integration:
    Implements a user-space network stack that communicates with a TUN/TAP device to send and receive packets.
    Supports frame injection and reception for testing and debugging.

#### Custom Ethernet Layer:
    Handles Ethernet frame parsing and construction.
    Supports padding removal and validation of incoming frames.

#### IPv4/IPv6 Protocols:
    Decodes IPv4 and IPv6 headers.
    Processes IP routing and implements packet forwarding logic.

#### TCP Protocol Implementation:
    Manages connection setup (SYN, SYN-ACK, ACK), teardown, and retransmission.
    Includes a simple congestion control and retransmission mechanism.

#### Debugging and Logging:
    Offers debug macros for easy tracing of packet flows and protocol states.
    Outputs detailed logs for received and transmitted frames.
