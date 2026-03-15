# redisInC

A lightweight, high-performance, in-memory key-value data store inspired by Redis, implemented from scratch in C++.

## 🚀 Features

- **Client-Server Architecture:** Custom TCP-based client and server event loops designed for handling multiple connections.
- **Multithreading:** Integrated thread pool (`thread_pool.cpp`) to handle asynchronous background tasks efficiently without blocking the main event loop.
- **Custom Data Structures:** Built completely from scratch to manage memory and performance:
  - **Hash Tables** (`hashtable.cpp`): For core O(1) key-value lookups.
  - **AVL Trees** (`avl.cpp`): For self-balancing binary search trees.
  - **Z-Sets (Sorted Sets)** (`zset.cpp`): Combining hash tables and AVL trees for ordered data operations.
  - **Min-Heaps** (`heap.cpp`): For managing TTLs (Time-To-Live) and timer-based events.
  - **Doubly Linked Lists** (`list.h`): For sequential data storage.

## 📁 Repository Structure

- `server.cpp` / `client.cpp` - Core networking, event loops, and command processing.
- `hashtable.cpp` / `hashtable.h` - Dictionary implementation for the main key space.
- `avl.cpp` / `avl.h` / `test_avl.cpp` - AVL tree implementation and testing suite.
- `zset.cpp` / `zset.h` - Sorted set implementation using AVL trees and hash maps.
- `heap.cpp` / `heap.h` - Min-heap data structure for TTL and timeouts.
- `thread_pool.cpp` / `thread_pool.h` - Worker pool for background execution.
- `utils.cpp` / `utils.h` - Networking and general helper utilities.
- `Makefile` - Build configuration.

## 🛠️ Building and Running

### Prerequisites
- A modern C++ compiler (supports C++11 or higher)
- `make`

### Build Instructions

To compile the project, simply run `make` in the root directory:

```bash
make
```

This will compile the necessary object files and build the executables.

### Running the Server
Start the server listening for incoming connections:

```bash
./server
```

### Running the Client
In a separate terminal window, start the client to send commands to the server:

```bash
./client
```

## 📄 License
This project is open-source and available under the MIT License.
