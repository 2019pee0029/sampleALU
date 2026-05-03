#!/bin/bash
# Build and run script for ALU Integration Project (sampleALU)

set -e  # Exit on error

echo "=================================="
echo "ALU Integration Project Build"
echo "=================================="

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

# Navigate to build directory
cd build

# Configure with CMake
echo ""
echo "Configuring project with CMake..."
cmake ..

# Build the project
echo ""
echo "Building ALU testbench..."
cmake --build .

# Navigate back to root
cd ..

echo ""
echo "=================================="
echo "Build Complete!"
echo "=================================="
echo ""
echo "Running ALU testbench..."
echo "=================================="
echo ""

# Run the testbench
./build/testbench/tb_alu.exe

echo ""
echo "=================================="
echo "Execution Complete!"
echo "=================================="
echo ""

