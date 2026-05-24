# FusionNet

FusionNet is a modern C++ networking library built around the Reactor pattern.

It provides a simple plus scalable architecture for:

* TCP servers
* TCP clients
* Event-driven networking
* Packet dispatching
* Custom packet serialization
* Multi-client management

The goal of FusionNet is to give you a clean networking core without hiding the low-level behavior of TCP.

---

# Features

## Networking

* TCP client
* TCP server
* Poll-based event loop
* Multi-client handling
* Automatic connection management

## Architecture

* Reactor pattern
* Event-driven design
* Packet dispatcher
* Callback system
* Shared connection ownership

## Packet System

* Packet IDs
* Typed packets
* Automatic packet routing
* Binary serialization
* Stream-safe message parsing

## Error Handling

* Custom networking exceptions
* Disconnect handling
* Receive/send error propagation
* Safe cleanup lifecycle

---

# Architecture

FusionNet uses the Reactor pattern.

The event loop is responsible for:

* polling sockets
* accepting connections
* receiving packets
* dispatching messages
* handling disconnects

The architecture is split into multiple layers.

```txt
NetServer
    ↓
EventLoop
    ↓
Acceptor
    ↓
Connection
    ↓
Message Parser
    ↓
PacketDispatcher
```

---

# Core Components

## NetServer

High-level server API.

Responsible for:

* starting the server
* stopping the server
* registering callbacks
* packet registration

Example:

```cpp
fus::net::NetServer server;
```

---

## EventLoop

The core reactor.

Responsible for:

* poll()
* socket events
* dispatching messages
* disconnect detection

The EventLoop owns the networking lifecycle.

Connections do not destroy themselves.

---

## Acceptor

Responsible for:

* creating listening sockets
* bind()
* listen()
* accept()

It only handles new incoming clients.

---

## Connection

Represents a TCP connection.

Responsible for:

* recv()
* send()
* stream parsing
* buffering

Connections are stored using:

```cpp
std::shared_ptr<Connection>
```

to guarantee stable ownership.

---

## PacketDispatcher

Routes messages to typed packet handlers.

Example:

```cpp
server.onPacket<MyPacket>([](auto connection, const MyPacket& packet) {
    std::cout << packet.value << std::endl;
});
```

---

# Reactor Pattern

FusionNet follows the Reactor pattern.

The workflow:

```txt
poll()
    ↓
socket becomes readable
    ↓
recv()
    ↓
message parsing
    ↓
packet dispatch
```

Important:

* only EventLoop uses poll()
* Connection never polls internally
* recv() determines disconnect state

---

# Packet System

Packets are transmitted using a message header.

## MessageHeader

```cpp
#pragma pack(push, 1)
struct MessageHeader {
    uint16_t id;
    uint32_t length;
};
#pragma pack(pop)
```

The header contains:

* packet ID
* packet body size

Headers are serialized using network byte order.

```cpp
header.id = htons(id);
header.length = htonl(size);
```

---

# Stream Parsing

TCP is a byte stream.

FusionNet uses an internal receive buffer to reconstruct packets safely.

The parser:

1. reads header
2. validates body size
3. waits for complete packet
4. dispatches message

This prevents:

* packet fragmentation issues
* partial packet corruption
* stream desynchronization

---

# Example Server

```cpp
#include "server/NetServer.hpp"
#include <iostream>

struct example_packet_data {
    static constexpr uint16_t ID = 1;

    int value;
};

int main()
{
    fus::net::NetServer server;

    server.onConnect([](auto connection) {
        std::cout << "Client connected: "
                  << connection->socket()
                  << std::endl;
    });

    server.onDisconnect([](auto connection) {
        std::cout << "Client disconnected: "
                  << connection->socket()
                  << std::endl;
    });

    server.onPacket<example_packet_data>(
        [](auto connection,
           const example_packet_data& packet)
    {
        std::cout << "Received packet value: "
                  << packet.value
                  << std::endl;
    });

    server.start(8080);

    while (server.isRunning()) {
    }

    return 0;
}
```

---

# Example Client

```cpp
#include "client/NetClient.hpp"

struct example_packet_data {
    static constexpr uint16_t ID = 1;

    int value;
};

int main()
{
    fus::net::NetClient client;

    client.connect("127.0.0.1", 8080);

    example_packet_data packet;
    packet.value = 42;

    client.send(packet);

    return 0;
}
```

---

# Threading Model

FusionNet currently uses:

* one event loop thread
* non-blocking sockets
* poll()

Future upgrades may include:

* epoll
* kqueue
* IOCP
* thread pools

---

# Error Handling

FusionNet uses custom networking exceptions.

Example:

```cpp
class DisconnectException : public NetworkException
{
};
```

The EventLoop catches networking exceptions and performs cleanup.

This keeps the architecture clean and centralized.

---

# Design Principles

FusionNet follows these principles:

* explicit ownership
* minimal hidden behavior
* event-driven architecture
* stable connection lifetime
* stream-safe TCP parsing
* scalable networking core

---

# Important Networking Notes

## TCP is a stream

TCP does not preserve packet boundaries.

FusionNet reconstructs packets manually.

Never assume:

```txt
1 send == 1 recv
```

---

## Partial sends

send() may send fewer bytes than requested.

FusionNet uses internal send loops to guarantee full transmission.

---

# Current State

FusionNet is currently focused on:

* architecture
* networking core
* packet system
* reactor implementation

Future goals:

* SSL/TLS
* UDP
* ECS integration
* RPC layer
* replication system
* reliable UDP
* game networking utilities

---

# Build

## Requirements

* C++20
* CMake
* Linux

## Build Steps

```bash
mkdir build
cd build
cmake ..
make
```

---
