# ALU Integration Project - sampleALU

This project demonstrates the integration of an **Arithmetic Logic Unit (ALU)** combining parametrized adder and subtractor modules.

## Project Structure

```
sampleALU/
├── include/
│   ├── parametrized_adder.h    # Template-based adder (8/16/32-bit)
│   └── alu.h                   # ALU with ADD/SUBTRACT operations
├── testbench/
│   └── tb_alu.cpp              # Comprehensive ALU testbench
├── scripts/
│   └── run.sh                  # Build and run script
├── CMakeLists.txt              # Build configuration
└── README.md                   # This file
```

## Components

### 1. Parametrized Adder (`parametrized_adder.h`)
Template-based addition module supporting multiple bit widths with overflow/underflow detection.

**Template Parameter:** BitWidth (8, 16, or 32)

**Ports:**
- Input: `a`, `b` - Operands
- Output: `sum` - Result (saturated)
- Output: `overflow_flag` - High on positive overflow
- Output: `underflow_flag` - High on negative underflow

### 2. ALU Module (`alu.h`)
Central processing unit combining addition and subtraction with comprehensive flag outputs.

**Template Parameter:** BitWidth (8, 16, or 32)

**Ports:**
- Input: `operand_a`, `operand_b` - Operands
- Input: `operation[1:0]` - Operation select:
  - `0`: ADD
  - `1`: SUBTRACT
  - `2-3`: Reserved (default to ADD)
- Output: `result` - Computation result (saturated)
- Output: `zero_flag` - High when result = 0
- Output: `sign_flag` - High when result < 0
- Output: `overflow_flag` - High on positive overflow
- Output: `underflow_flag` - High on negative underflow
- Output: `carry_flag` - High on signed overflow

### 3. ALU Testbench (`tb_alu.cpp`)
Comprehensive test suite validating ALU operations across all bit widths.

**Test Coverage:**
- 8-bit operations (Range: -128 to 127)
- 16-bit operations (Range: -32768 to 32767)
- 32-bit operations (Range: -2147483648 to 2147483647)
- Normal operations (ADD, SUBTRACT)
- Overflow/underflow saturation
- Boundary conditions
- Flag behavior validation

## Building

### Prerequisites
- SystemC library installed at `C:/msys64/usr/local/systemc`
- CMake 3.10 or later
- C++ compiler with C++17 support

### Build Steps

```bash
cd sampleALU
mkdir build
cd build
cmake ..
cmake --build .
```

## Running the Testbench

```bash
# From the build directory
./testbench/tb_alu.exe
```

### Expected Output
The testbench produces detailed test results including:
- Operand values (A, B)
- Operation type (ADD/SUBTRACT)
- Computed result
- Expected result
- Flag states (ZF, SF, OV, UF, CF)
- Pass/fail status

Example:
```
[TEST 1] 8-bit ALU (Range: -128 to 127)
A    | B    | Op | Result | Expected | ZF | SF | OV | UF | CF | Status
10   | 5    | 0  | 15     | 15       | 0  | 0  | 0  | 0  | 0  | PASS
```

## Design Highlights

### Overflow/Underflow Saturation
- Results exceeding maximum value saturate to max
- Results below minimum value saturate to min
- Maintains data type bounds without wrapping

### Flag Generation
- **Zero Flag (ZF)**: Set when result equals zero
- **Sign Flag (SF)**: Set when result is negative
- **Overflow Flag (OV)**: Set on positive overflow (saturation)
- **Underflow Flag (UF)**: Set on negative underflow (saturation)
- **Carry Flag (CF)**: Set on any overflow condition

### Template-Based Flexibility
- Single code base supports 8, 16, and 32-bit widths
- Automatic type selection based on template parameter
- Compile-time constant expressions for range limits

## Next Steps

Future enhancements could include:
- Additional operations (AND, OR, XOR, SHIFT operations)
- Conditional branching based on flags
- Multi-cycle operations
- Pipelined architecture
- Integration with control logic and memory systems

## References

- SystemC Documentation: https://www.systemc.org/
- IEEE 1666 Standard (SystemC)
- Digital Logic and Design principles
