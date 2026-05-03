cmake_minimum_required(VERSION 3.10)
project(sampleALU)

# Set SYSTEMC_HOME path as per local installation
set(SYSTEMC_HOME "C:/msys64/usr/local/systemc")
set(ENV{SYSTEMC_HOME} ${SYSTEMC_HOME})

# Set C++ standard to C++17 (required by SystemC)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${SYSTEMC_HOME}/include)

# Define SystemC libraries
set(SYSTEMC_LIBRARIES ${SYSTEMC_HOME}/lib/libsystemc.a)

# ============================================================================
# ALU (Arithmetic Logic Unit) - Parametrized template-based design
# ============================================================================
# Combines a parametrized adder and subtractor with operation selection.
# Supports 8-bit, 16-bit, and 32-bit signed integers.
# 
# Operations:
#   - Operation 0: ADD - Add two operands
#   - Operation 1: SUBTRACT - Subtract operand_b from operand_a
#   - Operations 2-3: Reserved (default to ADD)
#
# Output Flags:
#   - zero_flag: High when result is zero
#   - sign_flag: High when result is negative
#   - overflow_flag: High when result exceeds maximum value
#   - underflow_flag: High when result is below minimum value
#   - carry_flag: High when overflow occurs (for signed arithmetic)
#
# Saturation Behavior:
#   - On overflow: Result saturates to maximum value
#   - On underflow: Result saturates to minimum value
# ============================================================================

set(TESTBENCH_ALU
    testbench/tb_alu.cpp
)

add_executable(tb_alu ${TESTBENCH_ALU})
target_link_libraries(tb_alu ${SYSTEMC_LIBRARIES})

set_target_properties(tb_alu PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/testbench
)

# ============================================================================
# Build instructions:
# cd build
# cmake ..
# cmake --build .
#
# Run testbench:
# ./testbench/tb_alu.exe
# ============================================================================
