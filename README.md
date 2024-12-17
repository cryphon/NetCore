# IPv6 Network Stack Implementation

A lightweight IPv6 network stack implementation focusing on fundamental network protocols and low-level packet processing. This project provides hands-on experience with network protocol implementation, system-level programming, and direct hardware interface communication.

## Features

- Custom TUN/TAP interface driver for network device simulation
- IPv6 header parsing and handling
- ICMPv6 protocol implementation
 - Router Solicitation/Advertisement
 - Neighbor Solicitation/Advertisement
 - Redirect messages
- IPv6 Extension Headers support
- Binary packet manipulation and processing
- Efficient memory management with `sk_buff`

## Technical Details

### Packet Processing

The stack processes IPv6 packets through several stages:
1. TUN interface packet capture
2. IPv6 header parsing
3. Extension headers handling
4. ICMPv6 protocol processing

### Memory Management

Uses a custom buffer management system (`sk_buff`) for efficient packet handling and memory allocation.

### Protocol Implementation

- IPv6 basic header (RFC 2460)
- ICMPv6 (RFC 4443)
- IPv6 Extension Headers
