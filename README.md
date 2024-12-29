# IPv6 Network Stack Implementation
A lightweight IPv6 network stack implementation focusing on fundamental network protocols and low-level packet processing. This project provides hands-on experience with network protocol implementation, system-level programming, and direct hardware interface communication.

## Features
- Custom TUN/TAP interface driver for network device simulation
- IPv6 header parsing and handling  
- ICMPv6 protocol implementation
 - Neighbor Discovery Protocol (NDP)
   - ✅ Router Solicitation handling
   - ✅ Router Advertisement generation and sending
   - Neighbor Solicitation/Advertisement (planned)
   - Redirect messages (planned)
- IPv6 Extension Headers support (planned)
- Binary packet manipulation and processing
- Efficient memory management with `sk_buff`

## Technical Details

### Packet Processing
The stack processes IPv6 packets through several stages:
1. TUN interface packet capture and raw packet inspection
2. IPv6 header parsing
3. Protocol-specific handling (e.g., ICMPv6)
4. NDP message processing and response generation

### Memory Management
Uses a custom buffer management system (`sk_buff`) for efficient packet handling and memory allocation, including:
- Dynamic buffer allocation and deallocation
- Buffer data manipulation (push, pull, reset)
- Packet metadata tracking

### Protocol Implementation
- IPv6 basic header (RFC 2460)
- ICMPv6 (RFC 4443)
 - ✅ Router Advertisement/Solicitation (RFC 4861)
 - Neighbor Discovery Protocol components (in progress)
- IPv6 Extension Headers (planned)
