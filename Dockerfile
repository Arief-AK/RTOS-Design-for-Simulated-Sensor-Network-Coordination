FROM ubuntu:latest

# Install essential packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    g++ \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Clone the repository
COPY . /app

# Build the project
RUN ls -la && \
    mkdir -p build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc)

# Create logs directory
RUN mkdir -p logs

# Set the entry point
ENTRYPOINT ["./build/RTOS_sensor_network"]