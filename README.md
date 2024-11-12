# Ping Advanced

A lightweight and high-precision CLI tool for network latency measurements, offering jitter calculations with nanosecond precision.

## Features
- **Configurable packet count and intervals** — Set the number of packets and their intervals.
- **High-precision timing** — Jitter measured in nanoseconds for accuracy.
- **Millisecond display** — Ping results displayed in milliseconds for readability.
- **Early failure detection** — Quickly identifies network issues.

## Requirements
- **OS**: Linux (Debian-based distributions)
- **Dependencies**:
  - CMake 3.10+
  - C++17 compiler
  - Root privileges or **CAP_NET_RAW** capability

## Build Instructions

To build and install **Ping Advanced**, follow these steps:

```bash
# Create and navigate to build directory
mkdir build && cd build

# Run CMake to configure
cmake ..

# Compile the source code
make

# Install the binary (requires root privileges)
sudo make install

## Usage

To use **Ping Advanced**, run the following command:
ping_adv <target_ip> <packet_count> <interval_ms>