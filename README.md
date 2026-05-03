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

## System Architecture Diagram

```
                    PARAMETRIZED ADDER (8/16/32-bit)
                    ┌────────────────────────────────┐
                    │  parametrized_adder<BitWidth>  │
   a ──────────────►│                                ├──────► sum
                    │ • Performs signed addition     │
   b ──────────────►│ • Detects overflow/underflow   ├──────► overflow_flag
                    │ • Saturates result to range    │
                    │                                ├──────► underflow_flag
                    └────────────────────────────────┘


               ╔════════════════════════════════════════════════╗
               ║                                                ║
               ║         ALU (MAIN COMPUTE UNIT)                ║
               ║      ┌──────────────────────────────┐          ║
               ║      │     alu<BitWidth>            │          ║
               ║      │                              │          ║
operand_a ────►├─────►│  Input Ports:                │          ║
               ║      │  • operand_a (signed int)    │          ║
operand_b ────►├─────►│  • operand_b (signed/shift)  │          ║
               ║      │  • operation[2:0] (selector) │          ║
               ║      │                              │          ║
operation[2:0]►├─────►│  Core Logic:                 │          ║
(8 operations) ║      │  • Arithmetic Unit (ADD/SUB) │          ║
               ║      │  • Logic Unit (AND/OR/XOR)   │          ║
               ║      │  • Shifter (SHL/SHR)         │          ║
               ║      │  • Operation Multiplexer     │          ║
               ║      │  • Saturation Logic          │          ║
               ║      │  • Flag Generator            │          ║
               ║      │                              │          ║
               ║      │  Output Ports:               │          ║
               ║      │  • result                    ├─────────►result
               ║      │  • zero_flag                 ├─────────►zero_flag
               ║      │  • sign_flag                 ├─────────►sign_flag
               ║      │  • overflow_flag             ├─────────►overflow_flag
               ║      │  • underflow_flag            ├─────────►underflow_flag
               ║      │  • carry_flag                ├─────────►carry_flag
               ║      │                              │          ║
               ║      └──────────────────────────────┘          ║
               ║                                                ║
               ╚════════════════════════════════════════════════╝

OPERATION SELECTOR (operation[2:0]):
  • 0 (0b000): ADD             → result = operand_a + operand_b
  • 1 (0b001): SUBTRACT        → result = operand_a - operand_b
  • 2 (0b010): AND (bitwise)   → result = operand_a & operand_b
  • 3 (0b011): OR (bitwise)    → result = operand_a | operand_b
  • 4 (0b100): XOR (bitwise)   → result = operand_a ^ operand_b
  • 5 (0b101): LEFT SHIFT      → result = operand_a << (operand_b & 0x1F)
  • 6 (0b110): RIGHT SHIFT     → result = operand_a >> (operand_b & 0x1F) [arithmetic]
  • 7 (0b111): Reserved        → defaults to ADD

OUTPUT FLAGS:
  • result:         Saturated arithmetic result
  • zero_flag:      1 when result = 0
  • sign_flag:      1 when result < 0
  • overflow_flag:  1 when result exceeds max value (saturation triggered)
  • underflow_flag: 1 when result below min value (saturation triggered)
  • carry_flag:     1 on any overflow/underflow condition

SUPPORTED BIT WIDTHS (Template Parameter):
  • 8-bit:  Range [-128, 127]
  • 16-bit: Range [-32768, 32767]
  • 32-bit: Range [-2147483648, 2147483647]
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
Central processing unit supporting arithmetic, logic, and shift operations with comprehensive flag outputs.

**Template Parameter:** BitWidth (8, 16, or 32)

**Ports:**
- Input: `operand_a`, `operand_b` - Operands (operand_b also used as shift amount for shift operations)
- Input: `operation[2:0]` - Operation select (8 operations):
  - `0`: ADD (arithmetic)
  - `1`: SUBTRACT (arithmetic)
  - `2`: AND (bitwise logic)
  - `3`: OR (bitwise logic)
  - `4`: XOR (bitwise logic)
  - `5`: LEFT SHIFT (shift by operand_b amount)
  - `6`: RIGHT SHIFT (arithmetic shift, preserves sign)
  - `7`: Reserved (defaults to ADD)
- Output: `result` - Computation result (saturated for arithmetic ops)
- Output: `zero_flag` - High when result = 0
- Output: `sign_flag` - High when result < 0
- Output: `overflow_flag` - High on positive overflow (arithmetic ops only)
- Output: `underflow_flag` - High on negative underflow (arithmetic ops only)
- Output: `carry_flag` - High on overflow/underflow condition

### 3. ALU Testbench (`tb_alu.cpp`)
Comprehensive test suite validating ALU operations across all bit widths.

**Test Coverage:**
- 8-bit operations (Range: -128 to 127)
- 16-bit operations (Range: -32768 to 32767)
- 32-bit operations (Range: -2147483648 to 2147483647)
- Arithmetic operations (ADD, SUBTRACT) with saturation
- Logic operations (AND, OR, XOR)
- Shift operations (LEFT SHIFT, RIGHT SHIFT)
- Overflow/underflow saturation
- Boundary conditions
- Flag behavior validation across all operation types

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
- Operation type (ADD/SUB/AND/OR/XOR/SHL/SHR)
- Computed result
- Expected result
- Flag states (ZF, SF, OV, UF, CF)
- Pass/fail status

Example:
```
[TEST 1] 8-bit ALU - Arithmetic Operations (Range: -128 to 127)
A    | B    | Op | Result | Expected | ZF | SF | OV | UF | CF | Status
10   | 5    | 0  | 15     | 15       | 0  | 0  | 0  | 0  | 0  | PASS

[TEST 2] 8-bit ALU - Logic Operations (Range: -128 to 127)
A    | B    | Op | Result | Expected | ZF | SF | OV | UF | CF | Status
12   | 10   | 2  | 8      | 8        | 0  | 0  | 0  | 0  | 0  | PASS

[TEST 3] 8-bit ALU - Shift Operations (Range: -128 to 127)
A    | B    | Op | Result | Expected | ZF | SF | OV | UF | CF | Status
4    | 2    | 5  | 16     | 16       | 0  | 0  | 0  | 0  | 0  | PASS
```

## Design Highlights

### Overflow/Underflow Saturation (Arithmetic Operations)
- Results exceeding maximum value saturate to max
- Results below minimum value saturate to min
- Maintains data type bounds without wrapping
- Logic and shift operations bypass saturation logic

### Operation Categories
- **Arithmetic:** ADD, SUBTRACT (with overflow/underflow detection)
- **Logic:** AND, OR, XOR (bitwise, no saturation)
- **Shift:** LEFT SHIFT, RIGHT SHIFT (arithmetic shift preserves sign)

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
- Unified operation interface for 8 different operations

## Implemented Features

✅ **Arithmetic Operations**
- ADD with overflow/underflow detection
- SUBTRACT with saturation logic

✅ **Bitwise Logic Operations**
- AND (bitwise AND)
- OR (bitwise OR)
- XOR (bitwise XOR)

✅ **Shift Operations**
- LEFT SHIFT (logical shift, up to 31 positions)
- RIGHT SHIFT (arithmetic shift, preserves sign)

✅ **Flag System**
- Zero, Sign, Overflow, Underflow, and Carry flags
- Consistent flag behavior across all operations

## Next Steps

Future enhancements could include:
- Conditional branching based on flags
- Multi-cycle operations
- Pipelined architecture
- Integration with control logic and memory systems

## References

- SystemC Documentation: https://www.systemc.org/
- IEEE 1666 Standard (SystemC)
- Digital Logic and Design principles
