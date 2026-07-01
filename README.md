# AegisKV - Multi-Threaded TCP Datastore

A high-performance, persistent, in-memory key-value data store written in standard C++17. 
Engineered with a true Client-Server architecture utilizing POSIX TCP sockets, AegisKV mimics production caching engines. It features thread-safe concurrent connections, O(1) access times, and a custom network REPL.

## Core Features
- **Concurrent Client-Server Architecture**: Uses `<sys/socket.h>` and `<thread>` to handle multiple clients simultaneously without blocking.
- **Thread-Safe Operations**: Utilizes `std::mutex` to ensure data integrity during concurrent read/write operations.
- **In-Memory Storage**: Fast lookups using standard STL hash maps.
- **TTL (Time-to-Live)**: Passive invalidation mechanism to automatically expire keys after a set duration.
- **Data Persistence**: Disk-based serialization and state recovery.

## Prerequisites
- Linux/macOS or WSL (Windows Subsystem for Linux)
- `g++` compiler with C++17 support
- `make`

## Build Instructions
Navigate to the root directory and build the project using Make:
```bash
make