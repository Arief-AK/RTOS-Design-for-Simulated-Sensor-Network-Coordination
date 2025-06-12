# RTOS Sensor Network Simulation

[![CMake on multiple platforms](https://github.com/Arief-AK/RTOS-Design-for-Simulated-Sensor-Network-Coordination/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Arief-AK/RTOS-Design-for-Simulated-Sensor-Network-Coordination/actions/workflows/cmake-multi-platform.yml)

A C++ implementation of a real-time operating system (RTOS) simulation for coordinating sensor networks.

## Overview

This project simulates a real-time operating system designed for sensor network coordination. It implements core RTOS components including:

- Rate Monotonic scheduler
- Priority scheduler
- Task control and management
- Logging functionality
- Circular buffer task management
- Producer-consumer task simulation strategy

## Building the Project

### Prerequisites

- CMake (version 3.10 or higher)
- C++17 compatible compiler
- Make

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```
